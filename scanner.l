%{
  #include <iostream>

  #include "parser.h"
%}

%option noyywrap

%x INDENT
%%

  BEGIN(INDENT);

<INDENT>[ \t] { return T_INDENT; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

\n { BEGIN(INDENT); return T_NEWLINE; }
. { return T_OTHER; }

%%
