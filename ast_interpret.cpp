#include <iostream>
#include <stdexcept>
#include "ast.h"

void Expression::interpret(){
  std::cout << exec().getIdentifier() << std::flush;
}

void Assignment::interpret(){
  Object& expr = right.exec();
  context->newName(target.name, expr);
  std::cout << "<assignment> " << std::flush;
  std::cout << target.name << " = " << context->useName(target.name).getIdentifier();
}

void FunctionDef::interpret(){
  context->newName(name.name,*(new Function(name,*this)));
  std::cout << "<function def '" << std::flush;
  name.print();
  std::cout << "'>" << std::flush;
}

void FunctionRet::interpret(){
  std::cout << "<should return " << expr.exec().getIdentifier() << ">";
}

Object& CallOp::exec(){
  return context->useName(name.name).call(*(new Object()));
}

Object& Name::exec(){
  return context->useName(name);
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
