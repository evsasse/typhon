#pragma once

#include <string>
#include <map>
#include <list>
#include <functional>

class Object;

class Namespace {
public:
  void newName(std::string name, Object& obj);
  Object& useName(std::string name);
private:
  std::map<std::string,Object*> space;
};

class Object : public Namespace {
public:
  bool builtInFunction;
  Object(std::string identifier = "None") :
  identifier(identifier), builtInFunction(0) {};
  virtual std::string getIdentifier();
  virtual Object& call(std::list<Object*> arguments = std::list<Object*>());
protected:
  std::string identifier;
};

class Class : public Object {
public:
  Class(std::string identifier = "anonymous") :
  Object(identifier) {};
  std::string getIdentifier();
};

//Create an int class
  //Make a function
    //A function is a callable Object
    //every object can be callable
    //a call interprets __call__
  //Make a builtin function
    //override a call to execute something instead of interpret __call__
//Create an instance of int
  //Call the int function
  //Receive an int object with a int value

// class IntClass : public Class {
// public:
//   IntClass() :
//   Class("int") {};
//   Object& init(int);
// };

class IntObject : public Object {
public:
  long value;
  IntObject(long value);
  std::string getIdentifier();
};

class Name;
class Block;
class Parameter;

class Function : public Object {
public:
  Function(Name &name, std::list<Parameter*>& parameters, Block &body);
  Object& call(std::list<Object*> arguments);
private:
  Block& body;
  std::list<Parameter*> parameters;
};

class BuiltInFunction : public Object {
public:
  BuiltInFunction(std::function<Object& (std::list<Object*> arguments)> function) :
  Object("builtin function"), function(function) {};
  Object& call(std::list<Object*> arguments);
private:
  std::function<Object& (std::list<Object*> arguments)> function;
};
