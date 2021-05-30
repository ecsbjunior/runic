#include <runic-token.h>

Token::Token(int key, string name, string lexeme) {
  this->key = key;
  this->name = name;
  this->lexeme = lexeme;
}

Token::Token () { }

void Token::show() {
  printf(
      "Key: %-15d | Name: %-15s | Lexeme: %-15s\n",
      this->key, this->name.c_str(), this->lexeme.c_str()
  );
}
