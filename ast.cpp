#include <iostream>
#include <algorithm>
#include "ast.h"

std::string opSymbol(Op op){
  switch(op){
    case ADD: return "+"; break;
    case SUB: return "-"; break;
    case MUL: return "*"; break;
    case DIV: return "/"; break;
    case MOD: return "%"; break;
    case EXP: return "**"; break;
    case FDV: return "//"; break;

    case LT: return "<"; break;
    case LE: return "<="; break;
    case EQ: return "=="; break;
    case NE: return "!="; break;
    case GE: return ">="; break;
    case GT: return ">"; break;

    case KEY: return "[]"; break;

    default: return "?"; break;
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

void ElifStatement::setContext(Namespace *context){
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
  if(DEBUG){
    Block::print();
  }

  //TODO: add a return None statement at the end of the function

  if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
    interpret();
  }

  return Block::endBlock(stt);
}

Block* IfStatement::endBlock(Statement* stt){
  //the next statement is received by a parameter to check if it is an elif or else
  if(DEBUG){
    Block::print();
  }

  if(Statement::getIndent() == stt->getIndent()){
    if(ElseStatement *es = dynamic_cast<ElseStatement*>(stt)){
      // if the statement closing this block is an else on the same identation
      es->ifStt = this;
      es->setContext(getContext());
      elseStt = es;
    }
  }

  if(!elseStt){
    if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
      interpret();
    }
  }

  return Block::endBlock(stt);
}

Block* ElseStatement::endBlock(Statement* stt){
  if(DEBUG){
    Block::print();
  }

  if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
    ifStt->interpret();
  }

  return Block::endBlock(stt);
}

Block* ElifStatement::endBlock(Statement* stt){
  if(DEBUG){
    Block::print();
  }

  if(Statement::getIndent() == stt->getIndent()){
    if(ElseStatement *es = dynamic_cast<ElseStatement*>(stt)){
      // if the statement closing this block is an else on the same identation
      es->ifStt = ifStt; // passes the if that starts the chain
      es->setContext(getContext());
      elseStt = es;
    }
  }

  if(!elseStt){ //if there is no next elif/else
    if(MainBlock *mb = dynamic_cast<MainBlock*>(getParent())){
      ifStt->interpret();
    }
  }

  return Block::endBlock(stt);
}

Block* WhileStatement::endBlock(Statement *stt){
  // While endBlock has the same behavior as the If endBlock, except for elif trying to close it
  ElifStatement *elifs = dynamic_cast<ElifStatement*>(stt);
  if(Statement::getIndent() != stt->getIndent() || !elifs){
    IfStatement::endBlock(stt);
  }
}

Block* ForStatement::endBlock(Statement *stt){
  ElifStatement *elifs = dynamic_cast<ElifStatement*>(stt);
  if(Statement::getIndent() != stt->getIndent() || !elifs){
    IfStatement::endBlock(stt);
  }
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
    if(dynamic_cast<SyntaxError*>(stt))
      throw std::runtime_error("SyntaxError");

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
    if(ElseStatement *es = dynamic_cast<ElseStatement*>(stt)){
      //else statement is linked to the if statement, no need to pushed, prevents reinterpretation
    }else{
      cur_block->push(stt);
    }

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
    // while also gets this behavior
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
    if(!dynamic_cast<SyntaxError*>(stt)){
      std::cout << e.what() << std::flush;
      std::cout << std::endl;
    }
  }

  if(expect_indent)
    std::cout << "..> " << std::flush;
  else if(lastIndent() > 0)
    std::cout << "... " << std::flush;
  else
    std::cout << ">>> " << std::flush;
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
    //TODO check for return inside compound statements that are on main
  }

  stt->setContext(this);
  if(DEBUG){
    stt->print();
    std::cout << std::endl << std::flush;
  }
  stt->interpret();

  push_back(stt);
}
