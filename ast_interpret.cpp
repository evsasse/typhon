#include <iostream>
#include <stdexcept>
#include "ast.h"

Object* Expression::interpret(){
  std::cout << exec().getIdentifier() << std::endl;
  return nullptr;
}

Object* Assignment::interpret(){
  //TODO attribution of non built in types should be by reference, not copy ??

  if(Name* name = dynamic_cast<Name*>(&target)){
    Object& expr = right.exec();
    context->newName(name->name, expr);
    if(DEBUG){
      std::cout << "<assignment> " << std::flush;
      std::cout << name->name << " = " << context->useName(name->name).getIdentifier() << std::flush;
    }
  }else
  if(dynamic_cast<LitBool*>(&target)
  || dynamic_cast<LitInt*>(&target)
  || dynamic_cast<LitFloat*>(&target)
  || dynamic_cast<LitList*>(&target)){
    throw std::runtime_error("SyntaxError: can't assign to literal");
  }else
  if(BinaryOp* bo = dynamic_cast<BinaryOp*>(&target)){
    if(bo->op == Op::KEY){
      auto left = bo->left.exec();
      try{
        left.useName("__setitem__");
      }catch(NameError& e){
        throw std::runtime_error("TypeError: '"+left.getIdentifier()+"' object does not support item assignment");
      }
      auto params = std::list<Object*>();
      params.push_back(&(bo->right.exec())); // index
      params.push_back(&(right.exec())); // new value
      Object &ret = left.useName("__setitem__").call(params);
      if(dynamic_cast<IndexError*>(&ret)){
        throw std::runtime_error("IndexError: list assignment index out of range");
      }
    }else{
      throw std::runtime_error("SyntaxError: can't assign to operator");
    }
  }else
  if(dynamic_cast<UnaryOp*>(&target)){
    throw std::runtime_error("SyntaxError: can't assign to operator");
  }else{
    throw std::runtime_error("SyntaxError: This kind of assignment was unexpected");
  }

  return nullptr;
}

Object* FunctionDef::interpret(){
  //context->newName(name.name,*(new Function(name,*this)));

  if(DEBUG){
    std::cout << "<function def '" << std::flush;
    name.print();
    std::cout << "'>" << std::flush;
  }

  //The function is added to the namespace when it has a body
  // the interpret is called at endBlock, and by parent bodys
  if(size() > 0)
    context->newName(name.name,*(new Function(name,parameters,*this)));

  return nullptr;
}

Object* FunctionRet::interpret(){
  Object& ret = expr.exec();
  if(DEBUG){
    std::cout << "<return " << ret.getIdentifier() << ">" << std::flush;
  }
  return &ret;
}

