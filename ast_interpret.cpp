#include <iostream>
#include "ast.h"

void Name::interpret(){
  std::cout << name;
}

void Assignment::interpret(){
  target.interpret();
  std::cout << "=";
  right.interpret();
}

void BinaryOp::interpret(){
  std::cout << "(";
  left.interpret();
  std::cout << opSymbol(op);
  right.interpret();
  std::cout << ")";
}

void UnaryOp::interpret(){
  std::cout << "(";
  std::cout << opSymbol(op);
  right.interpret();
  std::cout << ")";
}

void LitInt::interpret(){
  std::cout << value;
}

void LitFloat::interpret(){
  std::cout << value;
}

void LitBool::interpret(){
  if(value)
    std::cout << "True";
  else
    std::cout << "False";
}
