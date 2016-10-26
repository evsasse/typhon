#include <iostream>
#include "ast.h"

extern int yyparse();
extern Block program;

int main(){
  std::cout << "Typhon 0.0.1" << std::endl;
  yyparse();
  program.print();
  return 0;
}
