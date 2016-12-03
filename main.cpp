#include <iostream>
#include "string.h"
#include "ast.h"

extern int yyparse();

bool DEBUG = 0;
bool HYBRID_INDENT = 0;
int HYBRID_INDENT_COUNT = 0;

int main(int argc, char* argv[]){

  for(int i=0; i<argc; i++){
    if(HYBRID_INDENT){
      HYBRID_INDENT = 0;
      HYBRID_INDENT_COUNT = atoi(argv[i]);
      std::cout << "HYBRID INDENTS " << argv[i] << " $ ";
    }else if(strcmp(argv[i],"-d") == 0){
      DEBUG = 1;
      std::cout << "DEBUG MESSAGES " << " $ ";
    }else if(strcmp(argv[i],"-t") == 0){
      HYBRID_INDENT = 1;
    }else{
     std::cout << argv[i] << " $ ";
    }
  }

  std::cout << "Typhon 0.0.1" << std::endl << std::flush;
  std::cout << ">>> " << std::flush;
  yyparse();
  return 0;
}
