%{
  #include <iostream>

  #include "ast.h"
  #include "parser.h"

  static bool first_run = true;
%}

%option noyywrap

integer [0-9]+
float ([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+)
bool "True"|"False"
name [a-zA-Z_][a-zA-Z0-9_]*

%x INDENT
%x COMMENT MLCOMMENT
%%

  if(first_run){
    BEGIN(INDENT);
    first_run = false;
  }

<INDENT>[ \t] { return T_INDENT; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

<COMMENT>. { /* ignores every char until a new line */ }
<COMMENT>\n { yyless(0); BEGIN(INITIAL); }

<MLCOMMENT>.|\n { /* ignores every char until ###+ */ }
<MLCOMMENT>###+ { BEGIN(INITIAL); }

\n { BEGIN(INDENT); return T_NEWLINE; }
# { BEGIN(COMMENT); }
###+ { BEGIN(MLCOMMENT); }

[ \t] { /* ignore spaces and tabs */}

[;\+\-\*\/\(\)\=] { return yytext[0]; }
"**" { return O_EXP; }
"//" { return O_FDV; }

"+=" { return A_SUM; }

{integer} { yylval.val_int = std::atoi(yytext); return L_INT; }
{float} { yylval.val_float = std::atof(yytext); return L_FLOAT; }
{bool} { yylval.val_bool = strcmp("True",yytext) == 0; return L_BOOL; }
{name} { yylval.val_str = strdup(yytext); return T_NAME; }

. { std::cerr << "lex-err['" << yytext << "']" << std::endl; }

%%
