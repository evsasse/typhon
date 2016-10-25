#pragma once

#include <list>

/* Addition, Subtraction, Multiplication, Division,
   Modulus, Exponent, Floor Division */
enum Op { ADD, SUB, MUL, DIV,
          MOD, EXP, FDV };

std::string opSymbol(Op op);

class Block : public std::list<Statement> {
public:
  void print();
};

class Statement {
public:
  virtual void print();
protected:
  Statement();
};

Expression : public Statement {
public:
  virtual void print();
protected:
  Expression();
};

class BinaryOp : public Expression {
public:
  void print();
  BinaryOp(Expression left, Op op, Expression right)
private:
  Expression left;
  Expression right;
  Op op;
};

class Value : public Expression {
public:
  void print();
}
