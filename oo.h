#pragma once

#include <string>
#include <map>
#include <list>
#include <functional>

class NameError : public std::runtime_error {
public:
  NameError(std::string name) :
  std::runtime_error("NameError: name '"+name+"' is not defined") {};
};

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
  Object(std::string identifier);
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

class NoneObject : public Object {
public:
  NoneObject();
};

class NotImplemented : public Object {
public:
  NotImplemented() :
  Object("NotImplemented") {};
};

class IndexError : public Object {
public:
  IndexError() :
  Object("IndexError") {};
};

class FloatObject : public Object {
public:
  double value;
  FloatObject(double value);
  std::string getIdentifier();
};

class IntObject : public Object {
public:
  long value;
  IntObject(long value);
  std::string getIdentifier();
};

class BoolObject : public Object {
public:
  bool value;
  BoolObject(bool value);
  std::string getIdentifier();
  int to_int();
};

class ListObject : public Object {
public:
  std::list<Object*>& values;
  ListObject(std::list<Object*>& values);
  std::string getIdentifier();
};

class StringObject : public Object {
public:
  std::string value;
  StringObject(std::string value);
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
