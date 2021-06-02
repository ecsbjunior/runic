#ifndef RUNIC_PARSER_H
#define RUNIC_PARSER_H

#include <map>
#include <runic-token-table.h>
#include <runic-parser-exception.h>

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_PARSER_IMPLEMENTATION
    #define RUNIC_PARSER_API __declspec(dllexport)
  #else
    #define RUNIC_PARSER_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_PARSER_API
#endif

using namespace std;

struct RUNIC_PARSER_API Parser {
  bool wasCompiled;
  TokenTable* token_table;
  TokenTableNode* current_token_table_node;

  Parser(TokenTable* token_table);

  bool run();
  void eat(int token_key);
  bool try_eat(int token_key);
  void next_statement();
  bool is_end();

  Token* get_token();
  void next_token_table_node();
  void backtrack_token_table_node();

  void expression();
  bool arithmetic_expression();
  bool relational_expression();
  bool logical_expression();

  void terminal();

  void entry_point();

  void command();

  void declaration_command();
  void declaration_initializer();
  void declaration_list();

  void assignment_command();
  void assignment_command_without_semicolon();
  void assingnment_operator();

  void if_command();

  void for_command();

  void while_command();

  void print_command();

  void read_command();

  void function_command();
  void function_parameters();
  void function_parameter_declaration();
  void function_call_command();
  void function_call_parameters();

  void return_command();
};

map<int, void (Parser::*)()> commands = {
  { TokenKeys::TYPE_SPECIFIER, &Parser::declaration_command },
  { TokenKeys::IDENTIFIER    , &Parser::assignment_command },
  { TokenKeys::IF            , &Parser::if_command },
  { TokenKeys::FOR           , &Parser::for_command },
  { TokenKeys::WHILE         , &Parser::while_command },
  { TokenKeys::PRINT         , &Parser::print_command },
  { TokenKeys::READ          , &Parser::read_command },
  { TokenKeys::FUNCTION      , &Parser::function_command },
  { TokenKeys::FUNCTION_CALL , &Parser::function_call_command },
  { TokenKeys::RETURN        , &Parser::return_command }
};

#endif
