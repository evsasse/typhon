%{
  #include <iostream>
  
  extern int yylex();

  void yyerror(const char *s);
%}

%define parse.error verbose

%token T_INDENT T_OTHER T_NEWLINE

%%

program : program symbol
        | %empty
        ;

symbol : T_INDENT { std::cout << "INDENT "; }
       | T_NEWLINE { std::cout << std::endl; }
       | T_OTHER { std::cout << "OTHER "; }
       ;

%%

void yyerror(const char *s){
  std::cout << s;
}
