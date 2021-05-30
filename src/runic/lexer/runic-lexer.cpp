#include <runic-lexer.h>

Lexer::Lexer(string source_code) {
  this->regex_number         = regex("(\\+|\\-)?[0-9]+(.[0-9]+)?");
  this->regex_identifier     = regex("[a-zA-Z]([a-zA-Z0-9_]+)?");
  this->regex_keyword        = regex("((fn|fc|if|else|for|while|print|read|return))");
  this->regex_type_specifier = regex("((void|bool|char|string|int|float))");

  this->current = 0;
  this->line = 1;
  this->column = 0;
  
  this->source_code = source_code;

  this->token_table = new TokenTable();

  this->build_token_table();
}

void Lexer::build_token_table() {
  while(!this->is_end()) {
    Token* token = this->get_next_token();
    this->token_table->push_back(token, this->line, this->column - token->lexeme.length());
  }
}

char Lexer::next() {
  this->column++;
  return this->source_code[this->current++];
}

void Lexer::backtrack() {
  this->current--;
  this->column--;
}

bool Lexer::eat(char charactere) {
  if(!this->is_end() && this->source_code[this->current] == charactere) {
    this->column++;
    this->current++;
    return true;
  }
  return false;
}

Token* Lexer::token_keyword(Token* token) {
  if(token->lexeme == "fn") {
    token->name = TokenNames[TokenKeys::FUNCTION];
    token->key = TokenKeys::FUNCTION;
  }
  else if(token->lexeme == "if") {
    token->name = TokenNames[TokenKeys::IF];
    token->key = TokenKeys::IF;
  }
  else if(token->lexeme == "else") {
    token->name = TokenNames[TokenKeys::ELSE];
    token->key = TokenKeys::ELSE;
  }
  else if(token->lexeme == "for") {
    token->name = TokenNames[TokenKeys::FOR];
    token->key = TokenKeys::FOR;
  }
  else if(token->lexeme == "while") {
    token->name = TokenNames[TokenKeys::WHILE];
    token->key = TokenKeys::WHILE;
  }
  else if(token->lexeme == "print") {
    token->name = TokenNames[TokenKeys::PRINT];
    token->key = TokenKeys::PRINT;
  }
  else if(token->lexeme == "read") {
    token->name = TokenNames[TokenKeys::READ];
    token->key = TokenKeys::READ;
  }
  else if(token->lexeme == "return") {
    token->name = TokenNames[TokenKeys::RETURN];
    token->key = TokenKeys::RETURN;
  }

  return token;
}

Token* Lexer::token_number() {
  Token* token = new Token();

  token->lexeme += this->source_code[this->current-1];

  char charactere = this->next();
  while(!this->is_delimiter(charactere)) {
    token->lexeme += charactere;
    charactere = this->next();
  }

  this->backtrack();

  //token number
  if(regex_match(token->lexeme, regex_number)) {
    token->name = TokenNames[TokenKeys::NUMBER];
    token->key = TokenKeys::NUMBER;
    return token;
  }
  else {
    // throw LexerException(
    //   "token '" + token->lexeme +
    //   "' not regonized at line " +
    //   std::to_string(this->line) +
    //   " and column " +
    //   std::to_string(this->column)
    // );
  }

  return nullptr;
}

