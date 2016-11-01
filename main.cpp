#include <iostream>
#include "ast.h"

extern int yyparse();

int main(){
  std::cout << "Typhon 0.0.1" << std::endl;
  yyparse();
  return 0;
}
