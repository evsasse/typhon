#include <iostream>
#include "ast.h"

void Expression::interpret(){
  std::cout << exec().identifier;
}

void Assignment::interpret(){
  std::cout << "an assignment";
}

Object Name::exec(){
  return Object();
}

Object BinaryOp::exec(){
  return Object();
}

Object UnaryOp::exec(){
  return Object();
}

Object LitInt::exec(){
  return Object();
}

Object LitFloat::exec(){
  return Object();
}

Object LitBool::exec(){
  return Object();
}
