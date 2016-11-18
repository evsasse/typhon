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
    if(cond.useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
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
    if(cond.useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
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
  //TODO be sure expr.exec is not being called more than expected
  std::cout << "<while " << cond.getIdentifier() << ">" << std::flush;

  if(size() > 0){
    // the body is actually interpreted only on endBlock
    while(cond.useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
      for(Statement *stt : *this){
        //TODO break; on continue;
        //TODO break;break on break;
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
      cond = expr.exec();
      // if(IntObject* io = dynamic_cast<IntObject*>(&cond)){
      //   std::cout << "{" << io->value << "}";
      //   std::cout << "{" << io->getIdentifier() << "}";
      // }
      // std::cout << "{" << cond.getIdentifier() << " "<< cond.useName("__bool__").call().getIdentifier() << "}";
    }

    if(elseStt){
      elseStt->interpret();
    }
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
  Object* ret = nullptr;
  auto argument = std::list<Object*>(1,&right);
  try {
    switch(op){
      case ADD: ret = & left.useName("__add__"); break;
      case SUB: ret = & left.useName("__sub__"); break;
      case MUL: ret = & left.useName("__mul__"); break;
      case DIV: ret = & left.useName("__div__"); break;
      case MOD: ret = & left.useName("__mod__"); break;
      case EXP: ret = & left.useName("__exp__"); break;
      case FDV: ret = & left.useName("__fdv__"); break;
      default: throw std::runtime_error("OperationError: "+opSymbol(op)+" is unexpected here"); break;
    }
  }catch(NameError& e){
    ret = nullptr;
  }
  if(ret)
    ret = & ret->call(argument);

  // If theres is no usual ("__add__") function on left implemented, or if it returns NotImplemented
  // We call the reverse version ("__radd__") on right
  NotImplemented *ni = dynamic_cast<NotImplemented*>(ret);
  if(!ret || ni){
    argument = std::list<Object*>(1,&left);
    try {
      switch(op){
        case ADD: ret = & right.useName("__radd__"); break;
        case SUB: ret = & right.useName("__rsub__"); break;
        case MUL: ret = & right.useName("__rmul__"); break;
        case DIV: ret = & right.useName("__rdiv__"); break;
        case MOD: ret = & right.useName("__rmod__"); break;
        case EXP: ret = & right.useName("__rexp__"); break;
        case FDV: ret = & right.useName("__rfdv__"); break;
        default: throw std::runtime_error("OperationError: "+opSymbol(op)+" is unexpected here"); break;
      }
    }catch(NameError& e){
      ret = nullptr;
    }
    if(ret)
      ret = & ret->call(argument);
  }

  //If it also couldnt evaluate using the reverse function
  //Throw a TypeError unsupported operand type
  ni = dynamic_cast<NotImplemented*>(ret);
  if(!ret || ni){
    throw std::runtime_error("TypeError: unsupported operand type(s) for "+opSymbol(op)+": '"+left.getIdentifier()+"' and '"+right.getIdentifier()+"'");
  }

  return *ret;
  //return left.useName("__add__").call(argument);
}

Object& UnaryOp::exec(){
  Object& right = this->right.exec();
  switch(op){
    case ADD: return right.useName("__pos__").call(); break;
    case SUB: return right.useName("__neg__").call(); break;
  }
}

Object& LitInt::exec(){
  return *(new IntObject(value));
}

Object& LitFloat::exec(){
  return *(new FloatObject(value));
}

Object& LitBool::exec(){
  return *(new BoolObject(value));
}