Object* IfStatement::interpret(){
  if(size() > 0){
    // the body is actually interpreted only on endBlock
    Object& cond = expr.exec();
    if(DEBUG){
      std::cout << "<if "<< cond.getIdentifier() << ">" << std::flush;
    }

    if(cond.useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
      for(Statement *stt : *this){
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
    }else if(elseStt){
      Object* ret = elseStt->interpret();
      if(ret) return ret;
    }
  }

  return nullptr;
}

Object* ElseStatement::interpret(){
  if(DEBUG){
    std::cout << "[else]";
  }

  if(size() > 0){
    for(Statement *stt : *this){
      Object* ret = stt->interpret();
      if(ret) return ret;
    }
  }

  return nullptr;
}

Object* ElifStatement::interpret(){
  if(size() > 0){
    // the body is actually interpreted only on endBlock
    Object& cond = expr.exec();
    if(DEBUG){
      std::cout << "<elif "<< cond.getIdentifier() << ">" << std::flush;
    }

    if(cond.useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
      for(Statement *stt : *this){
        Object* ret = stt->interpret();
        if(ret) return ret;
      }
    }else if(elseStt){
      Object* ret = elseStt->interpret();
      if(ret) return ret;
    }
  }

  return nullptr;
}

Object* WhileStatement::interpret(){
  if(size() > 0){
    // the body is actually interpreted only on endBlock
    Object* cond = &(expr.exec());
    if(DEBUG){
      print();
      std::cout << "<while " << cond->getIdentifier() << ">" << std::flush;
    }

    while(cond->useName("__bool__").call().getIdentifier() == BoolObject(1).getIdentifier()){
      for(Statement *stt : *this){
        //TODO break; on continue;
        //TODO break;break on break;
        Object* ret = stt->interpret();
        if(ret) return ret;
      }

      cond = &(expr.exec());

      if(DEBUG){
        print();
        std::cout << "<while " << cond->getIdentifier() << ">" << std::flush;
      }
    }

    //TODO elseStt && !break
    if(elseStt){
      Object* ret = elseStt->interpret();
      if(ret) return ret;
    }
  }

  return nullptr;
}

Object* ForStatement::interpret() {
  if(size() > 0){
    // the body is actually interpreted only on endBlock

    //make sure the expression given results in an iterable
    Object& iterable = expr.exec();
    try{
      iterable.useName("__getitem__");
    }catch(NameError& e){
      throw std::runtime_error("TypeError: '"+iterable.getIdentifier()+"' object is not iterable");
    }

    //gets the item at the first position of the iterable
    //TODO make it generic creating a iterator and calling next()
    auto cur_index = new IntObject(0);
    auto param = std::list<Object*>(1,cur_index);
    Object* item = &(iterable.useName("__getitem__").call(param));

    if(DEBUG){
      print();
    }
    //if trying to get the position results in a index error, it ended iterating
    while(!(dynamic_cast<IndexError*>(item))){
      if(DEBUG){
        std::cout << "<for " << item->getIdentifier() << ">" << std::flush;
      }
      //puts the value of the item on the namespace using the name given
      this->context->newName(name, *item);

      //executes each statement
      for(Statement *stt : *this){
        //TODO break; on continue;
        //TODO break;break on break;
        Object* ret = stt->interpret();
        //if an statement returns a value, it means it was a return statement
        if(ret) return ret;
      }

      //gets the next item
      //TODO make it generic calling next()
      cur_index = new IntObject(cur_index->value + 1);
      param = std::list<Object*>(1,cur_index);
      item = &(iterable.useName("__getitem__").call(param));
    }

    //TODO elseStt && !break
    if(elseStt){
      Object* ret = elseStt->interpret();
      if(ret) return ret;
    }
  }

  return nullptr;
}

Object* PassStatement::interpret(){
  return nullptr;
}

Object* ImportStatement::interpret(){
  import_file((filename+".ty").c_str());

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
  // review the setContext function of the last modified statement types
  if(DEBUG && !context){
    std::cout << "[UNSET CONTEXT WHEN LOOKING FOR NAME: '" << name << "']";
  }
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

      case LT: ret = & left.useName("__lt__"); break; // l<r;
      case LE: ret = & left.useName("__le__"); break; // l<=r;
      case EQ: ret = & left.useName("__eq__"); break; // l==r;
      case NE: ret = & left.useName("__ne__"); break; // l!=r;
      case GE: ret = & left.useName("__ge__"); break; // l>=r;
      case GT: ret = & left.useName("__gt__"); break; // l>r;

      case AND: ret = & left.useName("__and__"); break;
      case OR: ret = & left.useName("__or__"); break;

      case KEY: ret = & left.useName("__getitem__"); break;

      default: throw std::runtime_error("OperationError: "+opSymbol(op)+" is unexpected here"); break;
    }
  }catch(NameError& e){
    if(op == KEY)
      throw std::runtime_error("TypeError: "+left.getIdentifier()+" object is not subscriptable");
    ret = nullptr;
  }
  if(ret)
    ret = & ret->call(argument);

  // If theres is no usual ("__add__") function on left implemented,
  // or if it returns NotImplemented
  // We call the reverse version ("__radd__") on right

  // If there is no usual ("__eq__", "__ge__") comparison on left implemented,
  // or if it returns NotImplemented
  // We call an equivalent function on right (a==b; b==a;; x>=y; y<=x)
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

        case LT: ret = & right.useName("__gt__"); break; // l<r; r>l;
        case LE: ret = & right.useName("__ge__"); break; // l<=r; r>=l;
        case EQ: ret = & right.useName("__eq__"); break; // l==r; r==l;
        case NE: ret = & right.useName("__ne__"); break; // l!=r; r!=l;
        case GE: ret = & right.useName("__le__"); break; // l>=r; r<=l;
        case GT: ret = & right.useName("__lt__"); break; // l>r; r<l;

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
  }else if(dynamic_cast<IndexError*>(ret)){
    throw std::runtime_error("IndexError: list index out of range");
  }

  return *ret;
  //return left.useName("__add__").call(argument);
}

Object& UnaryOp::exec(){
  Object& right = this->right.exec();
  try{
    switch(op){
      case ADD: return right.useName("__pos__").call(); break;
      case SUB: return right.useName("__neg__").call(); break;
      case NOT: return right.useName("__not__").call(); break;
    }
  }catch(NameError& e){
    throw std::runtime_error("TypeError: bad operand type for unary "+opSymbol(op)+": '"+right.getIdentifier()+"'");
  }
}

Object& LitList::exec(){
  auto list = new std::list<Object*>();

  for(auto expr : exprs){
    list->push_back(&(expr->exec()));
  }

  return *(new ListObject(*list));
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

Object& LitString::exec(){
  return *(new StringObject(value));
}
