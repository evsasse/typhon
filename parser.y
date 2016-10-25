%{
  #include <iostream>

  #include "ast.h"

  extern int yylex();

  void yyerror(const char *s);

  Block program;
%}

%define parse.error verbose

%union {
  const char *val_str;
  int val_int;
  float val_float;
  bool val_bool;

  Statement *stt;
  Expression *expr;
  Value *value;
}

%left '+' '-'
%left '*' '/' O_FDV
%left O_EXP

%token <int> L_INT
%token <float> L_FLOAT
%token <bool> L_BOOL
%token <str> T_NAME

%token T_INDENT T_NEWLINE T_STATEMENT

%type <stt> statement statements
%type <expr> expression
%type <value> value

%%

program : program T_NEWLINE line
        | line
        ;

line : indent /* empty line */
     | indent statements opt-semicolon { program.push_back($2); }
     ;

indent : indent T_INDENT
       | %empty
       ;

statements : statements ';' statement { $$ = $1; }
           | statement
           ;

statement : expression { $$ = $1; }
          ;

opt-semicolon : ';'
              | %empty
              ;

expression : value { $$ = $1; }
           | '(' expression ')' { $$ = $2; }
           | expression '+' expression { $$ = new BinaryOp(*$1, Op::ADD, *$3); }
           | expression '-' expression { $$ = new BinaryOp(*$1, Op::SUB, *$3); }
           | expression '*' expression { $$ = new BinaryOp(*$1, Op::MUL, *$3); }
           | expression '/' expression { $$ = new BinaryOp(*$1, Op::DIV, *$3); }
           | expression O_EXP expression { $$ = new BinaryOp(*$1, Op::EXP, *$3); }
           | expression O_FDV expression { $$ = new BinaryOp(*$1, Op::FDV, *$3); }
           ;

value : L_INT { $$ = new LitInt(); }
      | L_FLOAT { $$ = new LitFloat(); }
      | L_BOOL { $$ = new LitBool(); }
      ;

%%

void yyerror(const char *s){
  std::cout << s;
}
