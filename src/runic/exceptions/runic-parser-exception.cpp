#include <runic-parser-exception.h>

ParserException::ParserException(string message) {
  this->prefix = "[PARSER EXCEPTION]";
  this->message = message;
}
