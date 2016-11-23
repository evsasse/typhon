#include "oo.h"
#include "ast.h"

#include <iostream>

std::string ListObject::getIdentifier(){
  std::string ret = "<list [";
  for(auto obj : values){
    ret += obj->getIdentifier();
  }
  ret += "]>";
  return ret;
}

ListObject::ListObject(std::list<Object*>& values) :
Object("list"), values(values){
}
