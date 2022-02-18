#include <stdio.h>

#include "lexer.h"

int main() {
  yyin = stdin;
  yylex();
  yylex_destroy();
  return 0;
}
