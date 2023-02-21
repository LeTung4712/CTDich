/* Scanner
thay doi tap luat theo yeu cau
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

// macros
#define tokenRead1(type)({\
  token = makeToken(type, lineNo, colNo);\
  readChar();\
})



void skipBlank();
void skipComment();
Token* readIdentKeyword(void);
Token* readNumber(void);
Token* readConstChar(void);




int scan(char *fileName);
Token* getToken(void);
void printToken(Token *token);

int scan(char *fileName){
    Token *token;
    
    if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

    token = getToken();
    while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
    }
    
    free(token);
    closeInputStream();
    return IO_SUCCESS;
}

Token* getToken(void) {
  Token *token;
  // int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE:
    skipBlank();
    return getToken();

  case CHAR_LETTER: 
  case CHAR_UNDERSCORE:
    return readIdentKeyword();
  
  case CHAR_DIGIT:
    return readNumber();

  case CHAR_SEMICOLON:
    tokenRead1(SB_SEMICOLON);
    return token;

  case CHAR_COLON: // assign
    readChar();
    if(currentChar == '='){
      token = makeToken(SB_ASSIGN, lineNo, colNo-1);
      readChar();
    }else{
      token =  makeToken(SB_COLON, lineNo, colNo-1);
    }
    return token;

  case CHAR_SLASH: // division | comment
    readChar();
    if(currentChar == '/' || currentChar == '*'){ // >=
      skipComment();
      readChar();
    }else{
      token = makeToken(SB_SLASH, lineNo, colNo-1);
    }
    return token;    

  case CHAR_PLUS: // addition
    tokenRead1(SB_PLUS);
    return token;

  case CHAR_MINUS: // subtraction
    tokenRead1(SB_MINUS);
    return token;    
  
  case CHAR_TIMES: // addition
    tokenRead1(SB_TIMES);
    return token;    
  
  case CHAR_EXCLAIMATION: // difference
    readChar();
    if(currentChar == '='){
      token = makeToken(SB_NEQ, lineNo, colNo-1);
      readChar();
      return token;
    }
    currentChar = CHAR_UNKNOWN;
    colNo--;
    return getToken();
  
  case CHAR_EQ:
    tokenRead1(SB_EQ);
    return token;    

  case CHAR_GT:
    readChar();
    if(currentChar == '='){ // >=
      token = makeToken(SB_GE, lineNo, colNo);
      readChar();
    }else{
      token = makeToken(SB_GT, lineNo, colNo-1);
    }
    return token;

  case CHAR_LT:
    readChar();
    if(currentChar == '='){ // <=
      token = makeToken(SB_LE, lineNo, colNo);
      readChar();
    }else{
      token = makeToken(SB_LT, lineNo, colNo-1);
    }
    return token;

  case CHAR_SINGLEQUOTE: // read constant string
    return readConstChar();

  case CHAR_LPAR:
    tokenRead1(SB_LPAR);
    return token;

  case CHAR_RPAR:
    tokenRead1(SB_RPAR);
    return token;

  case CHAR_LBRACKET: // index [{number}]
    readChar();
    if(charCodes[currentChar] != CHAR_DIGIT)
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
    else
      token = makeToken(SB_LBRACKET, lineNo, colNo);

    return token;

  case CHAR_RBRACKET:
    tokenRead1(SB_RBRACKET);
    return token;

  /*
  case CHAR_SINGLEQUOTE: // constant string
    token = readConstChar();

    return token;
  */    

  case CHAR_COMMA:
    tokenRead1(SB_COMMA);
    return token;

  case CHAR_PERIOD:
    tokenRead1(SB_PERIOD);
    return token;    

  case CHAR_POUND:
    skipComment();
    return getToken();

  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}

void skipBlank() {
  while (charCodes[currentChar] == CHAR_SPACE
      && currentChar != EOF){
    readChar();
  }
}

void skipComment() {
  // comments start with (*
  int oldLineNo = lineNo;
  int oldColNo = colNo;

  // multiple line comment
  if(currentChar == '*'){
    while(1){
      readChar();

      // end of comment not found
      if(currentChar == EOF)
        error(ERR_ENDOFCOMMENT, oldLineNo, oldColNo);

      // end of multiple-line comment
      if(currentChar == '*'){
        readChar();
        if(currentChar == '/'){
          readChar();
          return;
        }
      }
    }
  }
  // inline comment
  else if(currentChar=='/'){
    while(1){
      readChar();
      if(currentChar == EOF || currentChar == '\n'){
        readChar();
        return;
      }
    }
  }
  // comments start with #
  else{
    while(currentChar != '\n' && currentChar != EOF)
      readChar();
  }
}

Token* readNumber(void) {
  Token* token = makeToken(TK_NUMBER, lineNo, colNo);

  size_t tokenLen = 0;

  while(charCodes[currentChar] == CHAR_DIGIT){
    token->string[tokenLen++] = currentChar;
    readChar();
  }
  token->string[tokenLen] = '\0';
  sscanf(token->string, "%d", &(token->value));
  return token;
}

Token* readConstChar(void) {
  // TODO
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
  
  size_t tokenLen = 0;

  token->string[tokenLen++] = currentChar;
  token->string[tokenLen++] = '\0';

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
    readChar();
  }else{ // const string
    appendStr(token, token->string[0]);
    while(currentChar!='\''){
      appendStr(token, currentChar);
      readChar();
      if(currentChar==EOF)
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    }
    appendStr(token, '\0');
    token->tokenType = TK_STR;
    readChar();
  }
  return token;
}

Token* readIdentKeyword(void) {
  Token* token = makeToken(TK_IDENT, lineNo, colNo);

  size_t tokenLen = 0;

  int isAllCapital = 1;

  while(charCodes[currentChar] == CHAR_LETTER
    || charCodes[currentChar] == CHAR_DIGIT
    || currentChar == (int)'_'){

    // check maximum ident length
    if(tokenLen > MAX_IDENT_LEN){
      error(ERR_IDENTTOOLONG, lineNo, colNo);
      return makeToken(TK_NONE, lineNo, colNo);
    }

    if(currentChar>='a' && currentChar<='z')
      isAllCapital = 0;

    // append new character
    token->string[tokenLen++] = currentChar;

    readChar();
  }
  // append terminate character
  token->string[tokenLen] = '\0';
  

  // check keyword
  int tokenType = checkKeyword(token->string);
  if(tokenType != TK_NONE) // a keyword
  {
    if(!isAllCapital){
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
    }
    token->tokenType = tokenType;
  }

  return token;
}


void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%d)\n", token->value); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_STR:
    printf("TK_STR(\'%s\')\n", token->string);
    free(token->string);
    break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LBRACKET: printf("SB_LBRACKET\n"); break;
  case SB_RBRACKET: printf("SB_RBRACKET\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}
//=====================================================================================

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}

