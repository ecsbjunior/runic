#ifndef RUNIC_TOKEN_TABLE_H
#define RUNIC_TOKEN_TABLE_H

#include <runic-token.h>

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_TOKEN_IMPLEMENTATION
    #define RUNIC_TOKEN_API __declspec(dllexport)
  #else
    #define RUNIC_TOKEN_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_TOKEN_API
#endif

namespace Token {
  struct RUNIC_TOKEN_API Node {
    int line, column;
    Token* token;
    Node* prev;
    Node* next;

    Node(Token* token, int line, int column, Node* prev, Node* next);
    Node(Token* token, int line, int column);
    Node();
  };

  struct RUNIC_TOKEN_API Table {
    Node* current;
    Node* first;
    Node* last;

    Table();

    void push_back(Token* token, int line, int column);
    void push_front(Token* token, int line, int column);

    void backtrack();
    void init_current();
    
    Token* get_next_token();

    void show();
  };
};

#endif
