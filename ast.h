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
  virtual void interpret() = 0;
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
  void interpret();
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
  void interpret();
  Assignment(Name& target, Expression& right) :
  target(target), right(right) {};
  void setContext(Namespace *context);
private:
  Name& target; //TODO: make more generic,; a = 1; a[1] = 2; (a,b) = (1,2);
  Expression& right;
};

class FunctionDef : public Statement, public Block {
public:
  void print();
  void interpret();
  FunctionDef(Name& name) :
  Block(nullptr), name(name) {};
private:
  Name& name;
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
