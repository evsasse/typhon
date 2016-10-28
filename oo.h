#pragma once

#include <string>

class Object {
public:
  Object(std::string identifier = "anonymous") :
  identifier(identifier) {};
  virtual std::string getIdentifier();
  //virtual Object call(/* varargs? Object? */);
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
  int value;
  IntObject(int value) :
  Object("int"), value(value) {};
};
