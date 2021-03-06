#pragma once

#include <list>
#include <string>
#include "oo.h"

extern bool DEBUG;

extern void import_file(const char *filename);

/* Addition, Subtraction, Multiplication, Division,
   Modulus, Exponent, Floor Division */
enum Op { ADD, SUB, MUL, DIV,
          MOD, EXP, FDV,
          LT, LE, EQ, NE, GE, GT,
          AND, OR, NOT,
          KEY };

std::string opSymbol(Op op);

class Statement {
private:
  int indent;
public:
  virtual void print();
  virtual Object* interpret() = 0;
  void setIndent(int i);
  int getIndent();
  bool seeIndent();
  virtual void setContext(Namespace *context);
  Namespace* getContext();
protected:
  Statement(int indent = -1) :
  indent(indent), context(nullptr) {};
  Namespace* context;
};

class Block : public std::list<Statement*>, public Object {
public:
  Block(Block* parent = nullptr, int indent = -1, std::string name = "anonymous") :
  Object(name), parent(parent), indent(indent) {};
  void print();
  Block* getParent();
  void setParent(Block* parent);
  virtual void push(Statement *stt);
  virtual Block* endBlock(Statement* stt);
  Object& interpret();
  int getIndent();
  void setIndent(int indent);
private:
  Block* parent;
  int indent;
};

class MainBlock : public Block {
public:
  MainBlock() :
  Block(nullptr, 0, "__main__") {};
  void push(Statement *stt);
};

class Program {
  // TODO: could pack this back into the parser
public:
  Program() :
  expect_indent(0), cur_block(new MainBlock()) {};
  void push(Statement *stt);
private:
  bool expect_indent;
  Block* cur_block;
  int lastIndent();
};

class Expression : public Statement {
public:
  Object* interpret();
  virtual Object& exec() = 0;
};

class Name : public Expression {
public:
  void print();
  Object& exec();
  Name(std::string name) :
  name(name) {};
  std::string name;
};

class Assignment : public Statement {
public:
  void print();
  Object* interpret();
  Assignment(Expression& target, Expression& right) :
  target(target), right(right) {};
  void setContext(Namespace *context);
private:
  Expression& target; //TODO: make more generic, a.b a.c a.b.d ...
  Expression& right;
};

class Parameter {
public:
  Parameter(std::string name) :
  name(name) {};
  std::string name;
};

class FunctionDef : public Statement, public Block {
public:
  FunctionDef(Name& name, std::list<Parameter*>& parameters) :
  Block(nullptr), name(name), parameters(parameters) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
  std::list<Parameter*>& parameters;
private:
  Name& name;
};

class FunctionRet : public Statement {
public:
  void print();
  Object* interpret();
  void setContext(Namespace *context);
  FunctionRet(Expression& expr) :
  expr(expr) {};
private:
  Expression& expr;
};

class CallOp : public Expression {
public:
  void print();
  Object& exec();
  void setContext(Namespace *context);
  CallOp(Expression& target, std::list<Expression*>& arguments) :
  target(target), arguments(arguments) {};
private:
  Expression& target;
  std::list<Expression*>& arguments;
};

class Compound :  public Block {
// mantains namespace of block body as the parent
public:
  void push(Statement *stt);
};

class ElseStatement;

class IfStatement : public Statement, public Compound {
public:
  IfStatement(Expression& expr) :
  expr(expr), elseStt(nullptr) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
  void setContext(Namespace *context);
  Expression& expr;
  ElseStatement* elseStt;
};

class ElseStatement : public Statement, public Compound {
public:
  ElseStatement() :
  ifStt(nullptr) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
  IfStatement* ifStt;
};

class ElifStatement : public ElseStatement{
public:
  ElifStatement(Expression& expr) :
  expr(expr) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
  void setContext(Namespace *context);
  Expression &expr;
  ElseStatement* elseStt;
};

class WhileStatement : public IfStatement{
public:
  WhileStatement(Expression& expr) :
  IfStatement(expr) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
};

class ForStatement : public WhileStatement{
public:
  ForStatement(std::string name, Expression& expr) :
  WhileStatement(expr), name(name) {};
  void print();
  Object* interpret();
  Block* endBlock(Statement* stt);
  std::string name;
};

class PassStatement : public Statement {
public:
  Object* interpret();
};

class SyntaxError : public PassStatement {
};

class ImportStatement : public Statement {
public:
  ImportStatement(std::string filename) :
  filename(filename) {};
  Object* interpret();
  std::string filename;
};

class BinaryOp : public Expression {
public:
  void print();
  Object& exec();
  BinaryOp(Expression& left, Op op, Expression& right) :
  left(left), op(op), right(right) {};
  void setContext(Namespace *context);
  Expression& left;
  Expression& right;
  Op op;
};

class UnaryOp : public Expression {
public:
  void print();
  Object& exec();
  UnaryOp(Op op, Expression& right) :
  op(op), right(right) {};
  void setContext(Namespace *context);
  Expression& right;
  Op op;
};

class Value : public Expression {
};

class LitList: public Expression {
public:
  void print();
  Object& exec();
  LitList(std::list<Expression*> exprs):
  exprs(exprs) {};
private:
  std::list<Expression*> exprs;
};

//TODO: ? change into one number with arbitrary precision, using gmp lib
class LitInt: public Value {
public:
  void print();
  Object& exec();
  LitInt(int value) :
  value(value) {};
private:
  int value;
};

class LitFloat: public Value {
public:
  void print();
  Object& exec();
  LitFloat(float value) :
  value(value) {};
private:
  float value;
};

class LitBool: public Value {
public:
  void print();
  Object& exec();
  LitBool(bool value) :
  value(value) {};
private:
  bool value;
};

class LitString: public Value {
public:
  void print();
  Object& exec();
  LitString(std::string value) :
  value(value) {};
private:
  std::string value;
};
