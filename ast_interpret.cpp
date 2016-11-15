#include <iostream>
#include <stdexcept>
#include "ast.h"

Object* Expression::interpret(){
  std::cout << exec().getIdentifier() << std::flush;
  return nullptr;
}

Object* Assignment::interpret(){
  Object& expr = right.exec();
  context->newName(target.name, expr);
  std::cout << "<assignment> " << std::flush;
  std::cout << target.name << " = " << context->useName(target.name).getIdentifier() << std::flush;
  return nullptr;
}

Object* FunctionDef::interpret(){
  //context->newName(name.name,*(new Function(name,*this)));
  std::cout << "<function def '" << std::flush;
  name.print();
  std::cout << "'>" << std::flush;

  //The function is added to the namespace when it has a body
  // the interpret is called at endBlock, and by parent bodys
  if(size() > 0)
    context->newName(name.name,*(new Function(name,parameters,*this)));

  return nullptr;
}

Object* FunctionRet::interpret(){
  Object& ret = expr.exec();
  std::cout << "<return " << ret.getIdentifier() << ">" << std::flush;
  return &ret;
}

Object* IfStatement::interpret(){
  Object& cond = expr.exec();

  std::cout << "<if " << cond.getIdentifier() << ">" << std::flush;

  if(size() > 0){
    // the body is actually interpreted only on endBlock
    //TODO change check for a BoolObject
    if(cond.useName("__bool__").call().getIdentifier() == IntObject(1).getIdentifier()){
      for(Statement *stt : *this){
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
    }else if(elseStt){
      elseStt->interpret();
    }
  }

  return nullptr;
}

Object* ElseStatement::interpret(){
  std::cout << "<else>";

  if(size() > 0){
    for(Statement *stt : *this){
      Object* ret = stt->interpret();
      if(ret) return ret;
    }
  }
}

Object* ElifStatement::interpret(){
  Object& cond = expr.exec();

  std::cout << "<elif " << cond.getIdentifier() << ">" << std::flush;

  if(size() > 0){
    // the body is actually interpreted only on endBlock
    //TODO change check for a BoolObject
    if(cond.useName("__bool__").call().getIdentifier() == IntObject(1).getIdentifier()){
      for(Statement *stt : *this){
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
    }else if(elseStt){
      elseStt->interpret();
    }
  }

  return nullptr;
}

Object* WhileStatement::interpret(){
  Object& cond = expr.exec();
  //TODO change print, as a function on the Expression
  //may end up being called one more time than expected
  std::cout << "<while " << cond.getIdentifier() << ">" << std::flush;

  if(size() > 0){
    // the body is actually interpreted only on endBlock
    //TODO change check for a BoolObject
    while(cond.useName("__bool__").call().getIdentifier() == IntObject(1).getIdentifier()){
      for(Statement *stt : *this){
        //TODO break; on continue;
        //TODO break;break on break;
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
      cond = expr.exec();
    }
    // TODO support for-else
    // if(elseStt){
    //   elseStt->interpret();
    // }
  }

  return nullptr;
}

Object& CallOp::exec(){
  //return context->useName(name.name).call(*(new Object()));
  std::list<Object*> _arguments;
  for(auto arg : arguments){
    _arguments.push_back(&(arg->exec()));
  }
  return target.exec().call(_arguments);
}

Object& Name::exec(){
  // segmentation faults here mean that the context is not being properly set
  // review the setContext function of new statement types
  return context->useName(name);
}

Object& BinaryOp::exec(){
  Object& left = this->left.exec();
  Object& right = this->right.exec();
  auto argument = std::list<Object*>(1,&right);
  return left.useName("__add__").call(argument);
}

Object& UnaryOp::exec(){
  Object& right = this->right.exec();
  return right.useName("__neg__").call();
}

Object& LitInt::exec(){
  return *(new IntObject(value));
}

Object& LitFloat::exec(){
  return *(new Object());
}

Object& LitBool::exec(){
  return *(new Object());
}
