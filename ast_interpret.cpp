#include <iostream>
#include "ast.h"

void Expression::interpret(){
  std::cout << exec().getIdentifier();
}

void Assignment::interpret(){
  target.print();
  std::cout << "=";
  right.interpret();
}

void FunctionDef::interpret(){
  std::cout << "<function def '"+name+"'>";
}

Object& Name::exec(){
  return *(new Object());
}

Object& BinaryOp::exec(){
  Object& left = this->left.exec();
  Object& right = this->right.exec();
  return left.useName("__add__").call(right);
}

Object& UnaryOp::exec(){
  return *(new Object());
}

Object& LitInt::exec(){
  return *(new IntObject(value));
}

Object& LitFloat::exec(){
  return *(new Object());
}

Object& LitBool::exec(){
  return *(new Object());
}
