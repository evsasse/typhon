#include "oo.h"
#include "ast.h"
#include <math.h>
#include <iostream>


std::string IntObject::getIdentifier(){
  return "<int "+std::to_string(value)+">";
}

IntObject::IntObject (long value) :
Object("int"), value(value) {
  // __neg__
  std::function<Object& (std::list<Object*> arguments)> __neg__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new IntObject(-(this->value)));
  };
  // __pos__
  std::function<Object& (std::list<Object*> arguments)> __pos__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new IntObject(+(this->value)));
  };

  ///////////////////
  // __add__
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value + int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __sub__
  std::function<Object& (std::list<Object*> arguments)> __sub__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value - int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value * int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __div__
  std::function<Object& (std::list<Object*> arguments)> __div__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    //TODO protect division by zero
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value / int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __mod__
  std::function<Object& (std::list<Object*> arguments)> __mod__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value % int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __exp__
  std::function<Object& (std::list<Object*> arguments)> __exp__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(pow(this->value, int_right->value)));
    }else{
      return *(new NotImplemented());
    }
  };
  // __fdv__
  std::function<Object& (std::list<Object*> arguments)> __fdv__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    //TODO protect division by zero
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value / int_right->value));
    }else{
      return *(new NotImplemented());
    }
  };

  ///////////////////
  // __bool__
  std::function<Object& (std::list<Object*> arguments)> __bool__ = [this](std::list<Object*> arguments)-> Object& {
    if(this->value == 0){
      return *(new BoolObject(0));
    }else{
      return *(new BoolObject(1));
    }
  };

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
}
