#ifndef RUNIC_LEXER_EXCEPTION_H
#define RUNIC_LEXER_EXCEPTION_H

#include <runic-exception.h>

struct RUNIC_EXCEPTION_API LexerException : RunicException {
	LexerException(string message);
};

#endif
