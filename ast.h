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
