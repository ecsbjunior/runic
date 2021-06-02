#include <runic-lexer-exception.h>

LexerException::LexerException(string message) {
  this->prefix = "[LEXER EXCEPTION]";
  this->message = message;
}