#include <runic-parser.h>

Parser::Parser(TokenTable* token_table) {
  this->token_table = token_table;
}

bool Parser::run() {
  this->wasCompiled = true;
  this->current_token_table_node = this->token_table->first;

  try {
    this->entry_point();
  }
  catch(RunicException exception) {
    printf("%s\n", exception.get_message().c_str());
    this->next_statement();

    while(!this->is_end()) {
      try {
        this->command();
      }
      catch(RunicException exception) {
        printf("%s\n", exception.get_message().c_str());
        this->next_statement();
      }
    }
  }

  return this->wasCompiled;
}

void Parser::eat(int token_key) {
  Token* token = this->get_token();

  if(token->key != token_key) {
    throw ParserException (
      "Expected '" + TokenNames[token_key] +
      "' but was founded '" + token->name +
      "' at line " + std::to_string(this->current_token_table_node->prev->line) +
      " and column " + std::to_string(this->current_token_table_node->prev->column)
    );
  }
}

bool Parser::try_eat(int token_key) {
  Token* token = this->get_token();

  if(token->key == token_key) {
    this->backtrack_token_table_node();
    return true;
  }
  
  this->backtrack_token_table_node();
  return false;
}

void Parser::next_statement() {
  Token* token = this->current_token_table_node->token;
  this->next_token_table_node();
  while(!this->is_end() && token->key != TokenKeys::OPEN_BRACKETS) {
    token = this->current_token_table_node->token;
    this->next_token_table_node();
  }

  if(!this->is_end() && token->key == TokenKeys::OPEN_BRACKETS) {
    this->backtrack_token_table_node();
  }
}

bool Parser::is_end() {
  return this->current_token_table_node == nullptr;
}

Token* Parser::get_token() {
  Token* token = this->current_token_table_node->token;
  this->next_token_table_node();

  while(token->key == TokenKeys::COMMENT || token->key == TokenKeys::WHITESPACE || token->key == TokenKeys::NEW_LINE) {
    token = this->current_token_table_node->token;
    this->next_token_table_node();
  }

  return token;
}

void Parser::next_token_table_node() {
  this->current_token_table_node = this->current_token_table_node->next;
}

void Parser::backtrack_token_table_node() {
  this->current_token_table_node = this->current_token_table_node->prev;
}

void Parser::expression() {
  this->terminal();

  if(this->arithmetic_expression()) {
    this->expression();
  }
  else if(this->relational_expression()) {
    this->expression();
  }
  else if(this->logical_expression()) {
    this->expression();
  }
}

bool Parser::arithmetic_expression() {
  Token* token = this->get_token();

  if(!(token->key == TokenKeys::PLUS  || token->key == TokenKeys::MINUS   ||
       token->key == TokenKeys::TIMES || token->key == TokenKeys::DIVIDED || token->key == TokenKeys::MODULE ||
       token->key == TokenKeys::POWER)) {
    this->backtrack_token_table_node();
    return false;
  }

  return true;
}

bool Parser::relational_expression() {
  Token* token = this->get_token();

  if(!(token->key == TokenKeys::GREATER_THAN || token->key == TokenKeys::GREATER_EQUAL ||
       token->key == TokenKeys::LESS_THAN    || token->key == TokenKeys::LESS_EQUAL    ||
       token->key == TokenKeys::EQUAL        || token->key == TokenKeys::NOT_EQUAL)) {
    this->backtrack_token_table_node();
    return false;
  }

  return true;
}

bool Parser::logical_expression() {
  Token* token = this->get_token();

  if(!(token->key == TokenKeys::OR || token->key == TokenKeys::AND)) {
    this->backtrack_token_table_node();
    return false;
  }

  return true;
}

void Parser::terminal() {
  Token* token = this->get_token();

  if(!(token->key == TokenKeys::IDENTIFIER || token->key == TokenKeys::STRING || token->key == TokenKeys::NUMBER)) {
    throw ParserException (
      "Expected 'TERMINAL' but was founded '" +
      token->name +
      "' at line " + std::to_string(this->current_token_table_node->prev->line) +
      " and column " + std::to_string(this->current_token_table_node->prev->column)
    );
  }
}

void Parser::entry_point() {
  this->eat(TokenKeys::FUNCTION);
  this->eat(TokenKeys::IDENTIFIER);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::RETURN_TYPE);
  this->eat(TokenKeys::TYPE_SPECIFIER);
  this->eat(TokenKeys::OPEN_BRACKETS);
  this->command();
  this->eat(TokenKeys::CLOSE_BRACKETS);
}

void Parser::command() {
  void (Parser::*com)();

  Token* token = this->get_token();

  while(!this->is_end()) {
    com = commands[token->key];
    
    this->backtrack_token_table_node();

    (this->*com)();

    token = this->get_token();

    if(token->key == TokenKeys::CLOSE_BRACKETS) {
      if(this->current_token_table_node == nullptr) {
        this->current_token_table_node = this->token_table->last;
        break;
      }
      this->backtrack_token_table_node();
      break;
    }
  }

}

void Parser::declaration_command() {
  this->eat(TokenKeys::TYPE_SPECIFIER);
  this->declaration_initializer();
  this->eat(TokenKeys::SEMICOLON);
}

