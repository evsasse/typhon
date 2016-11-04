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

Object& Object::call(const Object& obj){
  throw std::runtime_error("TypeError: "+getIdentifier()+" object is not callable");
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
}

IntObject::IntObject (int value) :
Object("int"), value(value) {
  std::function<Object& (const Object& right)> __add__ = [this](const Object& right)-> Object& {
    IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
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

Function::Function(Name &name, Block &body) :
Object("function "+name.name), body(body) {};

Object& Function::call(const Object& obj){
  for(Statement *stt : body){
    stt->interpret();
  }
  return *(new Object());
}

Object& BuiltInFunction::call(const Object& obj){
  return function(obj);
}
