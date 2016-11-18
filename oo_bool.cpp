#include "oo.h"
#include "ast.h"
#include <math.h>
#include <iostream>


std::string BoolObject::getIdentifier(){
  if(value) return "<bool True>";
  return "<bool False>";
}

int BoolObject::to_int(){
  if(value) return 1;
  return 0;
}

BoolObject::BoolObject (bool value) :
Object("bool"), value(value) {
  // __neg__
  std::function<Object& (std::list<Object*> arguments)> __neg__ = [this](std::list<Object*> arguments)-> Object& {
    return IntObject(to_int()).useName("__neg__").call(arguments);
  };
  // __pos__
  std::function<Object& (std::list<Object*> arguments)> __pos__ = [this](std::list<Object*> arguments)-> Object& {
    return IntObject(to_int()).useName("__pos__").call(arguments);
  };

  ///////////////////
  // __add__
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__add__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __sub__
  std::function<Object& (std::list<Object*> arguments)> __sub__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__sub__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__mul__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __div__
  std::function<Object& (std::list<Object*> arguments)> __div__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__div__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __mod__
  std::function<Object& (std::list<Object*> arguments)> __mod__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__mod__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __exp__
  std::function<Object& (std::list<Object*> arguments)> __exp__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__exp__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };
  // __fdv__
  std::function<Object& (std::list<Object*> arguments)> __fdv__ = [this](std::list<Object*> arguments)-> Object& {
    if(BoolObject* bo = dynamic_cast<BoolObject*>(arguments.front())){
      arguments = std::list<Object*>(1, new IntObject(bo->to_int()));
      return IntObject(to_int()).useName("__fdv__").call(arguments);
    }else{
      return *(new NotImplemented());
    }
  };

  ///////////////////
  // __bool__
  std::function<Object& (std::list<Object*> arguments)> __bool__ = [this](std::list<Object*> arguments)-> Object& {
    *(new BoolObject(this->value));
  };

  //Some functions use the IntObject behavior, and consider True as 1 and False as 0
  newName("__neg__", *(new BuiltInFunction(__neg__)));
  newName("__pos__", *(new BuiltInFunction(__pos__)));

  newName("__add__", *(new BuiltInFunction(__add__)));
  newName("__sub__", *(new BuiltInFunction(__sub__)));
  newName("__mul__", *(new BuiltInFunction(__mul__)));
  newName("__div__", *(new BuiltInFunction(__div__)));
  newName("__mod__", *(new BuiltInFunction(__mod__)));
  newName("__exp__", *(new BuiltInFunction(__exp__)));
  newName("__fdv__", *(new BuiltInFunction(__fdv__)));

  newName("__bool__", *(new BuiltInFunction(__bool__)));
  //newName("__int__", *(new BuiltInFunction(__int__)));
}