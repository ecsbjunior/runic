#include <runic-token-table.h>

TokenTableNode::TokenTableNode(Token* token, int line, int column, TokenTableNode* prev, TokenTableNode* next) {
  this->token = token;
  this->prev = prev;
  this->next = next;
  this->line = line;
  this->column = column;
}

TokenTableNode::TokenTableNode(Token* token, int line, int column) {
  this->token = token;
  this->prev = nullptr;
  this->next = nullptr;
  this->line = line;
  this->column = column;
}

TokenTableNode::TokenTableNode() {
  this->token = nullptr;
  this->prev = nullptr;
  this->next = nullptr;
  this->line = -1;
  this->column = -1;
}

TokenTable::TokenTable() {
  this->first = nullptr;
  this->last = nullptr;
}

void TokenTable::push_back(Token* token, int line, int column) {
  TokenTableNode* node = new TokenTableNode(token, line, column);

  if(this->first == nullptr && this->last == nullptr) {
    this->first = node;
    this->last = node;
  }
  else {
    this->last->next = node;
    node->prev = this->last;
    this->last = node;
  }
}

void TokenTable::push_front(Token* token, int line, int column) {
  TokenTableNode* node = new TokenTableNode(token, line, column);

  if(this->first == nullptr && this->last == nullptr) {
    this->first = node;
    this->last = node;
  }
  else {
    this->first->prev = node;
    node->next = this->first;
    this->first = node;
  }
}

void TokenTable::show() {
  const int space = 20;
  const int columns = 4;

  printf(" %s \n", std::string(space*columns+(columns-1), '-').c_str());
  printf(
    "|%-*s %-*s %-*s %-*s|\n",
    space, "TOKEN",
    space, "LEXEME",
    space, "LINE",
    space, "COLUMN"
  );
  printf("|%s|\n", std::string(space*columns+(columns-1), '-').c_str());

  TokenTableNode* node = this->first;
  while(node != nullptr) {
    Token* token = node->token;

    if (!(token->key == TokenKeys::COMMENT || token->key == TokenKeys::WHITESPACE || token->key == TokenKeys::NEW_LINE)) {
      printf("|%-*s %-*s %-*d %-*d|\n",
        space, token->name.c_str(),
        space, token->lexeme.c_str(),
        space, node->line,
        space, node->column
      );
    }

    node = node->next;
  }
  printf(" %s \n", std::string(space*columns+(columns-1), '-').c_str());
}
