#include <iostream>
#include "string.h"
#include "ast.h"

extern int yyparse();

bool DEBUG = 0;

int main(int argc, char* argv[]){

  for(int i=0; i<argc; i++){
    if(strcmp(argv[i],"-d") == 0)
      DEBUG = 1;
    else
     std::cout << argv[i] << " $ ";
  }

  std::cout << "Typhon 0.0.1" << std::endl << std::flush;
  std::cout << ">>> " << std::flush;
  yyparse();
  return 0;
}
