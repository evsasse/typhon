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
Namespace* Statement::getContext(){
  return context;
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

void IfStatement::setContext(Namespace *context){
  this->context = context;
  expr.setContext(context);
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

Block* Block::endBlock(Statement* stt){
  return parent;
}

Block* FunctionDef::endBlock(Statement* stt){
  Block::print();
  std::cout << "endBlock " << name.name << std::endl << std::flush;
  //TODO: add a return None statement at the end of the function
  context->newName(name.name,*(new Function(name,parameters,*this)));
  return Block::endBlock(stt);
}

Block* IfStatement::endBlock(Statement* stt){
  //TODO the next statement is received by a parameter to check if it is an elif or else

  Block::print();

  if(Statement::getIndent() == stt->getIndent()){
    if(ElseStatement *es = dynamic_cast<ElseStatement*>(stt)){
      // if the statement closing this block is an else on the same identation
      es->ifStt = this;
      elseStt = es;
    }
  }
  //TODO interpret body only if there is no elif/else. ? DONE ?
  //TODO interpret would be called by else or elif endBlock
  if(!elseStt){
    if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
      interpret();
    }
  }

  return Block::endBlock(stt);
}

Block* ElseStatement::endBlock(Statement* stt){
  Block::print();

  ifStt->interpret();

  // if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
  //   interpret();
  // }

  return Block::endBlock(stt);
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
        cur_block = cur_block->endBlock(stt);
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
    cur_block->push(stt);

    if(FunctionDef* fd = dynamic_cast<FunctionDef*>(stt)){
      fd->setParent(cur_block);
      cur_block = fd;
      expect_indent = 1;
    } else
    // check for else first, so elif will also get this behavior
    if(ElseStatement *es = dynamic_cast<ElseStatement*>(stt)){
      if(!(es->ifStt)) // ifStt should have been filled by if's endBlock
        throw std::runtime_error("SyntaxError: invalid syntax, else statement without previous if");
      es->setParent(es->ifStt->getParent());
      cur_block = es;
      expect_indent = 1;
    } else
    if(IfStatement *is = dynamic_cast<IfStatement*>(stt)){
      is->setParent(cur_block);
      cur_block = is;
      expect_indent = 1;
    }

  } catch (std::runtime_error& e) {
    // an error brings the interpreter back to the main context
    //TODO fix that an error brought by an endBlock discards the current
    //statement that ended the block
    expect_indent = 0;
    while(cur_block->getParent())
      //cur_block = cur_block->endBlock();
      cur_block = cur_block->getParent();
      //TODO properly destroy wronged statement
    std::cout << e.what() << std::flush;
  }

  if(expect_indent)
    std::cout << std::endl << "..> " << std::flush;
  else if(lastIndent() > 0)
    std::cout << std::endl << "... " << std::flush;
  else
    std::cout << std::endl << ">>> " << std::flush;
}

void Block::push(Statement *stt){
  stt->setContext(this);

  push_back(stt);
}

void Compound::push(Statement *stt){
  // set the context to the last proper namespace
  if(MainBlock* mb = dynamic_cast<MainBlock*>(getParent())){
    stt->setContext(getParent());
  }else
  if(FunctionDef* fd = dynamic_cast<FunctionDef*>(getParent())){
    stt->setContext(getParent());
  }else{
    Statement* ps = dynamic_cast<Statement*>(getParent());
    stt->setContext(ps->getContext());
  }

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
