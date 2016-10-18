%{
  #include <iostream>
%}

%option noyywrap

%x INDENT
%%

  BEGIN(INDENT);

<INDENT>[ \t] { std::cout << "<>"; }
<INDENT>.|\n { yyless(0); BEGIN(INITIAL); }

\n { std::cout << std::endl; BEGIN(INDENT); }
. { std::cout << "'" << yytext << "'"; }

%%

int main(){
  yylex();
  return 0;
}
