#include "ast.h"

extern int yyparse();
extern Block program;

int main(){
  yyparse();
  program.print();
  return 0;
}
