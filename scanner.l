%{
  #include <iostream>

  #include "parser.h"

  static bool first_run = true;
%}

%option noyywrap

%x INDENT
%%

  if(first_run){
    BEGIN(INDENT);
    first_run = false;
  }

<INDENT>[ \t] { return T_INDENT; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

\n { BEGIN(INDENT); return T_NEWLINE; }
. { return T_OTHER; }

%%
