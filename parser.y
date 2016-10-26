%{
  #include <iostream>

  #include "ast.h"

  extern int yylex();

  void yyerror(const char *s);

  Block program;
  int cur_indent = 666; // Current line number of indents
  //int exp_indent = 0; // Current block number of indents
  //int cur_level = 0; // Current block level
  //bool new_level = 0; // Is an increase in indentation expected
%}

%define parse.error verbose

%union {
  const char *val_str;
  int val_int;
  float val_float;
  bool val_bool;

  Statement *stt;
  Expression *expr;
  Assignment *assign;
  Value *value;
  Name *name;
}

%left '+' '-'
%left '*' '/' O_FDV
%left O_UNARY
%left O_EXP

%token <val_int> L_INT
%token <val_float> L_FLOAT
%token <val_bool> L_BOOL
%token <val_str> T_NAME

%token T_INDENT T_NEWLINE T_STATEMENT

%type <val_int> indent
%type <stt> statement
%type <value> value
%type <name> name
%type <expr> expression
%type <assign> assignment


%%

program : program T_NEWLINE line
        | line
        ;

line : indent /* empty line */
     | indent { cur_indent = $1; } statements opt-semicolon
     ;

indent : indent T_INDENT { $$ = $1 + 1; }
       | %empty { $$ = 0; }
       ;

statements : statements ';' statement { program.push_back($3); $3->setIndent(cur_indent); }
           | statement { program.push_back($1); $1->setIndent(cur_indent); }
           ;

statement : expression { $$ = $1; }
          | assignment { $$ = $1; }
          ;

opt-semicolon : ';'
              | %empty
              ;

value : L_INT { $$ = new LitInt($1); }
      | L_FLOAT { $$ = new LitFloat($1); }
      | L_BOOL { $$ = new LitBool($1); }
      ;

name : T_NAME { $$ = new Name($1); }
     ;

expression : value { $$ = $1; }
           | name { $$ = $1; }
           | '(' expression ')' { $$ = $2; }
           | '+' expression %prec O_UNARY { $$ = new UnaryOp(Op::ADD, *$2); }
           | '-' expression %prec O_UNARY { $$ = new UnaryOp(Op::SUB, *$2); }
        /* | expression '[' expression-list ']' new subscription */
        /* | expression '(' expression-list ')' new call */
           | expression '+' expression { $$ = new BinaryOp(*$1, Op::ADD, *$3); }
           | expression '-' expression { $$ = new BinaryOp(*$1, Op::SUB, *$3); }
           | expression '*' expression { $$ = new BinaryOp(*$1, Op::MUL, *$3); }
           | expression '/' expression { $$ = new BinaryOp(*$1, Op::DIV, *$3); }
           | expression O_EXP expression { $$ = new BinaryOp(*$1, Op::EXP, *$3); }
           | expression O_FDV expression { $$ = new BinaryOp(*$1, Op::FDV, *$3); }
           ;

assignment : name '=' expression { $$ = new Assignment(*$1,*$3); }
           ;

%%

void yyerror(const char *s){
  std::cout << s;
}
