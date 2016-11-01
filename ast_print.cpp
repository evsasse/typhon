#include <iostream>
#include "ast.h"

void Statement::print(){
  if(indent >= 0)
    std::cout << "[" << indent << "]";
}

void Block::print(){
  for(auto stt : *this){
    stt->print();
    std::cout << std::endl;
  }
}

void Name::print(){
  Statement::print();
  std::cout << name;
}

void Assignment::print(){
  Statement::print();
  target.print();
  std::cout << "=";
  right.print();
}

void FunctionDef::print(){
  Statement::print();
  std::cout << name;
  body.print();
}

void BinaryOp::print(){
  Statement::print();
  std::cout << "(";
  left.print();
  std::cout << opSymbol(op);
  right.print();
  std::cout << ")";
}

void UnaryOp::print(){
  Statement::print();
  std::cout << "(";
  std::cout << opSymbol(op);
  right.print();
  std::cout << ")";
}

void LitInt::print(){
  Statement::print();
  std::cout << value;
}

void LitFloat::print(){
  Statement::print();
  std::cout << value;
}

void LitBool::print(){
  Statement::print();
  if(value)
    std::cout << "True";
  else
    std::cout << "False";
}
