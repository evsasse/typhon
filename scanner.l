%{
  #include <iostream>

  #include "parser.h"

  static bool first_run = true;
%}

%option noyywrap

%x INDENT
%x COMMENT MLCOMMENT
%%

  if(first_run){
    BEGIN(INDENT);
    first_run = false;
  }

<INDENT>[ \t] { return T_INDENT; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

<COMMENT>. { }
<COMMENT>\n { yyless(0); BEGIN(INITIAL); }

<MLCOMMENT>.|\n { }
<MLCOMMENT>###+ { BEGIN(INITIAL); }

\n { BEGIN(INDENT); return T_NEWLINE; }
# { BEGIN(COMMENT); }
###+ { BEGIN(MLCOMMENT); }

[ \t] { /* ignore spaces and tabs */}

[;] { return yytext[0]; }

stt { return T_STATEMENT; }

. { std::cerr << "err[" << yytext << "]"; }

%%
