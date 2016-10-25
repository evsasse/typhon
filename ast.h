#pragma once

#include <list>
#include <string>

/* Addition, Subtraction, Multiplication, Division,
   Modulus, Exponent, Floor Division */
enum Op { ADD, SUB, MUL, DIV,
          MOD, EXP, FDV };

std::string opSymbol(Op op);

class Statement {
public:
  virtual void print() = 0;
};

class Block : public std::list<Statement*> {
public:
  void print();
};

class Expression : public Statement {
public:
  virtual void print() = 0;
};

class BinaryOp : public Expression {
public:
  void print();
  BinaryOp(Expression& left, Op op, Expression& right) :
  left(left), right(right), op(op) {};
private:
  Expression& left;
  Expression& right;
  Op op;
};

class Value : public Expression {
public:
  virtual void print() = 0;
};

class LitInt: public Value {
public:
  void print();
};

class LitFloat: public Value {
public:
  void print();
};

class LitBool: public Value {
public:
  void print();
};
