%{
  #include <iostream>

  #include "ast.h"

  extern int yylex();

  void yyerror(const char *s);

  Block program;
%}

%define parse.error verbose

%union {
  const char *str;

  Statement *stt;
  Expression *expr;
  Value *value;
}

%left '+' '-'
%left '*' '/'

%token T_INDENT T_NEWLINE T_STATEMENT

%type <stt> statement statements

%%

program : program T_NEWLINE { std::cout << std::endl; } line
        | line
        ;

line : indent /* empty line */
     | indent statements opt-semicolon {  }
     ;

indent : indent T_INDENT { std::cout << "< >"; }
       | %empty
       ;

statements : statements ';' { std::cout << ";"; } statement { $$ = $1 }
           | statement
           ;

statement : expression
          ;

opt-semicolon : ';' { std::cout << ";"; }
              | %empty
              ;

expression : value
           | '(' expression ')'
           | expression binary-op expression { $$ = new BinaryOp($1, $2, $3); }
           ;

binary-op : '+' { $$ = Op::ADD; }
          | '-' { $$ = Op::SUB; }
          | '*' { $$ = Op::MUL; }
          | '/' { $$ = Op::DIV; }
          ;

value : T_STATEMENT { $$ = new Value(); }
      ;

%%

void yyerror(const char *s){
  std::cout << s;
}
