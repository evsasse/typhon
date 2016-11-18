#include "oo.h"
#include "ast.h"
#include <math.h>
#include <iostream>

std::string FloatObject::getIdentifier(){
  //TODO format value
  return "<float "+std::to_string(value)+">";
}

FloatObject::FloatObject (double value) :
Object("float"), value(value) {
  // __neg__
  std::function<Object& (std::list<Object*> arguments)> __neg__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new FloatObject(-(this->value)));
  };
  // __pos__
  std::function<Object& (std::list<Object*> arguments)> __pos__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new FloatObject(+(this->value)));
  };

  ///////////////////
  // __add__
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(this->value + float_right->value));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(this->value + int_right->value));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(this->value + bool_right->to_int()));
    }else{
      return *(new NotImplemented());
    }
  };
  // __sub__
  std::function<Object& (std::list<Object*> arguments)> __sub__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(this->value - float_right->value));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(this->value - int_right->value));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(this->value - bool_right->to_int()));
    }else{
      return *(new NotImplemented());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(this->value * float_right->value));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(this->value * int_right->value));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(this->value * bool_right->to_int()));
    }else{
      return *(new NotImplemented());
    }
  };
  // __div__
  std::function<Object& (std::list<Object*> arguments)> __div__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      if(float_right->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(this->value / float_right->value));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      if(int_right->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(this->value / int_right->value));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      if(bool_right->to_int() == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(this->value / bool_right->to_int()));
    }else{
      return *(new NotImplemented());
    }
  };
  // // __mod__
  // std::function<Object& (std::list<Object*> arguments)> __mod__ = [this](std::list<Object*> arguments)-> Object& {
  //   //TODO protect division by zero
  //   if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
  //     return *(new IntObject(this->value % int_right->value));
  //   }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
  //     return *(new IntObject(this->value % bool_right->to_int()));
  //   }else{
  //     return *(new NotImplemented());
  //   }
  // };
  // __exp__
  std::function<Object& (std::list<Object*> arguments)> __exp__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(pow(this->value, float_right->value)));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(pow(this->value, int_right->value)));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(pow(this->value, bool_right->value)));
    }else{
      return *(new NotImplemented());
    }
  };
  // __fdv__
  std::function<Object& (std::list<Object*> arguments)> __fdv__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_right = dynamic_cast<FloatObject*>(arguments.front())){
      if(float_right->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(this->value / float_right->value)));
    }else if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      if(int_right->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(this->value / int_right->value)));
    }else if(BoolObject* bool_right = dynamic_cast<BoolObject*>(arguments.front())){
      if(bool_right->to_int() == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(this->value / bool_right->to_int())));
    }else{
      return *(new NotImplemented());
    }
  };

  ///////////////////
  // __rsub__
  std::function<Object& (std::list<Object*> arguments)> __rsub__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_left = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(float_left->value - this->value));
    }else if(IntObject* int_left = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(int_left->value - this->value));
    }else if(BoolObject* bool_left = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(bool_left->to_int() - this->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __rdiv__
  std::function<Object& (std::list<Object*> arguments)> __rdiv__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_left = dynamic_cast<FloatObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(float_left->value / this->value));
    }else if(IntObject* int_left = dynamic_cast<IntObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(int_left->value / this->value));
    }else if(BoolObject* bool_left = dynamic_cast<BoolObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(bool_left->to_int() / this->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // // __rmod__
  // std::function<Object& (std::list<Object*> arguments)> __rmod__ = [this](std::list<Object*> arguments)-> Object& {
  //   //TODO protect division by zero
  //   if(IntObject* int_left = dynamic_cast<IntObject*>(arguments.front())){
  //     return *(new IntObject(int_left->value % this->value));
  //   }else if(BoolObject* bool_left = dynamic_cast<BoolObject*>(arguments.front())){
  //     return *(new IntObject(bool_left->to_int() % this->value));
  //   }else{
  //     return *(new NotImplemented());
  //   }
  // };
  // __rexp__
  std::function<Object& (std::list<Object*> arguments)> __rexp__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_left = dynamic_cast<FloatObject*>(arguments.front())){
      return *(new FloatObject(pow(float_left->value, this->value)));
    }else if(IntObject* int_left = dynamic_cast<IntObject*>(arguments.front())){
      return *(new FloatObject(pow(int_left->value, this->value)));
    }else if(BoolObject* bool_left = dynamic_cast<BoolObject*>(arguments.front())){
      return *(new FloatObject(pow(bool_left->value, this->value)));
    }else{
      return *(new NotImplemented());
    }
  };
  // __rfdv__
  std::function<Object& (std::list<Object*> arguments)> __rfdv__ = [this](std::list<Object*> arguments)-> Object& {
    if(FloatObject* float_left = dynamic_cast<FloatObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(float_left->value / this->value)));
    }else if(IntObject* int_left = dynamic_cast<IntObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(int_left->value / this->value)));
    }else if(BoolObject* bool_left = dynamic_cast<BoolObject*>(arguments.front())){
      if(this->value == 0) throw std::runtime_error("ZeroDivisionError: division by zero");
      return *(new FloatObject(static_cast<long>(bool_left->to_int() / this->value)));
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

  // __int__
  std::function<Object& (std::list<Object*> arguments)> __int__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new IntObject(static_cast<int>(this->value)));
  };

  // __float__
  std::function<Object& (std::list<Object*> arguments)> __float__ = [this](std::list<Object*> arguments)-> Object& {
    return *(new FloatObject(this->value));
  };

  newName("__neg__", *(new BuiltInFunction(__neg__)));
  newName("__pos__", *(new BuiltInFunction(__pos__)));

  newName("__add__", *(new BuiltInFunction(__add__)));
  newName("__sub__", *(new BuiltInFunction(__sub__)));
  newName("__mul__", *(new BuiltInFunction(__mul__)));
  newName("__div__", *(new BuiltInFunction(__div__)));
  //newName("__mod__", *(new BuiltInFunction(__mod__)));
  newName("__exp__", *(new BuiltInFunction(__exp__)));
  newName("__fdv__", *(new BuiltInFunction(__fdv__)));

  newName("__radd__", *(new BuiltInFunction(__add__)));
  newName("__rsub__", *(new BuiltInFunction(__rsub__)));
  newName("__rmul__", *(new BuiltInFunction(__mul__)));
  newName("__rdiv__", *(new BuiltInFunction(__rdiv__)));
  //ewName("__rmod__", *(new BuiltInFunction(__rmod__)));
  newName("__rexp__", *(new BuiltInFunction(__rexp__)));
  newName("__rfdv__", *(new BuiltInFunction(__rfdv__)));

  newName("__bool__", *(new BuiltInFunction(__bool__)));
  newName("__int__", *(new BuiltInFunction(__int__)));
  newName("__float__", *(new BuiltInFunction(__float__)));
}
