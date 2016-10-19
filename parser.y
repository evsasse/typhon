%{
  #include <iostream>

  extern int yylex();

  void yyerror(const char *s);
%}

%define parse.error verbose

%union {
  const char *str;
}

%token T_INDENT T_NEWLINE
%token <str> T_OTHER

%%

program : program symbol
        | %empty
        ;

symbol : T_OTHER { std::cout << $1; }
       | T_INDENT { std::cout << "< >"; }
       | T_NEWLINE { std::cout << std::endl; }
       ;

%%

void yyerror(const char *s){
  std::cout << s;
}
