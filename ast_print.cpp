#include <iostream>
#include "ast.h"

void Statement::print(){
  if(indent >= 0)
    std::cout << "[" << indent << "]" << std::flush;
}

void Block::print(){
  for(auto stt : *this){
    stt->print();
    std::cout << std::endl << std::flush;
  }
}

void Name::print(){
  Statement::print();
  std::cout << name << std::flush;
}

void Assignment::print(){
  Statement::print();
  target.print();
  std::cout << "=" << std::flush;
  right.print();
}

void FunctionDef::print(){
  Statement::print();
  std::cout << "def " << std::flush;
  name.print();
  std::cout << "[ ";
  for(auto param : parameters){
    std::cout << param->name << " ";
  }
  std::cout << "]";
}

void FunctionRet::print(){
  Statement::print();
  std::cout << "return " << std::flush;
  expr.print();
}

void CallOp::print(){
  Statement::print();
  std::cout << "(call " << std::flush;
  target.print();
  std::cout << "[ ";
  for(auto expr : arguments){
    expr->print();
    std::cout << " ";
  }
  std::cout << "])" << std::flush;
}

void IfStatement::print(){
  std::cout << "if ";
  expr.print();
}

void ElseStatement::print(){
  std::cout << "else";
}

void ElifStatement::print(){
  std::cout << "elif ";
  expr.print();
}

void BinaryOp::print(){
  Statement::print();
  std::cout << "(" << std::flush;
  left.print();
  std::cout << opSymbol(op);
  right.print();
  std::cout << ")" << std::flush;
}

void UnaryOp::print(){
  Statement::print();
  std::cout << "(" << std::flush;
  std::cout << opSymbol(op);
  right.print();
  std::cout << ")" << std::flush;
}

void LitInt::print(){
  Statement::print();
  std::cout << value << std::flush;
}

void LitFloat::print(){
  Statement::print();
  std::cout << value << std::flush;
}

void LitBool::print(){
  Statement::print();
  if(value)
    std::cout << "True" << std::flush;
  else
    std::cout << "False" << std::flush;
}
