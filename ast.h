#pragma once

#include <list>
#include <string>
#include "oo.h"

/* Addition, Subtraction, Multiplication, Division,
   Modulus, Exponent, Floor Division */
enum Op { ADD, SUB, MUL, DIV,
          MOD, EXP, FDV };

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
protected:
  Statement(int indent = -1) :
  indent(indent), context(nullptr) {};
  Namespace* context;
};

class Block : public std::list<Statement*>, public Object {
public:
  Block(Block* parent, int indent = -1, std::string name = "anonymous") :
  Object(name), parent(parent), indent(indent) {};
  void print();
  Block* getParent();
  void setParent(Block* parent);
  virtual void push(Statement *stt);
  virtual Block* endBlock();
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
  Assignment(Name& target, Expression& right) :
  target(target), right(right) {};
  void setContext(Namespace *context);
private:
  Name& target; //TODO: make more generic, a.b a.c a.b.d ...
  Expression& right;
};

class FunctionDef : public Statement, public Block {
public:
  void print();
  Object* interpret();
  FunctionDef(Name& name) :
  Block(nullptr), name(name) {};
  Block* endBlock();
private:
  Name& name;
};

class FunctionRet : public Statement {
public:
  void print();
  Object* interpret();
  FunctionRet(Expression& expr) :
  expr(expr) {};
private:
  Expression& expr;
};

class CallOp : public Expression {
public:
  void print();
  Object& exec();
  CallOp(Name& name) :
  name(name) {};
private:
  Name& name;
};

class BinaryOp : public Expression {
public:
  void print();
  Object& exec();
  BinaryOp(Expression& left, Op op, Expression& right) :
  left(left), op(op), right(right) {};
  void setContext(Namespace *context);
private:
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
private:
  Expression& right;
  Op op;
};

class Value : public Expression {
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
