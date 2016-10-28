#include "oo.h"

std::string Object::getIdentifier(){
  return "<object '"+identifier+"'>";
}

std::string Class::getIdentifier(){
  return "<class '"+identifier+"'>";
}
