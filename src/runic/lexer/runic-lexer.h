#ifndef RUNIC_LEXER_H
#define RUNIC_LEXER_H

#include <regex>
#include <string>
#include <runic-token.h>
#include <runic-token-table.h>
#include <runic-lexer-exception.h>

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_LEXER_IMPLEMENTATION
    #define RUNIC_LEXER_API __declspec(dllexport)
  #else
    #define RUNIC_LEXER_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_LEXER_API
#endif

using namespace std;

struct RUNIC_LEXER_API Lexer {
  int current;
  int line, column;

  string source_code;
  
  TokenTable* token_table;

  regex regex_number;
  regex regex_identifier;
  regex regex_keyword;
  regex regex_type_specifier;

  Lexer(string source_code);

  void build_token_table();

  char next();
  void backtrack();
  bool eat(char charactere);

  Token* token_keyword(Token* token);
  Token* token_number();

  Token* get_next_token();

  bool is_delimiter(char charactere);
  bool is_number(char charactere);
  bool is_letter(char charactere);
  bool is_end();
};

#endif
