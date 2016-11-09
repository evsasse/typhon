#include "oo.h"
#include "ast.h"
#include <iostream>

void Namespace::newName(std::string name, Object& obj){
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

IntObject::IntObject (int value) :
Object("int"), value(value) {
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    //IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
    IntObject* int_right = dynamic_cast<IntObject*>(arguments.front());
    if(int_right){
      return *(new IntObject(this->value + int_right->value));
    }else{
      return *(new Object());
    }
  };
  newName("__add__", *(new BuiltInFunction(__add__)));
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
