#include <iostream>
#include "ast.h"

std::string opSymbol(Op op){
  switch(op){
    case ADD: return "+";
    case SUB: return "-";
    case MUL: return "*";
    case DIV: return "/";
    case MOD: return "%";
    case EXP: return "**";
    case FDV: return "//";
    default: return "?";
  }
}

void Statement::setIndent(int i){
  indent = i;
}
int Statement::getIndent(){
  return indent;
}
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
  std::cout << "int";
}
void LitFloat::print(){
  std::cout << "float";
}
void LitBool::print(){
  std::cout << "bool";
}
