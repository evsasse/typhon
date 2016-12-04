#include "oo.h"
#include "ast.h"

#include <iostream>

std::string StringObject::getIdentifier(){
  return "<string '"+value+"'>";
}

StringObject::StringObject(std::string value) :
Object("string"), value(value){
  ///////////////////
  // __add__
  std::function<Object& (std::list<Object*> arguments)> __add__ = [this](std::list<Object*> arguments)-> Object& {
    if(StringObject* string_right = dynamic_cast<StringObject*>(arguments.front())){
      return *(new StringObject(""+this->value+string_right->value));
    }else{
      return *(new NotImplemented());
    }
  };
  // __mul__
  std::function<Object& (std::list<Object*> arguments)> __mul__ = [this](std::list<Object*> arguments)-> Object& {
    if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
      std::string ret = "";
      for(int i=0; i<int_right->value; i++){
        ret += this->value;
      }
      return *(new StringObject(ret));
    }else{
      return *(new NotImplemented());
    }
  };

  // ///////////////////
  // // __getitem__
  // std::function<Object& (std::list<Object*> arguments)> __getitem__ = [this](std::list<Object*> arguments)-> Object& {
  //   if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
  //     auto index = int_right->value;
  //     auto it = this->values.begin();
  //     bool negative = 0;
  //     if(index < 0){
  //       negative = 1;
  //       index = -(index+1);
  //     }
  //
  //     if(index >= this->values.size()){
  //       return *(new IndexError());
  //     }
  //
  //     if(negative){
  //       index = this->values.size() - index - 1;
  //     }
  //
  //     std::advance(it,index);
  //     return *(*it);
  //   }else{
  //     throw std::runtime_error("TypeError: list indices must be integers, not "+arguments.front()->getIdentifier());
  //   }
  // };
  //
  // // __setitem__
  // std::function<Object& (std::list<Object*> arguments)> __setitem__ = [this](std::list<Object*> arguments)-> Object& {
  //   auto new_value = arguments.back();
  //   if(IntObject* int_right = dynamic_cast<IntObject*>(arguments.front())){
  //     auto index = int_right->value;
  //     auto it = this->values.begin();
  //     bool negative = 0;
  //     if(index < 0){
  //       negative = 1;
  //       index = -(index+1);
  //     }
  //
  //     if(index >= this->values.size()){
  //       return *(new IndexError());
  //     }
  //
  //     if(negative){
  //       index = this->values.size() - index - 1;
  //     }
  //
  //     std::advance(it,index);
  //
  //     this->values.insert(it, new_value);
  //     this->values.erase(it);
  //     return *(*it);
  //   }else{
  //     throw std::runtime_error("TypeError: list indices must be integers, not "+arguments.front()->getIdentifier());
  //   }
  // };
  //
  // // __bool__
  // std::function<Object& (std::list<Object*> arguments)> __bool__ = [this](std::list<Object*> arguments)-> Object& {
  //   if(this->values.size() > 0){
  //     return *(new BoolObject(1));
  //   }
  //   return *(new BoolObject(0));
  // };
  // // __not__
  // std::function<Object& (std::list<Object*> arguments)> __not__ = [this](std::list<Object*> arguments)-> Object& {
  //   if(this->values.size() > 0){
  //     return *(new BoolObject(0));
  //   }
  //   return *(new BoolObject(1));
  // };
  //
  // ///////////////////
  // // __and__
  // std::function<Object& (std::list<Object*> arguments)> __and__ = [this](std::list<Object*> arguments)-> Object& {
  //   if(this->values.size() == 0) return *(new ListObject(this->values));
  //   return *arguments.front();
  // };
  // ///////////////////
  // // __or__
  // std::function<Object& (std::list<Object*> arguments)> __or__ = [this](std::list<Object*> arguments)-> Object& {
  //   if(this->values.size() != 0) return *(new ListObject(this->values));
  //   return *arguments.front();
  // };

  newName("__add__", *(new BuiltInFunction(__add__)));
  newName("__mul__", *(new BuiltInFunction(__mul__)));

  //newName("__getitem__", *(new BuiltInFunction(__getitem__)));
  //newName("__setitem__", *(new BuiltInFunction(__setitem__)));

  //newName("__bool__", *(new BuiltInFunction(__bool__)));
  //newName("__not__", *(new BuiltInFunction(__not__)));

  //newName("__and__", *(new BuiltInFunction(__and__)));
  //newName("__or__", *(new BuiltInFunction(__or__)));
}
