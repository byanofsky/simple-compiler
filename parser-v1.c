#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer-v1.h"
#include "parser-v1.h"

Token current_token;
ASTNode *parse_expression();

char *node_type_to_string(enum NodeType type)
{
  switch (type)
  {
  case NODE_PROGRAM:
    return "NODE_PROGRAM";
  case NODE_STATEMENT_LIST:
    return "NODE_STATEMENT_LIST";
  case NODE_STATEMENT:
    return "NODE_STATEMENT";
  case NODE_PRINT_STATEMENT:
    return "NODE_PRINT_STATEMENT";
  case NODE_ASSIGNMENT_STATEMENT:
    return "NODE_ASSIGNMENT_STATEMENT";
  case NODE_EXPRESSION:
    return "NODE_EXPRESSION";
  case NODE_TERM:
    return "NODE_TERM";
  case NODE_INTEGER:
    return "NODE_INTEGER";
  case NODE_VARIABLE:
    return "NODE_VARIABLE";
  case NODE_OP:
    return "NODE_OP";
  default:
    return "UNKNOWN";
  }
}

void consume_token(TokenType type)
{
  if (current_token.type == type)
  {
    current_token = get_next_token();
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\" at position %d. Expected \"%s\".\n", token_type_to_string(current_token.type), current_token.position, token_type_to_string(type));
    exit(1);
  }
}

ASTNode *new_ast_node(enum NodeType type)
{
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  return node;
}

ASTNode *parse_integer()
{
  ASTNode *node = new_ast_node(NODE_INTEGER);
  Token token = current_token;
  consume_token(TOKEN_INTEGER);

  node->data.int_value = atoi(token.lexeme);

  return node;
}

ASTNode *parse_variable()
{
  ASTNode *node = new_ast_node(NODE_VARIABLE);
  Token token = current_token;
  consume_token(TOKEN_VARIABLE);

  node->data.string_value = strcpy(malloc(strlen(token.lexeme) + 1), token.lexeme);

  return node;
}

ASTNode *parse_term()
{
  ASTNode *node = new_ast_node(NODE_TERM);
  Token token = current_token;

  if (token.type == TOKEN_INTEGER)
  {
    node->left = parse_integer();
  }
  else if (token.type == TOKEN_VARIABLE)
  {
    node->left = parse_variable();
  }
  else if (token.type == TOKEN_LPAREN)
  {
    consume_token(TOKEN_LPAREN);
    node->left = parse_expression();
    consume_token(TOKEN_RPAREN);
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\" at position %d. Expected \"%s\",\"%s\",\"%s\".\n",
            token_type_to_string(current_token.type),
            current_token.position,
            token_type_to_string(TOKEN_INTEGER),
            token_type_to_string(TOKEN_VARIABLE),
            token_type_to_string(TOKEN_LPAREN));
    exit(1);
  }

  return node;
}

ASTNode *parse_expression()
{
  ASTNode *node = new_ast_node(NODE_EXPRESSION);
  ASTNode *term_node = parse_term();

  if (current_token.type == TOKEN_ADD_OP)
  {
    ASTNode *add_node = new_ast_node(NODE_OP);

    if (strcmp(current_token.lexeme, "+") == 0)
    {
      add_node->data.op_type = OP_ADD;
    }
    else if (strcmp(current_token.lexeme, "-") == 0)
    {
      add_node->data.op_type = OP_MINUS;
    }
    else
    {
      fprintf(stderr, "Error: Unexpected add_ops value \"%s\"\n", current_token.lexeme);
      exit(1);
    }

    consume_token(TOKEN_ADD_OP);

    add_node->left = term_node;
    add_node->right = parse_expression();
    node->left = add_node;
  }
  else
  {
    node->left = term_node;
  }

  return node;
}

ASTNode *parse_print_statement()
{
  ASTNode *node = new_ast_node(NODE_PRINT_STATEMENT);

  consume_token(TOKEN_PRINT);
  consume_token(TOKEN_LPAREN);
  node->left = parse_expression();
  consume_token(TOKEN_RPAREN);

  return node;
}

ASTNode *parse_assignment_statement()
{
  ASTNode *node = new_ast_node(NODE_ASSIGNMENT_STATEMENT);

  node->left = parse_variable();
  consume_token(TOKEN_ASSIGN_OP);
  node->right = parse_expression();

  return node;
}

ASTNode *parse_statement()
{
  ASTNode *node = new_ast_node(NODE_STATEMENT);
  if (current_token.type == TOKEN_PRINT)
  {
    node->left = parse_print_statement();
  }
  else if (current_token.type == TOKEN_VARIABLE)
  {
    node->left = parse_assignment_statement();
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\" at position %d. Expected \"%s\",\"%s\".\n",
            token_type_to_string(current_token.type),
            current_token.position,
            token_type_to_string(TOKEN_PRINT),
            token_type_to_string(TOKEN_VARIABLE));
    exit(1);
  }

  consume_token(TOKEN_SEMICOLON);

  return node;
}

ASTNode *parse_statement_list()
{
  ASTNode *node = new_ast_node(NODE_STATEMENT_LIST);
  StatementList *list_head = NULL, *current = NULL;
  list_head = current = malloc(sizeof(StatementList));
  current->statement = parse_statement();
  current->next = NULL;

  while (current_token.type != TOKEN_END)
  {
    current->next = malloc(sizeof(StatementList));
    current = current->next;
    current->statement = parse_statement();
    current->next = NULL;
  }

  node->data.statement_list = list_head;

  return node;
}

ASTNode *parse_program()
{
  ASTNode *node = new_ast_node(NODE_PROGRAM);
  current_token = get_next_token();
  node->left = parse_statement_list();

  consume_token(TOKEN_END);

  return node;
}