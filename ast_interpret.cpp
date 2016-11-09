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
  // the function is now added to the namespace only at the endBlock
  std::cout << "<function def '" << std::flush;
  name.print();
  std::cout << "'>" << std::flush;
  return nullptr;
}

Object* FunctionRet::interpret(){
  Object& ret = expr.exec();
  std::cout << "<return " << ret.getIdentifier() << ">" << std::flush;
  return &ret;
}

Object* IfStatement::interpret(){
  // the body can be interpreted only on endBlock
  Object& cond = expr.exec();

  std::cout << "<if " << cond.getIdentifier() << " is truthy>";
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
  return *(new Object());
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
