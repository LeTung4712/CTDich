/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#ifndef __PARSER_H__
#define __PARSER_H__
#include "token.h"
#include "symtab.h"

void scan(void);
void eat(TokenType tokenType);

void compileProgram(void);
void compileBlock(void);
void compileBlock2(void);
void compileBlock3(void);
void compileBlock4(void);
void compileBlock5(void);
void compileConstDecls(void);
void compileConstDecl(void);
void compileTypeDecls(void);
void compileTypeDecl(void);
void compileVarDecls(void);
void compileVarDecl(void);
void compileSubDecls(void);
void compileFuncDecl(void);
void compileProcDecl(void);
ConstantValue* compileUnsignedConstant(void);
ConstantValue* compileConstant(void);
ConstantValue* compileConstant2(void);
Type* compileType(void);
Type* compileBasicType(void); // chỉ có integer và char
void compileParams(void);// kiểm tra danh sách tham số
void compileParam(void); // khởi tạo kiểu cho tham số  (chỉ gồm các kiểu trong basicType)
void compileStatements(void);
void compileStatement(void);
Type* compileLValue(void); //dùng trong compileAssign
void compileAssignSt(void);
void compileCallSt(void);
void compileGroupSt(void);
void compileIfSt(void);
void compileElseSt(void);
void compileWhileSt(void);
void compileForSt(void);
void compileArgument(Object* param);
void compileArguments(ObjectNode* paramList);
void compileCondition(void); //kiểm tra so sánh dùng trong while, if 
Type* compileExpression(void);
Type* compileExpression2(void);
void compileExpression3(void);
Type* compileTerm(void);
void compileTerm2(void);
Type* compileFactor(void);
Type* compileIndexes(Type* arrayType);// kiểm tra chỉ số mảng

int compile(char *fileName);

#endif
