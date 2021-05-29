#include <runic-token-table.h>

namespace Token {
  Node::Node(Token* token, int line, int column, Node* prev, Node* next) {
    this->token = token;
    this->prev = prev;
    this->next = next;
    this->line = line;
    this->column = column;
  }

  Node::Node(Token* token, int line, int column) {
    this->token = token;
    this->prev = nullptr;
    this->next = nullptr;
    this->line = line;
    this->column = column;
  }

  Node::Node() {
    this->token = nullptr;
    this->prev = nullptr;
    this->next = nullptr;
    this->line = -1;
    this->column = -1;
  }

  Table::Table() {
    this->current = nullptr;
    this->first = nullptr;
    this->last = nullptr;
  }

  void Table::push_back(Token* token, int line, int column) {
    Node* node = new Node(token, line, column);

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

  void Table::push_front(Token* token, int line, int column) {
    Node* node = new Node(token, line, column);

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

  void Table::init_current() {
    this->current = this->first;
  }

  void Table::backtrack() {
    this->current = this->current->prev;
  }

  Token* Table::get_next_token() {
    Token* token = this->current->token;
    
    this->current = this->current->next;

    return token;
  }

  void Table::show() {
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

    Node* node = this->first;
    while(node != nullptr) {
      Token* token = node->token;

      // if (!(token->key == keys::COMMENT || token->key == keys::WHITESPACE || token->key == keys::NEW_LINE)) {
        printf("|%-*s %-*s %-*d %-*d|\n",
          space, token->name.c_str(),
          space, token->lexeme.c_str(),
          space, node->line,
          space, node->column
        );
      // }

      node = node->next;
    }
    printf(" %s \n", std::string(space*columns+(columns-1), '-').c_str());
  }
};
