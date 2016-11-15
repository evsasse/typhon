#include "oo.h"
#include "ast.h"
#include <math.h>
#include <iostream>

void Namespace::newName(std::string name, Object& obj){
  //std::cout << "[" << name << " = "<< obj.getIdentifier() << "]" << std::flush;
  space[name] = &obj;
}

Object& Namespace::useName(std::string name){
  if(space.find(name) != space.end())
    return *(space[name]);
  else
    throw std::runtime_error("NameError: name '"+name+"' is not defined");
}

std::string Object::getIdentifier(){
  return "<object '"+identifier+"'>";
}

Object& Object::call(std::list<Object*> arguments){
  throw std::runtime_error("TypeError: "+getIdentifier()+" object is not callable");
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
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
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value + int_right->value));
    }else{
      return *(new Object());
    }
  };
  // __sub__
  std::function<Object& (std::list<Object*> arguments)> __sub__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value - int_right->value));
    }else{
      return *(new Object());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value * int_right->value));
    }else{
      return *(new Object());
    }
  };
  // __div__
  std::function<Object& (std::list<Object*> arguments)> __div__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value / int_right->value));
    }else{
      return *(new Object());
    }
  };
  // __mod__
  std::function<Object& (std::list<Object*> arguments)> __mod__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value % int_right->value));
    }else{
      return *(new Object());
    }
  };
  // __exp__
  std::function<Object& (std::list<Object*> arguments)> __exp__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(pow(this->value, int_right->value)));
    }else{
      return *(new Object());
    }
  };
  // __fdv__
  std::function<Object& (std::list<Object*> arguments)> __fdv__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value / int_right->value));
    }else{
      return *(new Object());
    }
  };

  ///////////////////
  // __bool__
  std::function<Object& (std::list<Object*> arguments)> __bool__ = [this](std::list<Object*> arguments)-> Object& {
    //TODO return actual BoolObject
    if(this->value == 0){
      return *(new IntObject(0));
    }else{
      return *(new IntObject(1));
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

std::string IntObject::getIdentifier(){
  return "<int "+std::to_string(value)+">";
}

Function::Function(Name &name, std::list<Parameter*>& parameters, Block &body) :
Object("function "+name.name), parameters(parameters), body(body) {};

Object& Function::call(std::list<Object*> arguments){
  int diff = arguments.size() - parameters.size();
  if(diff < 0){
    throw std::runtime_error("TypeError: "+identifier+"() missing "+std::to_string(-diff)+" required arguments");
  }else if(diff > 0){
    throw std::runtime_error("TypeError: "+identifier+"() takes "+std::to_string(parameters.size())+" arguments but "+std::to_string(arguments.size())+" were given");
  }

  auto arg = arguments.begin();
  auto param = parameters.begin();
  // arguments and parameters have the same size
  for(; arg != arguments.end(); arg++, param++){
    body.newName((*param)->name, *(*arg));
  }

  for(Statement *stt : body){
    Object* ret = stt->interpret();
    if(ret) return *ret;
  }
  return *(new Object());
  // TODO add a return None statement at the end of every function
  //throw std::runtime_error("ImplementationError: unexpected function ending without return");
}

Object& BuiltInFunction::call(std::list<Object*> arguments){
  return function(arguments);
}
