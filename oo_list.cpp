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
  ///////////////////
  // __add__
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    if(ListObject* list_right = dynamic_cast<ListObject*>(arguments.front())){
      auto list = new std::list<Object*>();
      list->insert(list->end(),this->values.begin(),this->values.end());
      list->insert(list->end(),list_right->values.begin(),list_right->values.end());
      return *(new ListObject(*list));
    }else{
      return *(new NotImplemented());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      auto list = new std::list<Object*>();
      for(int i=0; i< int_right->value; i++)
        list->insert(list->end(),this->values.begin(),this->values.end());
      return *(new ListObject(*list));
    }else{
      return *(new NotImplemented());
    }
  };

  newName("__add__", *(new BuiltInFunction(__add__)));
  newName("__mul__", *(new BuiltInFunction(__mul__)));
}