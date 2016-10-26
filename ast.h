#pragma once

#include <list>
#include <string>

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
  void setIndent(int i);
  int getIndent();
protected:
  Statement(int indent = -1) :
  indent(indent) {};
};

class Block : public std::list<Statement*> {
public:
  void print();
};

class Expression : public Statement {
};

class Name : public Expression {
public:
  void print();
  Name(std::string name) :
  name(name) {};
private:
  std::string name;
};

class Assignment : public Statement {
public:
  void print();
  Assignment(Name& target, Expression& right) :
  target(target), right(right) {};
private:
  Name& target; //TODO: make more generic,; a = 1; a[1] = 2; (a,b) = (1,2);
  Expression& right;
};

class BinaryOp : public Expression {
public:
  void print();
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
  LitInt(int value) :
  value(value) {};
private:
  int value;
};

class LitFloat: public Value {
public:
  void print();
  LitFloat(float value) :
  value(value) {};
private:
  float value;
};

class LitBool: public Value {
public:
  void print();
  LitBool(bool value) :
  value(value) {};
private:
  bool value;
};
