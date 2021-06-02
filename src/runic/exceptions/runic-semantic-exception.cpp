#include <runic-semantic-exception.h>

SemanticException::SemanticException(string message) {
    this->prefix = "[SEMANTIC EXCEPTION]";
    this->message = message;
}
