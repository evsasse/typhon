%{
  #include <iostream>

  extern int yylex();

  void yyerror(const char *s);
%}

%define parse.error verbose

%union {
  const char *str;
}

%token T_INDENT T_NEWLINE T_STATEMENT

%%

program : program T_NEWLINE { std::cout << std::endl; } line
        | line
        ;

line : indent /* empty line */
     | indent statements opt-semicolon /* could have multiple statements per line divided by semicolons */
     ;

indent : indent T_INDENT { std::cout << "< >"; }
       | %empty
       ;

statements : statements ';' { std::cout << ";"; } statement
           | statement
           ;

statement : T_STATEMENT { std::cout << "stt"; }
          ;

opt-semicolon : ';' { std::cout << ";"; }
              | %empty
              ;

%%

void yyerror(const char *s){
  std::cout << s;
}
