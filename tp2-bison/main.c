#include "main.h"

#include <string.h>

#include "parser.h"
#include "lexer.h"

double constant(const char *name) {
  if (strcmp(name, "PI") == 0) {
    return 3.14159265358979323846;
  }

  fprintf(stderr, "Unknown constant: '%s'\n", name);
  return 0.0;
}

int main() {
  int ret = yyparse();
  yylex_destroy();
  return ret;
}