void Parser::declaration_initializer() {
  this->eat(TokenKeys::IDENTIFIER);

  if(this->try_eat(TokenKeys::ASSIGNMENT)) {
    this->eat(TokenKeys::ASSIGNMENT);
    this->expression();
  }
  // else if(this->try_eat(TokenKeys::EQUAL)) {
  //   throw ParserException("Expected 'ASSIGNMENT' but was founded 'EQUAL' at line " + std::to_string(this->lexer->line) + " and column " + std::to_string(this->lexer->column));
  // }
  
  if(this->try_eat(TokenKeys::COMMA)) {
    this->declaration_list();
  }
}

void Parser::declaration_list() {
  this->eat(TokenKeys::COMMA);
  this->declaration_initializer();
}

void Parser::assignment_command() {
  this->eat(TokenKeys::IDENTIFIER);
  this->assingnment_operator();
  this->expression();
  this->eat(TokenKeys::SEMICOLON);
}

void Parser::assignment_command_without_semicolon() {
  this->eat(TokenKeys::IDENTIFIER);
  this->assingnment_operator();
  this->expression();
}

void Parser::assingnment_operator() {
  Token* token = this->get_token();

  if(!(token->key == TokenKeys::ASSIGNMENT     ||
        token->key == TokenKeys::TIMES_EQUAL   ||
        token->key == TokenKeys::DIVIDED_EQUAL ||
        token->key == TokenKeys::MODULE_EQUAL  ||
        token->key == TokenKeys::PLUS_EQUAL    ||
        token->key == TokenKeys::MINUS_EQUAL   ||
        token->key == TokenKeys::POWER_EQUAL)) {
    throw ParserException (
      "Expected 'ASSIGNMENT EXPRESSION' but was founded '" +
      token->name +
      "' at line " + std::to_string(this->current_token_table_node->prev->line) +
      " and column " + std::to_string(this->current_token_table_node->prev->column)
    );
  }
}

void Parser::if_command() {
  this->eat(TokenKeys::IF);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->expression();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::OPEN_BRACKETS);
  this->command();
  this->eat(TokenKeys::CLOSE_BRACKETS);

  if(this->try_eat(TokenKeys::ELSE)) {
    this->eat(TokenKeys::ELSE);
    this->eat(TokenKeys::OPEN_BRACKETS);
    this->command();
    this->eat(TokenKeys::CLOSE_BRACKETS);
  }
}

void Parser::for_command() {
  this->eat(TokenKeys::FOR);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->try_eat(TokenKeys::TYPE_SPECIFIER) ? this->declaration_command() : this->assignment_command();
  this->expression();
  this->eat(TokenKeys::SEMICOLON);
  this->assignment_command_without_semicolon();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::OPEN_BRACKETS);
  this->command();
  this->eat(TokenKeys::CLOSE_BRACKETS);
}

void Parser::while_command() {
  this->eat(TokenKeys::WHILE);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->expression();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::OPEN_BRACKETS);
  this->command();
  this->eat(TokenKeys::CLOSE_BRACKETS);
}

void Parser::print_command() {
  this->eat(TokenKeys::PRINT);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->terminal();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::SEMICOLON);
}

void Parser::read_command() {
  this->eat(TokenKeys::READ);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->eat(TokenKeys::IDENTIFIER);
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::SEMICOLON);
}

void Parser::function_command() {
  this->eat(TokenKeys::FUNCTION);
  this->eat(TokenKeys::IDENTIFIER);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->function_parameters();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::RETURN_TYPE);
  this->eat(TokenKeys::TYPE_SPECIFIER);
  this->eat(TokenKeys::OPEN_BRACKETS);
  this->command();
  this->eat(TokenKeys::CLOSE_BRACKETS);
}

void Parser::function_parameters() {
  if(this->try_eat(TokenKeys::TYPE_SPECIFIER)) {
    this->function_parameter_declaration();

    if(this->try_eat(TokenKeys::COMMA)) {
      this->eat(TokenKeys::COMMA);
      this->function_parameters();
    }
  }
}

void Parser::function_parameter_declaration() {
  this->eat(TokenKeys::TYPE_SPECIFIER);
  this->eat(TokenKeys::IDENTIFIER);

  if(this->try_eat(TokenKeys::ASSIGNMENT)) {
    this->eat(TokenKeys::ASSIGNMENT);
    this->expression();
  }
}

void Parser::function_call_command() {
  this->eat(TokenKeys::FUNCTION_CALL);
  this->eat(TokenKeys::IDENTIFIER);
  this->eat(TokenKeys::OPEN_PARENTHESES);
  this->function_call_parameters();
  this->eat(TokenKeys::CLOSE_PARENTHESES);
  this->eat(TokenKeys::SEMICOLON);
}

void Parser::function_call_parameters() {
  if(this->try_eat(TokenKeys::IDENTIFIER)) {
    this->eat(TokenKeys::IDENTIFIER);

    if(this->try_eat(TokenKeys::COMMA)) {
      this->eat(TokenKeys::COMMA);
      this->function_call_parameters();
    }
  }
}

void Parser::return_command() {
  this->eat(TokenKeys::RETURN);
  this->expression();
  this->eat(TokenKeys::SEMICOLON);
}
