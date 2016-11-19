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
    throw NameError(name);
}

Object::Object(std::string identifier = "None") :
identifier(identifier), builtInFunction(0) {
  //TODO default comparison operations by address number
};

std::string Object::getIdentifier(){
  return "<object '"+identifier+"'>";
}

Object& Object::call(std::list<Object*> arguments){
  throw std::runtime_error("TypeError: "+getIdentifier()+" object is not callable");
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
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