Token* Lexer::get_next_token() {
  char charactere = this->next();

  //token new line
  if(charactere == '\n') {
    this->line++;
    this->column = 0;
    return new Token(TokenKeys::NEW_LINE, TokenNames[TokenKeys::NEW_LINE], "NEW_LINE" );
  }

  //token whitespace
  if(charactere == ' ') {
    return new Token(TokenKeys::WHITESPACE, TokenNames[TokenKeys::WHITESPACE], " ");
  }

  //token open parentheses
  if(charactere == '(') {
    return new Token(TokenKeys::OPEN_PARENTHESES, TokenNames[TokenKeys::OPEN_PARENTHESES], "(");
  }

  //token close parentheses
  if(charactere == ')') {
    return new Token(TokenKeys::CLOSE_PARENTHESES, TokenNames[TokenKeys::CLOSE_PARENTHESES], ")");
  }

  //token open brackets
  if(charactere == '{') {
    return new Token(TokenKeys::OPEN_BRACKETS, TokenNames[TokenKeys::OPEN_BRACKETS], "{");
  }

  //token close brackets
  if(charactere == '}') {
    return new Token(TokenKeys::CLOSE_BRACKETS, TokenNames[TokenKeys::CLOSE_BRACKETS], "}");
  }

  //token comma
  if(charactere == ',') {
    return new Token(TokenKeys::COMMA, TokenNames[TokenKeys::COMMA], ",");
  }

  //token semicolon
  if(charactere == ';') {
    return new Token(TokenKeys::SEMICOLON, TokenNames[TokenKeys::SEMICOLON], ";");
  }

  //token string
  if(charactere == '"') {
    Token* token = new Token(TokenKeys::STRING, TokenNames[TokenKeys::STRING], "\"");

    charactere = this->next();
    while(!this->is_end() && charactere != '"') {
      token->lexeme += charactere;
      charactere = this->next();
    }

    if(charactere == '"') {
      token->lexeme += "\"";
      return token;
    }
  }

  if(charactere == '+') {
    //token plus equal
    if(this->eat('=')) {
      return new Token(TokenKeys::PLUS_EQUAL, TokenNames[TokenKeys::PLUS_EQUAL], "+=");
    }

    //token number
    if(this->is_number(this->source_code[this->current])) {
      return this->token_number();
    }

    return new Token(TokenKeys::PLUS, TokenNames[TokenKeys::PLUS], "+");
  }

  if(charactere == '-') {
    //token return type
    if(this->eat('>')) {
      return new Token(TokenKeys::RETURN_TYPE, TokenNames[TokenKeys::RETURN_TYPE], "->");
    }
    
    //token minus equal
    if(this->eat('=')) {
      return new Token(TokenKeys::MINUS_EQUAL, TokenNames[TokenKeys::MINUS_EQUAL], "-=");
    }

    //token number
    if(this->is_number(this->source_code[this->current])) {
      return this->token_number();
    }

    //token minus
    return new Token(TokenKeys::MINUS, TokenNames[TokenKeys::MINUS], "-");
  }

  if(charactere == '*') {
    //token times equal
    if(this->eat('=')) {
      return new Token(TokenKeys::TIMES_EQUAL, TokenNames[TokenKeys::TIMES_EQUAL], "*=");
    }

    //token times
    return new Token(TokenKeys::TIMES, TokenNames[TokenKeys::TIMES], "*");
  }

  if(charactere == '/') {
    //token line comment
    if(this->eat('/')) {
      Token* token = new Token(TokenKeys::COMMENT, TokenNames[TokenKeys::COMMENT], "//");

      while(charactere = this->next()) {
        token->lexeme += charactere;
        if(charactere == '\n') {
          this->line++;
          this->column = 0;
          return token;
        }
      }
    }

    //token multiline coment
    if(this->eat('*')) {
      Token* token = new Token(TokenKeys::COMMENT, TokenNames[TokenKeys::COMMENT], "/*");

      while(charactere = this->next()) {
        token->lexeme += charactere;

        if(charactere == '\n') {
          this->line++;
          this->column = 0;
        }

        if(charactere == '*' && this->eat('/')) {
          token->lexeme += '/';
          return token;
        }
      }
    }

    //token divided equal
    if(this->eat('=')) {
      return new Token(TokenKeys::DIVIDED_EQUAL, TokenNames[TokenKeys::DIVIDED_EQUAL], "/=");
    }

    //token divided
    return new Token(TokenKeys::DIVIDED, TokenNames[TokenKeys::DIVIDED], "/");
  }

  if(charactere == '%') {
    //token module equal
    if(this->eat('=')) {
      return new Token(TokenKeys::MODULE_EQUAL, TokenNames[TokenKeys::MODULE_EQUAL], "%=");
    }

    //token module
    return new Token(TokenKeys::MODULE, TokenNames[TokenKeys::MODULE], "%");
  }

  if(charactere == '^') {
    //token power equal
    if(this->eat('=')) {
      return new Token(TokenKeys::POWER_EQUAL, TokenNames[TokenKeys::POWER_EQUAL], "^=");
    }

    //token power
    return new Token(TokenKeys::POWER, TokenNames[TokenKeys::POWER], "^");
  }

  if(charactere == '>') {
    //token greater equal
    if(this->eat('=')) {
      return new Token(TokenKeys::GREATER_EQUAL, TokenNames[TokenKeys::GREATER_EQUAL], ">=");
    }

    //token greater
    return new Token(TokenKeys::GREATER_THAN, TokenNames[TokenKeys::GREATER_THAN], ">");
  }

  if(charactere == '<') {
    //token less equal
    if(this->eat('=')) {
      return new Token(TokenKeys::LESS_EQUAL, TokenNames[TokenKeys::LESS_EQUAL], "<=");
    }

    //token less
    return new Token(TokenKeys::LESS_THAN, TokenNames[TokenKeys::LESS_THAN], "<");
  }

  if(charactere == '=') {
    //token equal
    if(this->eat('=')) {
      return new Token(TokenKeys::EQUAL, TokenNames[TokenKeys::EQUAL], "==");
    }

    //token assignment
    return new Token(TokenKeys::ASSIGNMENT, TokenNames[TokenKeys::ASSIGNMENT], "=");
  }

  if(charactere == '!') {
    //token not equal
    if(this->eat('=')) {
      return new Token(TokenKeys::NOT_EQUAL, TokenNames[TokenKeys::NOT_EQUAL], "!=");
    }
  }

  if(charactere == '|') {
    //token or
    if(this->eat('|')) {
      return new Token(TokenKeys::OR, TokenNames[TokenKeys::OR], "||");
    }
  }

  if(charactere == '&') {
    //token and
    if(this->eat('&')) {
      return new Token(TokenKeys::AND, TokenNames[TokenKeys::AND], "&&");
    }
  }

  if(this->is_number(charactere)) {
    return token_number();
  }

  Token* token = new Token();

  token->lexeme += charactere;

  charactere = this->next();
  while(!this->is_delimiter(charactere)) {
    token->lexeme += charactere;
    charactere = this->next();
  }

  this->backtrack();

  //token type specifier
  if(regex_match(token->lexeme, regex_type_specifier)) {
    token->key = TokenKeys::TYPE_SPECIFIER;
    token->name = TokenNames[TokenKeys::TYPE_SPECIFIER];
    return token;
  }
  
  //token keyword
  if(regex_match(token->lexeme, regex_keyword)) {
    return token_keyword(token);
  }
  
  //token identifier
  if(regex_match(token->lexeme, regex_identifier)) {
    token->key = TokenKeys::IDENTIFIER;
    token->name = TokenNames[TokenKeys::IDENTIFIER];
    return token;
  }

  // throw LexerException(
  //   "token '" + token->lexeme +
  //   "' not regonized at line " +
  //   to_string(this->line) +
  //   " and column " +
  //   to_string(this->column)
  // );

  return nullptr;
}

bool Lexer::is_delimiter(char charactere) {
  return charactere == ' ' ||
         charactere == '+' || charactere == '-' ||
         charactere == '*' || charactere == '/' ||
         charactere == '%' || charactere == '^' ||
         charactere == '(' || charactere == ')' ||
         charactere == '{' || charactere == '}' ||
         charactere == ',' || charactere == ';' ||
         charactere == '>' || charactere == '<' ||
         charactere == '!' || charactere == '=' ||
         charactere == '|' || charactere == '&' ||
         charactere == '\n';
}

bool Lexer::is_number(char charactere) {
  return charactere >= '0' && charactere <= '9';
}

bool Lexer::is_letter(char charactere) {
  return (charactere >= 'a' && charactere <= 'z') || (charactere >= 'A' && charactere <= 'Z');
}

bool Lexer::is_end() {
  return this->current >= source_code.length();
}
