%{
  #include <iostream>

  #include "ast.h"

  extern int yylex();

  void yyerror(const char *s);

  Block* cur_block = new MainBlock();
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

  Block *block;
  Statement *stt;
  Expression *expr;
  Assignment *assign;
  FunctionDef *funcd;
  Value *value;
  Name *name;
}

%left '+' '-'
%left '*' '/' O_FDV
%left O_UNARY
%left O_EXP

%token A_SUM

%token <val_int> L_INT
%token <val_float> L_FLOAT
%token <val_bool> L_BOOL
%token <val_str> T_NAME

%token T_INDENT T_NEWLINE T_DEF

%type <block> scoped-block body inline-body
%type <val_int> indent
%type <stt> statement
%type <value> value
%type <name> name
%type <expr> expression
%type <assign> assignment
%type <funcd> function


%%

program : block
        ;

scoped-block : { cur_block = new Block(cur_block); }
               block
               { $$ = cur_block; cur_block = cur_block->getParent(); }
             ;

block : block T_NEWLINE line
      | line
      ;

line : indent /* empty line */
     | indent { cur_indent = $1; } statements opt-semicolon
     ;

indent : indent T_INDENT { $$ = $1 + 1; }
       | %empty { $$ = 0; }
       ;

statements : statements ';' statement
             { $3->setIndent(cur_indent); cur_block->push($3); }
           | statement { $1->setIndent(cur_indent); cur_block->push($1); }
           ;

statement : expression { $$ = $1; }
          | assignment { $$ = $1; }
          | function { $$ = $1; }
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

assignment : name '=' expression
             { $$ = new Assignment(*$1, *$3); }
           | name A_SUM expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::ADD, *$3))); }
           ;

function : T_DEF T_NAME '(' ')' ':' body { $$ = new FunctionDef($2, *$6); }
         ;

body : inline-body { $$ = $1; }
     | T_NEWLINE scoped-block { $$ = $2; }

inline-body : { cur_block = new Block(cur_block); }
              statements opt-semicolon
              { $$ = cur_block; cur_block = cur_block->getParent(); }
            ;

%%

void yyerror(const char *s){
  std::cout << s;
}
