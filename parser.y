%{
  #include <iostream>
  #include <cstring>

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
  LitList *arr;
  Parameter *param;
  IfStatement *ifstt;
  ElseStatement *elsestt;
  ElifStatement *elifstt;
  WhileStatement *whilestt;
  ForStatement *forstt;

  std::list<Expression*> *exprs;
  std::list<Parameter*> *params;
}

%left O_AND O_OR O_NOT
%left O_LT O_LE O_EQ O_NE O_GE O_GT
%left '+' '-'
%left '%' '*' '/' O_FDV
%left O_UNARY
%left O_EXP
%left '(' '['

%token A_SUM A_SUB A_MUL A_DIV
%token A_MOD A_EXP A_FDV

%token <val_int> L_INT
%token <val_float> L_FLOAT
%token <val_bool> L_BOOL
%token <val_str> T_NAME T_CHAR
%token T_STRING

%token T_INDENT T_NEWLINE
%token T_DEF T_RETURN
%token T_IF T_ELIF T_ELSE
%token T_WHILE T_FOR T_IN
%token T_PASS
%token T_IMPORT

%type <val_int> indent
%type <val_str> string
%type <stt> statement simple-statement
%type <value> value
%type <name> name
%type <arr> array
%type <expr> expression assignment-target
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
%type <forstt> for

%%

program : block { /* includes a 'pass' statement at the end of file to close any unfinished blocks*/
           auto pass = new PassStatement(); pass->setIndent(0); program.push(pass); }
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
           | error { auto pass = new SyntaxError(); pass->setIndent(0); program.push(pass); }
           ;

statement : simple-statement { $$ = $1; }
          | function { $$ = $1; }
          | if { $$ = $1; }
          | elif { $$ = $1; }
          | else { $$ = $1; }
          | while { $$ = $1; }
          | for { $$ = $1; }
          ;

simple-statement: /* one that does not contain blocks and new lines */
                  expression { $$ = $1; }
                | assignment { $$ = $1; }
                | return { $$ = $1; }
                | T_IMPORT T_NAME { $$ = new ImportStatement($2); }
                | T_PASS { $$ = new PassStatement(); }
                ;

opt-semicolon : ';'
              | %empty
              ;

string : string T_CHAR
         { char *concat = new char[std::strlen($1)+std::strlen($2)+1];
           std::strcpy(concat,$1);
           std::strcat(concat,$2);
           $$ = concat; }
       | T_STRING { $$ = ""; }
       ;

value : L_INT { $$ = new LitInt($1); }
      | L_FLOAT { $$ = new LitFloat($1); }
      | L_BOOL { $$ = new LitBool($1); }
      | string { $$ = new LitString($1); }
      ;

name : T_NAME { $$ = new Name($1); }
     ;

array : '[' expression-list-opt ']' { $$ = new LitList(*$2); }

expression : value { $$ = $1; }
           | name { $$ = $1; }
           | array { $$ = $1; }
           | expression '(' expression-list-opt ')' { $$ = new CallOp(*$1, *$3); }
           | expression '[' expression ']' { $$ = new BinaryOp(*$1, Op::KEY, *$3); }
           | '(' expression ')' { $$ = $2; }
           | '+' expression %prec O_UNARY { $$ = new UnaryOp(Op::ADD, *$2); }
           | '-' expression %prec O_UNARY { $$ = new UnaryOp(Op::SUB, *$2); }
           | O_NOT expression %prec O_UNARY { $$ = new UnaryOp(Op::NOT, *$2); }
        /* */
           | expression '+' expression { $$ = new BinaryOp(*$1, Op::ADD, *$3); }
           | expression '-' expression { $$ = new BinaryOp(*$1, Op::SUB, *$3); }
           | expression '*' expression { $$ = new BinaryOp(*$1, Op::MUL, *$3); }
           | expression '/' expression { $$ = new BinaryOp(*$1, Op::DIV, *$3); }
           | expression '%' expression { $$ = new BinaryOp(*$1, Op::MOD, *$3); }
           | expression O_EXP expression { $$ = new BinaryOp(*$1, Op::EXP, *$3); }
           | expression O_FDV expression { $$ = new BinaryOp(*$1, Op::FDV, *$3); }
        /* TODO comparison operations should work as "x<y<z" */
           | expression O_LT expression { $$ = new BinaryOp(*$1, Op::LT, *$3); }
           | expression O_LE expression { $$ = new BinaryOp(*$1, Op::LE, *$3); }
           | expression O_EQ expression { $$ = new BinaryOp(*$1, Op::EQ, *$3); }
           | expression O_NE expression { $$ = new BinaryOp(*$1, Op::NE, *$3); }
           | expression O_GE expression { $$ = new BinaryOp(*$1, Op::GE, *$3); }
           | expression O_GT expression { $$ = new BinaryOp(*$1, Op::GT, *$3); }
        /* */
           | expression O_AND expression { $$ = new BinaryOp(*$1, Op::AND, *$3); }
           | expression O_OR expression { $$ = new BinaryOp(*$1, Op::OR, *$3); }
           ;

expression-list-opt : %empty { $$ = new std::list<Expression*>(); }
                    | expression-list { $$ = $1; }
                    ;

expression-list : expression-list ',' expression { $1->push_back($3); $$ = $1; }
                | expression { $$ = new std::list<Expression*>(); $$->push_back($1); }
                ;

assignment : assignment-target '=' expression
             { $$ = new Assignment(*$1, *$3); }
           | assignment-target A_SUM expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::ADD, *$3))); }
           | assignment-target A_SUB expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::SUB, *$3))); }
           | assignment-target A_MUL expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::MUL, *$3))); }
           | assignment-target A_DIV expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::DIV, *$3))); }
           | assignment-target A_MOD expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::MOD, *$3))); }
           | assignment-target A_EXP expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::EXP, *$3))); }
           | assignment-target A_FDV expression
             { $$ = new Assignment(*$1, *(new BinaryOp(*$1, Op::FDV, *$3))); }
           ;

assignment-target : expression { $$ = $1; }

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

for: T_FOR T_NAME T_IN expression ':' { $$ = new ForStatement($2,*$4); }

%%

void yyerror(const char *s){
  std::cout << s << std::endl;
}
