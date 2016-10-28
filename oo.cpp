#include "oo.h"

void Namespace::newName(std::string name, Object& obj){
  space[name] = &obj;
}

Object& Namespace::useName(std::string name){
  return *(space[name]);
}

std::string Object::getIdentifier(){
  return "<object '"+identifier+"'>";
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
}
