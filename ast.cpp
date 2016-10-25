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
  }
}

void Block::print(){
  for(auto stt : *this){
    stt.print();
    std::cout << std::endl;
  }
}

void BinaryOp::print(){
  left.print();
  std::cout << opSymbol(op);
  right.print();
}

void Value::print(){
  std::cout << "value";
}
