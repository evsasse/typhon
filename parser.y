%{
  #include <iostream>

  #include "ast.h"

  extern int yylex();

  void yyerror(const char *s);

  Program program;
  int cur_indent = 666; // Current line number of indents
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
  FunctionRet *funcr;
  Value *value;
  Name *name;
  Parameter *param;
  IfStatement *ifstt;
  ElseStatement *elsestt;
  ElifStatement *elifstt;
  WhileStatement *whilestt;

  std::list<Expression*> *exprs;
  std::list<Parameter*> *params;
}

%left '+' '-'
%left '*' '/' O_FDV
%left O_UNARY
%left O_EXP
%left '('

%token A_SUM

%token <val_int> L_INT
%token <val_float> L_FLOAT
%token <val_bool> L_BOOL
%token <val_str> T_NAME

%token T_INDENT T_NEWLINE
%token T_DEF T_RETURN
%token T_IF T_ELIF T_ELSE
%token T_WHILE

%type <val_int> indent
%type <stt> statement simple-statement
%type <value> value
%type <name> name
%type <expr> expression
%type <exprs> expression-list expression-list-opt
%type <param> parameter
%type <params> parameter-list parameter-list-opt
%type <assign> assignment
%type <funcd> function
%type <funcr> return
%type <ifstt> if
%type <elsestt> else
%type <elifstt> elif
%type <whilestt> while

%%

program : block
        ;

block : block T_NEWLINE line
      | line
      ;

line : indent /* empty line */ { std::cout << ">>> "; }
     | indent { cur_indent = $1; } statements opt-semicolon
     ;

indent : indent T_INDENT { $$ = $1 + 1; }
       | %empty { $$ = 0; }
       ;

statements : statement { $1->setIndent(cur_indent); program.push($1); }
        /* | error { yyerrok; } */
           ;

statement : simple-statement { $$ = $1; }
          | function { $$ = $1; }
          | if { $$ = $1; }
          | elif { $$ = $1; }
          | else { $$ = $1; }
          | while { $$ = $1; }
          ;

simple-statement: /* one that does not contain blocks and new lines */
                  expression { $$ = $1; }
                | assignment { $$ = $1; }
                | return { $$ = $1; }
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
           | expression '(' expression-list-opt ')' { $$ = new CallOp(*$1, *$3); }
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

expression-list-opt : %empty { $$ = new std::list<Expression*>(); }
                    | expression-list { $$ = $1; }
                    ;

expression-list : expression-list ',' expression { $1->push_back($3); $$ = $1; }
                | expression { $$ = new std::list<Expression*>(); $$->push_back($1); }
                ;

assignment : name '=' expression
             { $$ = new Assignment(*$1, *$3); }
           | name A_SUM expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::ADD, *$3))); }
           ;

function : T_DEF name '(' parameter-list-opt ')' ':' { $$ = new FunctionDef(*$2,*$4); }
         ;

parameter-list-opt : %empty { $$ = new std::list<Parameter*>(); }
                   | parameter-list { $$ = $1; }
                   ;

parameter-list : parameter-list ',' parameter { $1->push_back($3); $$ = $1;}
               | parameter { $$ = new std::list<Parameter*>(); $$->push_back($1); }

parameter : T_NAME { $$ = new Parameter($1); }

return : T_RETURN expression { $$ = new FunctionRet(*$2); }

if : T_IF expression ':' { $$ = new IfStatement(*$2); }
   ;

elif : T_ELIF expression ':' { $$ = new ElifStatement(*$2); }
     ;

else : T_ELSE ':' { $$ = new ElseStatement(); }
     ;

while : T_WHILE expression ':' { $$ = new WhileStatement(*$2); }

%%

void yyerror(const char *s){
  std::cout << s;
}
