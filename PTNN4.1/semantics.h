/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include "symtab.h"

void checkFreshIdent(char *name);
Object* checkDeclaredIdent(char *name);
Object* checkDeclaredConstant(char *name);
Object* checkDeclaredType(char *name);
Object* checkDeclaredVariable(char *name);
Object* checkDeclaredFunction(char *name);
Object* checkDeclaredProcedure(char *name);
Object* checkDeclaredLValueIdent(char *name);// kiểm tra xem định danh đang xet thuộc loại biến ,tham số hay hàm 

void checkIntType(Type* type);
void checkCharType(Type* type);
void checkArrayType(Type* type);
void checkBasicType(Type* type);
void checkTypeEquality(Type* type1, Type* type2); //kiểm tra sự nhất quán về kiểu của 2 vế trong lệnh asign,condition và agrument

#endif
