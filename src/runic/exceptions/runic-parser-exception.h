#ifndef RUNIC_PARSER_EXCEPTION_H
#define RUNIC_PARSER_EXCEPTION_H

#include <runic-exception.h>

struct RUNIC_EXCEPTION_API ParserException : RunicException {
	ParserException(string message);
};

#endif
