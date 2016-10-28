#include "oo.h"
#include <iostream>

void Namespace::newName(std::string name, Object& obj){
  space[name] = &obj;
}

Object& Namespace::useName(std::string name){
  return *(space[name]);
}

std::string Object::getIdentifier(){
  return "<object '"+identifier+"'>";
}

Object& Object::call(const Object& obj){
  std::cerr << " object is not callable";
  return *(new Object());
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
}

IntObject::IntObject (int value) :
Object("int"), value(value) {
  newName("__add__", *(new BuiltInFunction(
    [](const Object& right)-> Object& {
      IntObject* int_right = dynamic_cast<IntObject*>(const_cast<Object*>(&right));
      if(int_right){
        return *(new IntObject(666 + int_right->value));
      }else{
        return *(new Object());
      }
    })
  ));
}

std::string IntObject::getIdentifier(){
  return "<"+std::to_string(value)+">";
}

Object& BuiltInFunction::call(const Object& obj){
  return function(obj);
}
