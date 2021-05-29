#ifndef RUNIC_TOKEN_H
#define RUNIC_TOKEN_H

#include <string>

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_TOKEN_IMPLEMENTATION
    #define RUNIC_TOKEN_API __declspec(dllexport)
  #else
    #define RUNIC_TOKEN_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_TOKEN_API
#endif

using namespace std;

namespace Token {
  enum keys {
    NEW_LINE,
    COMMENT,
    WHITESPACE,
    SEMICOLON,
    COMMA,
    
    FUNCTION,
    TYPE_SPECIFIER,
    RETURN_TYPE,
    RETURN,
    
    PRINT,
    READ,

    IF,
    ELSE,

    FOR,
    WHILE,

    IDENTIFIER,
    NUMBER,
    STRING,
    
    ASSIGNMENT,
    
    PLUS,
    PLUS_EQUAL,
    MINUS,
    MINUS_EQUAL,
    TIMES,
    TIMES_EQUAL,
    DIVIDED,
    DIVIDED_EQUAL,
    MODULE,
    MODULE_EQUAL,
    POWER,
    POWER_EQUAL,
    
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,

    AND,
    OR,

    OPEN_PARENTHESES,
    CLOSE_PARENTHESES,
    OPEN_BRACKETS,
    CLOSE_BRACKETS,
    OPEN_BRACES,
    CLOSE_BRACES
  };

  string names[] = {
    "NEW_LINE",
    "COMMENT",
    "WHITESPACE",
    "SEMICOLON",
    "COMMA",

    "FUNCTION",
    "TYPE_SPECIFIER",
    "RETURN_TYPE",
    "RETURN",

    "PRINT",
    "READ",

    "IF",
    "ELSE",
    
    "FOR",
    "WHILE",

    "IDENTIFIER",
    "NUMBER",
    "STRING",

    "ASSIGNMENT",

    "PLUS",
    "PLUS_EQUAL",
    "MINUS",
    "MINUS_EQUAL",
    "TIMES",
    "TIMES_EQUAL",
    "DIVIDED",
    "DIVIDED_EQUAL",
    "MODULE",
    "MODULE_EQUAL",
    "POWER",
    "POWER_EQUAL",

    "EQUAL",
    "NOT_EQUAL",
    "LESS_THAN",
    "LESS_EQUAL",
    "GREATER_THAN",
    "GREATER_EQUAL",

    "AND",
    "OR",

    "OPEN_PARENTHESES",
    "CLOSE_PARENTHESES",
    "OPEN_BRACKETS",
    "CLOSE_BRACKETS",
    "OPEN_BRACES",
    "CLOSE_BRACES"
  };

  struct RUNIC_TOKEN_API Token {
    string lexeme;
    string name;
    int key;
    
    Token(int key, string name, string lexeme);
    Token();

    void show();
  };
};

#endif