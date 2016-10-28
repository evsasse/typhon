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

void MainBlock::push(Statement *stt){
  push_back(stt);
  stt->print();
  std::cout << std::endl;
  stt->interpret();
  std::cout << std::endl << std::endl;
}
