/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "token.h"

enum TypeClass
{
  TP_INT,
  TP_CHAR,
  TP_ARRAY
};

enum ObjectKind // Phân loại ký hiệu
{
  OBJ_CONSTANT,   //hằng
  OBJ_VARIABLE,   //biến 
  OBJ_TYPE,       //kiểu tự định nghĩa
  OBJ_FUNCTION,   //hàm
  OBJ_PROCEDURE,  //thủ tục
  OBJ_PARAMETER,  //tham số
  OBJ_PROGRAM     //chương trinh 
};

enum ParamKind //kiểu tham số
{
  PARAM_VALUE,    // tham trị
  PARAM_REFERENCE // tham chiếu
};

struct Type_
{
  enum TypeClass typeClass;
  int arraySize;
  struct Type_ *elementType;
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;

struct ConstantValue_
{
  enum TypeClass type;
  union
  {
    int intValue;
    char charValue;
  };
};

typedef struct ConstantValue_ ConstantValue;

struct Scope_;  // Phạm vi của một block
struct ObjectNode_;
struct Object_;

struct ConstantAttributes_ //thuộc tính hằng
{
  ConstantValue *value;
};

struct VariableAttributes_
{
  Type *type;
  struct Scope_ *scope;
};

struct TypeAttributes_
{
  Type *actualType;
};

struct ProcedureAttributes_
{
  struct ObjectNode_ *paramList;
  struct Scope_ *scope;
};

struct FunctionAttributes_
{
  struct ObjectNode_ *paramList;
  Type *returnType;
  struct Scope_ *scope;
};

struct ProgramAttributes_
{
  struct Scope_ *scope;
};

struct ParameterAttributes_
{
  enum ParamKind kind;
  Type *type;
  struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

struct Object_ // Thuộc tính của đối tượng trên bảng ký hiệu
{
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union
  {
    ConstantAttributes *constAttrs;
    VariableAttributes *varAttrs;
    TypeAttributes *typeAttrs;
    FunctionAttributes *funcAttrs;
    ProcedureAttributes *procAttrs;
    ProgramAttributes *progAttrs;
    ParameterAttributes *paramAttrs;
  };
};

typedef struct Object_ Object;

struct ObjectNode_
{
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;

struct Scope_ // Phạm vi của một block
{
  ObjectNode *objList; // Danh sách các đối tượng trong block
  Object *owner;        // Hàm, thủ tục, chương trình tương ứng block
  struct Scope_ *outer; // Phạm vi bao ngoài
};

typedef struct Scope_ Scope;

struct SymTab_ //// Bảng ký hiệu
{
  Object *program;  // Chương trình chính
  Scope *currentScope;  // Phạm vi hiện tại
  ObjectNode *globalObjectList; // Các đối tượng toàn cục như hàm WRITEI, WRITEC, WRITELN,READI, READC
};

typedef struct SymTab_ SymTab;

Type *makeIntType(void);
Type *makeCharType(void);
Type *makeArrayType(int arraySize, Type *elementType);
Type *duplicateType(Type *type);
int compareType(Type *type1, Type *type2);
void freeType(Type *type);

ConstantValue *makeIntConstant(int i);
ConstantValue *makeCharConstant(char ch);
ConstantValue *duplicateConstantValue(ConstantValue *v);

Scope *createScope(Object *owner, Scope *outer);

Object *createProgramObject(char *programName);
Object *createConstantObject(char *name);
Object *createTypeObject(char *name);
Object *createVariableObject(char *name);
Object *createFunctionObject(char *name);
Object *createProcedureObject(char *name);
Object *createParameterObject(char *name, enum ParamKind kind, Object *owner);

Object *findObject(ObjectNode *objList, char *name);

void initSymTab(void); // Khởi tạo bảng ký hiệu
void cleanSymTab(void);// Giải phóng bảng ký hiệu
void enterBlock(Scope *scope);//Mỗi khi dịch một hàm hay thủ tục, phải cập nhật giá trị của currentScope
void exitBlock(void);//Mỗi khi kết thúc duyệt một hàm hay thủ tục phải chuyển lại currentScope ra block bên ngoài
void declareObject(Object *obj);//Đăng ký một đối tượng vào block hiện tại

#endif
