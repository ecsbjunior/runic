#include <stdio.h>
#include <runic-lexer.h>

int main() {
  string source_code = "fn main() -> int { }";
  
  Lexer* lexer = new Lexer(source_code);

  lexer->token_table->show();

  return 0;
}
