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
void FunctionRet::setContext(Namespace *context){
  expr.setContext(context);
}
void CallOp::setContext(Namespace* context){
  target.setContext(context);
  for(auto arg : arguments) {
    arg->setContext(context);
  }
}

Block* Block::getParent(){
  return parent;
}
void Block::setParent(Block* parent){
  this->parent = parent;
}

int Block::getIndent(){
  return indent;
}
void Block::setIndent(int indent){
  this->indent = indent;
}

Block* Block::endBlock(){
  return parent;
}

Block* FunctionDef::endBlock(){
  Block::print();
  std::cout << "endBlock " << name.name << std::endl << std::flush;
  //TODO: add a return None statement at the end of the function
  context->newName(name.name,*(new Function(name,parameters,*this)));
  return Block::endBlock();
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
      expect_indent = 0;
      cur_block->setIndent(stt->getIndent());
    }else if(!expect_indent && diff<0){
      // trying to end a block
      while(cur_block->getIndent() > stt->getIndent())
        cur_block = cur_block->endBlock();
      if(cur_block->getIndent() != stt->getIndent()){
        throw std::runtime_error("IndentationError: unindent does not match any outer indentation level");
      }
    }else if(!expect_indent && diff>0){
      // more indents than expected
      throw std::runtime_error("IndentationError: unexpected indent");
    }else if(expect_indent && diff<=0){
      // less indents than expected
      throw std::runtime_error("IndentationError: expected an indented block");
    }
    // !expect_indent && diff==0
    // continue current block
    std::cout << "continue current block" << std::endl << std::flush;
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
      //cur_block = cur_block->endBlock();
      cur_block = cur_block->getParent();
      //TODO properly destroy wronged statement
    std::cout << e.what() << std::flush;
  }
  std::cout << std::endl << ">>> " << std::flush;
}

void Block::push(Statement *stt){
  stt->setContext(this);

  push_back(stt);
}

void MainBlock::push(Statement *stt){
  if(FunctionRet* fr = dynamic_cast<FunctionRet*>(stt)){
    throw std::runtime_error("SyntaxError: 'return' outside function");
  }

  stt->setContext(this);
  stt->print();
  std::cout << std::endl << std::flush;
  stt->interpret();

  push_back(stt);
}
