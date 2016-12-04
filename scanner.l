%{
  #include <iostream>
  #include <stdexcept>


  #include "ast.h"
  #include "parser.h"

  static bool first_run = true;

  #define MAX_INCLUDE_DEPTH 10
  YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
  int include_stack_ptr = 0;

  extern int HYBRID_INDENT_COUNT;
  enum IndentType { NOT_DEFINED, SPACE, TAB, HYBRID };
  static IndentType INDENT_TYPE = NOT_DEFINED;
  static int curHybridIndent = 0;
%}

%option noyywrap

integer [0-9]+
float ([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+)
bool "True"|"False"
name [a-zA-Z_][a-zA-Z0-9_]*

%x INDENT HYBRIDINDENT
%x COMMENT MLCOMMENT
%x IMPORT
%x SIMPLESTRING DOUBLESTRING
%%

  if(first_run){
    BEGIN(INDENT);
    first_run = false;
    if(HYBRID_INDENT_COUNT > 0){
      INDENT_TYPE = HYBRID;
    }
  }

<INDENT>" " { if(INDENT_TYPE == NOT_DEFINED){ INDENT_TYPE = SPACE; }else if(INDENT_TYPE == TAB){ std::cerr << "lexical error, mixed Space with Tabs" << std::endl; } return T_INDENT; }
<INDENT>\t { if(INDENT_TYPE == HYBRID){ curHybridIndent = 1; yyless(0); BEGIN(HYBRIDINDENT); }else if(INDENT_TYPE == NOT_DEFINED){ INDENT_TYPE = TAB; }else if(INDENT_TYPE == SPACE){ std::cerr << "lexical error, mixed Tab with Spaces" << std::endl; } return T_INDENT; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

<HYBRIDINDENT>\t { if(curHybridIndent < HYBRID_INDENT_COUNT){ curHybridIndent++; yyless(0); return T_INDENT; }else{ BEGIN(INDENT); } }

<COMMENT>. { /* ignores every char until a new line */ }
<COMMENT>\n { yyless(0); BEGIN(INITIAL); }

<MLCOMMENT>.|\n { /* ignores every char until ###+ */ }
<MLCOMMENT>###+ { BEGIN(INITIAL); }

<SIMPLESTRING>\' { BEGIN(INITIAL); }
<SIMPLESTRING>\n { yyless(0); BEGIN(INITIAL); }
<SIMPLESTRING>\\\' { yylval.val_str = strdup("'"); return T_CHAR; }
<SIMPLESTRING>\\\\ { yylval.val_str = strdup("\\"); return T_CHAR; }
<SIMPLESTRING>\\n { yylval.val_str = strdup("\n"); return T_CHAR; }
<SIMPLESTRING>\\\n { }
<SIMPLESTRING>. { yylval.val_str = strdup(yytext); return T_CHAR; }

<DOUBLESTRING>\" { BEGIN(INITIAL); }
<DOUBLESTRING>\n { yyless(0); BEGIN(INITIAL); }
<DOUBLESTRING>\\\" { yylval.val_str = strdup("'"); return T_CHAR; }
<DOUBLESTRING>\\\\ { yylval.val_str = strdup("\\"); return T_CHAR; }
<DOUBLESTRING>\\n { yylval.val_str = strdup("\n"); return T_CHAR; }
<DOUBLESTRING>\\\n { }
<DOUBLESTRING>. { yylval.val_str = strdup(yytext); return T_CHAR; }

\n { BEGIN(INDENT); return T_NEWLINE; }
# { BEGIN(COMMENT); }
###+ { BEGIN(MLCOMMENT); }

\' { BEGIN(SIMPLESTRING); return T_STRING; }
\" { BEGIN(DOUBLESTRING); return T_STRING; }

[ \t] { /* ignore spaces and tabs */}

"def " { return T_DEF; }
"return " { return T_RETURN; }

"if " { return T_IF; /*TODO if(), elif(), while(). without space*/ }
"elif " { return T_ELIF; }
"else" { return T_ELSE; }
"while " { return T_WHILE; }
"for " { return T_FOR; }
" in " { return T_IN; }

"pass" { return T_PASS; }

"import " { return T_IMPORT; }

[,;:\(\)\=\[\]] { return yytext[0]; }

[\+\-\*\/\%] { return yytext[0]; }
"**" { return O_EXP; }
"//" { return O_FDV; }

"+=" { return A_SUM; }
"-=" { return A_SUB; }
"*=" { return A_MUL; }
"/=" { return A_DIV; }
"%=" { return A_MOD; }
"**=" { return A_EXP; }
"//=" { return A_FDV; }

"<" { return O_LT; }
"<=" { return O_LE; }
"==" { return O_EQ; }
"!=" { return O_NE; }
">=" { return O_GE; }
">" { return O_GT; }

"and"|"&&" { return O_AND; }
"or"|"||" { return O_OR; }
"not"|"!" { return O_NOT; }

{integer} { yylval.val_int = std::atoi(yytext); return L_INT; }
{float} { yylval.val_float = std::atof(yytext); return L_FLOAT; }
{bool} { yylval.val_bool = strcmp("True",yytext) == 0; return L_BOOL; }
{name} { yylval.val_str = strdup(yytext); return T_NAME; }

. { std::cerr << "lexical error, '" << yytext << "' was unexpected" << std::endl; }

<<EOF>> {
  if ( --include_stack_ptr < 0 ){
    yyterminate();
  }else{
    yy_delete_buffer( YY_CURRENT_BUFFER );
    yy_switch_to_buffer( include_stack[include_stack_ptr] );
  }
}

%%

void import_file(const char *filename){
  /* from flex manual
     http://dinosaur.compilertools.net/flex/flex_12.html */
  if ( include_stack_ptr >= MAX_INCLUDE_DEPTH ){
    fprintf( stderr, "Includes nested too deeply, infinite recursion ?" );
    exit( 1 );
  }

  include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;

  yyin = fopen( filename, "r" );

  if ( ! yyin ){
    /*--include_stack_ptr;
    yy_delete_buffer( YY_CURRENT_BUFFER );
    yy_switch_to_buffer( include_stack[include_stack_ptr] );
    throw std::runtime_error("ImportError: No module named "+std::string(filename));*/
  }

  yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );

  BEGIN(INITIAL);
}
