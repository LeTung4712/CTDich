#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define NUM_OF_ERRORS 29

struct ErrorMessage {
  ErrorCode errorCode;
  char *message;
};

struct ErrorMessage errors[29] = {
//*****lỗi từ vựng
  {ERR_END_OF_COMMENT, "End of comment expected."},
  {ERR_IDENT_TOO_LONG, "Identifier too long."},// tên định danh quá dài
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},//không tìm thấy ký tự đóng ngoặc của hằng ký tự ' đúng phải là ''
  {ERR_INVALID_SYMBOL, "Invalid symbol."},// ký tự không hợp lệ vd %,&,@

//***** lỗi cú pháp
  {ERR_INVALID_IDENT, "An identifier expected."},//định danh khong hợp lệ VD gán hàm f:=....bên ngoài phạm vi hàm f
  {ERR_INVALID_CONSTANT, "A constant expected."},//hằng không hợp lệ vd const c2=+'a'
  {ERR_INVALID_TYPE, "A type expected."},//kiểu khai báo không hợp lệ vd var a: 121213;
  {ERR_INVALID_BASICTYPE, "A basic type expected."},// kiểu trả về của hàm hoặc tham số ko hợp lệ vd function f(vv.) : 12345;
  {ERR_INVALID_VARIABLE, "A variable expected."},// biến không hợp lệ
  {ERR_INVALID_FUNCTION, "A function identifier expected."},//hàm không hợp lệ
  {ERR_INVALID_PROCEDURE, "A procedure identifier expected."},//thủ tục không hợp lệ VD dùng call để gọi một function
  {ERR_INVALID_PARAMETER, "A parameter expected."}, //mong đợi một tham số
  {ERR_INVALID_STATEMENT, "Invalid statement."}, //statement không hợp lệ VD viết function trong begin end
  {ERR_INVALID_COMPARATOR, "A comparator expected."},// vd if n ; 2
  {ERR_INVALID_EXPRESSION, "Invalid expression."},// Ví dụ: (5 7)
  {ERR_INVALID_TERM, "Invalid term."},//Ví dụ x := x : 2; đúng phải là x*2
  {ERR_INVALID_FACTOR, "Invalid factor."},  //Ví dụ  x := x*-2;
  {ERR_INVALID_LVALUE, "Invalid lvalue in assignment."},
  {ERR_INVALID_ARGUMENTS, "Wrong arguments."},// căn bản là không thể báo lỗi do đã check lpar

//******lỗi ngữ nghĩa
  {ERR_UNDECLARED_IDENT, "Undeclared identifier."},//định danh chưa được khai báo
  {ERR_UNDECLARED_CONSTANT, "Undeclared constant."},//chưa khai báo hằng
  {ERR_UNDECLARED_INT_CONSTANT, "Undeclared integer constant."},
  {ERR_UNDECLARED_TYPE, "Undeclared type."}, //type chưa được khai báo
  {ERR_UNDECLARED_VARIABLE, "Undeclared variable."},//biến không được khai báo
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},//hàm chưa được khai báo 
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},//thủ tục chưa đựợc khai báo
  {ERR_DUPLICATE_IDENT, "Duplicate identifier."}, //định danh đã tồn tại
  {ERR_TYPE_INCONSISTENCY, "Type inconsistency"}, //không nhất quán về kiểu
  {ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, "The number of arguments and the number of parameters are inconsistent."}
};                                          // danh sách tham số và đối số không nhất quán 

void error(ErrorCode err, int lineNo, int colNo) {
  int i;
  for (i = 0 ; i < NUM_OF_ERRORS; i ++) 
    if (errors[i].errorCode == err) {
      printf("%d-%d:%s\n", lineNo, colNo, errors[i].message);
      exit(0);
    }
}

void missingToken(TokenType tokenType, int lineNo, int colNo) {
  printf("%d-%d:Missing %s\n", lineNo, colNo, tokenToString(tokenType));
  exit(0);
}

void assert(char *msg) {
  printf("%s\n", msg);
}
