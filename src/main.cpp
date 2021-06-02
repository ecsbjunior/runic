#include <stdio.h>
#include <runic-lexer.h>
#include <runic-parser.h>

int main() {
  const char* source_code =
    "fn main() -> int {\n"
    " int i;\n"
    " int j = 1 + 1;\n"
    " for(int x = 0; x < 10; x+=1) {\n"
    "   print(\"hello world!\");\n"
    " }\n"
    " fn teste(int a, int b) -> void {\n"
    "   if(a > b) {\n"
    "     return a;\n"
    "   }\n"
    "   else {\n"
    "     return b;\n"
    "   }\n"
    " }\n"
    " fc teste(i, j);\n"
    "}";
  
  Lexer* lexer = new Lexer(source_code);

  lexer->token_table->show();

  Parser* parser = new Parser(lexer->token_table);

  bool wasCompiled = parser->run();

  if(wasCompiled) {
    printf("Successfully Compiled!");
  }

  return 0;
}
