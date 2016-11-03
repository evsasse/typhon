#include <iostream>
#include <algorithm>
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
bool Statement::seeIndent(){
  return indent >= 0;
}
void Statement::setContext(Namespace *context){
  this->context = context;
}
void Assignment::setContext(Namespace *context){
  this->context = context;
  right.setContext(context);
}
void BinaryOp::setContext(Namespace *context){
  //this->context = context;
  left.setContext(context);
  right.setContext(context);
}
void UnaryOp::setContext(Namespace *context){
  //this->context = context;
  right.setContext(context);
}

Block* Block::getParent(){
  return parent;
}

void Block::setParent(Block* parent){
  this->parent = parent;
}

void Block::push(Statement *stt){
  std::cout << "dumb";
}

Block* Block::endBlock(){
  return parent;
}

int Block::getIndent(){
  return indent;
}

void Block::setIndent(int indent){
  this->indent = indent;
}

int Program::lastIndent(){
  Block* cur = cur_block;
  while(cur && cur->getIndent() == -1)
    cur = cur->getParent();
  if(cur) return cur->getIndent();
  return 0;
}

void Program::push(Statement *stt){
  try {
    int diff = stt->getIndent() - lastIndent();
    if(expect_indent && diff>0){
      // starts a expected new block
      std::cout << "starts a expected new block" << std::endl;
      std::cout << "[" << stt->getIndent() << "]" << std::endl;
      std::cout << "[" << lastIndent() << "]" << std::endl;
      expect_indent = 0;
      cur_block->setIndent(stt->getIndent());
    }else if(!expect_indent && diff<0){
      // trying to end a block
      std::cout << "trying to end a block" << std::endl;
      while(cur_block->getIndent() > stt->getIndent())
        cur_block = cur_block->endBlock();
      if(cur_block->getIndent() != stt->getIndent()){
        throw std::runtime_error("IndentationError: unindent does not match any outer indentation level");
      }
    }else if(!expect_indent && diff>0){
      // more indents than expected
      std::cout << "more indents than expected" << std::endl;
      throw std::runtime_error("IndentationError: unexpected indent");
    }else if(expect_indent && diff<=0){
      // less indents than expected
      std::cout << "less indents than expected" << std::endl;
      throw std::runtime_error("IndentationError: expected an indented block");
    }
    // !expect_indent && diff==0
    // continue current block
    std::cout << "continue current block" << std::endl;
    cur_block->push(stt);

    if(FunctionDef* fd = dynamic_cast<FunctionDef*>(stt)){
      fd->setParent(cur_block);
      cur_block = fd;
      expect_indent = 1;
    }

  } catch (std::runtime_error& e) {
    // an error brings the interpreter back to the main context
    expect_indent = 0;
    while(cur_block->getParent())
      cur_block = cur_block->endBlock();
    std::cout << e.what();
  }
  std::cout << std::endl << ">>> ";
}

void MainBlock::push(Statement *stt){
  newName("_bnn", *new IntObject(666));
  push_back(stt);
  stt->setContext(this);
  stt->print();
  std::cout << std::endl;
  stt->interpret();
}
