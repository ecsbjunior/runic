#include <runic-exception.h>

string RunicException::get_message() {
  return this->prefix + " " + this->message;
}
