#ifndef RUNIC_TOKEN_TABLE_H
#define RUNIC_TOKEN_TABLE_H

#include <runic-token.h>

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_TOKEN_TABLE_IMPLEMENTATION
    #define RUNIC_TOKEN_TABLE_API __declspec(dllexport)
  #else
    #define RUNIC_TOKEN_TABLE_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_TOKEN_TABLE_API
#endif

struct RUNIC_TOKEN_TABLE_API TokenTableNode {
  int line, column;
  Token* token;
  TokenTableNode* prev;
  TokenTableNode* next;

  TokenTableNode(Token* token, int line, int column, TokenTableNode* prev, TokenTableNode* next);
  TokenTableNode(Token* token, int line, int column);
  TokenTableNode();
};

struct RUNIC_TOKEN_TABLE_API TokenTable {
  TokenTableNode* current;
  TokenTableNode* first;
  TokenTableNode* last;

  TokenTable();

  void push_back(Token* token, int line, int column);
  void push_front(Token* token, int line, int column);

  void backtrack();
  void init_current();
  
  Token* get_next_token();

  void show();
};

#endif
