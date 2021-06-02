#ifndef RUNIC_SEMANTIC_EXCEPTION_H
#define RUNIC_SEMANTIC_EXCEPTION_H

#include <runic-exception.h>

struct RUNIC_EXCEPTION_API SemanticException : RunicException {
	SemanticException(string message);
};

#endif
