#include <stdlib.h>
#include "lexer-v1.c"
#include <stdio.h>

typedef struct ASTNode ASTNode;

ASTNode *parse_expression();

enum NodeType
{
  NODE_PROGRAM,
  NODE_STATEMENT_LIST,
  NODE_STATEMENT,
  NODE_PRINT_STATEMENT,
  NODE_ASSIGNMENT_STATEMENT,
  NODE_EXPRESSION,
  NODE_TERM,
  NODE_INTEGER,
  NODE_VARIABLE,
  NODE_OP
};

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

enum OpType
{
  OP_ADD,
  OP_MINUS,
};

typedef struct StatementList
{
  ASTNode *statement;
  struct StatementList *next;
} StatementList;

typedef struct ASTNode
{
  enum NodeType type;
  struct ASTNode *left;
  struct ASTNode *right;

  union
  {
    int int_value;

    char *string_value;

    enum OpType op_type;

    StatementList *statement_list;
  } data;
} ASTNode;

Token current_token;

ASTNode *create_node(enum NodeType type)
{
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void printNode(ASTNode *node, int depth)
{
  if (node == NULL)
  {
    return;
  }

  for (int i = 0; i < depth; i++)
  {
    printf(" ");
  }

  // Print current node.
  switch (node->type)
  {
  case NODE_PROGRAM:
  case NODE_STATEMENT_LIST:
  case NODE_STATEMENT:
  case NODE_PRINT_STATEMENT:
  case NODE_ASSIGNMENT_STATEMENT:
  case NODE_EXPRESSION:
  case NODE_TERM:
    printf("%s\n", node_type_to_string(node->type));
    break;
  case NODE_INTEGER:
    printf(" %s %d\n", node_type_to_string(node->type), node->data.int_value);
    break;
  case NODE_VARIABLE:
    printf(" %s %s\n", node_type_to_string(node->type), node->data.string_value);
    break;
  case NODE_OP:
    printf(" %s %c\n", node_type_to_string(node->type), node->data.op_type == OP_ADD ? '+' : '-');
    break;
  default:
    fprintf(stderr, "[PrintNode] Unknown node type: %d\n", node->type);
    exit(1);
    break;
  }

  StatementList *current;
  // Handle next nodes.
  switch (node->type)
  {
  case NODE_PROGRAM:
  case NODE_STATEMENT:
  case NODE_PRINT_STATEMENT:
  case NODE_ASSIGNMENT_STATEMENT:
  case NODE_EXPRESSION:
  case NODE_TERM:
  case NODE_INTEGER:
  case NODE_VARIABLE:
  case NODE_OP:
    printNode(node->left, depth + 1);
    printNode(node->right, depth + 1);
    break;
  case NODE_STATEMENT_LIST:
    current = node->data.statement_list;
    while (current != NULL)
    {
      printNode(current->statement, depth + 1);
      current = current->next;
    }
    break;
  default:
    fprintf(stderr, "[HandleNext] Unknown node type: %d\n", node->type);
    exit(1);
    break;
  }
  // printNode(node->left, depth + 1);
  // printNode(node->right, depth + 1);
}

void eat(enum TokenType type)
{
  if (current_token.type == type)
  {
    current_token = get_next_token();
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\". Expect \"%i\".\n", current_token.lexeme, current_token.type);
    exit(1);
  }
}

ASTNode *parse_integer()
{
  ASTNode *node;
  node = create_node(NODE_INTEGER);

  if (current_token.type == TOKEN_INTEGER)
  {
    node->data.int_value = atoi(current_token.lexeme);
    eat(TOKEN_INTEGER);
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\"\n", current_token.lexeme);
    exit(1);
  }

  return node;
}

ASTNode *parse_variable()
{
  ASTNode *node;
  node = create_node(NODE_VARIABLE);

  if (current_token.type == TOKEN_VARIABLE)
  {
    node->data.string_value = strcpy(malloc(strlen(current_token.lexeme) + 1), current_token.lexeme);
    eat(TOKEN_VARIABLE);
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\"\n", current_token.lexeme);
    exit(1);
  }

  return node;
}

ASTNode *parse_term()
{
  ASTNode *node;
  node = create_node(NODE_TERM);

  if (current_token.type == TOKEN_INTEGER)
  {
    node->left = parse_integer();
  }
  else if (current_token.type == TOKEN_VARIABLE)
  {
    node->left = parse_variable();
  }
  else if (current_token.type == TOKEN_LPAREN)
  {
    eat(TOKEN_LPAREN);
    node->left = parse_expression();
    eat(TOKEN_RPAREN);
  }
  else
  {
    fprintf(stderr, "Error: Unexpected token \"%s\"\n", current_token.lexeme);
    exit(1);
  }

  return node;
}

ASTNode *parse_expression()
{
  ASTNode *node;
  node = create_node(NODE_EXPRESSION);

  ASTNode *term_node = parse_term();

  if (current_token.type == TOKEN_ADD_OP)
  {
    ASTNode *add_node;
    add_node = malloc(sizeof(ASTNode));
    add_node->type = NODE_OP;

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

    eat(TOKEN_ADD_OP);

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
  ASTNode *node;
  node = create_node(NODE_PRINT_STATEMENT);

  eat(TOKEN_PRINT);
  eat(TOKEN_LPAREN);
  node->left = parse_expression();
  eat(TOKEN_RPAREN);

  return node;
}

ASTNode *parse_assignment_statement()
{
  ASTNode *node;
  node = create_node(NODE_ASSIGNMENT_STATEMENT);

  node->left = parse_variable();
  eat(TOKEN_ASSIGN_OP);
  node->right = parse_expression();

  return node;
}

ASTNode *parse_statement()
{
  ASTNode *node;
  node = create_node(NODE_STATEMENT);

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
    // TODO: More descriptive error message.
    // TODO: Extract this into a function.
    // Include character position in error message.
    fprintf(stderr, "Error: Unexpected token \"%s\"\n", current_token.lexeme);
    exit(1);
  }

  eat(TOKEN_SEMICOLON);

  return node;
}

ASTNode *parse_statement_list()
{
  ASTNode *node;
  node = create_node(NODE_STATEMENT_LIST);

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
  ASTNode *node;
  node = create_node(NODE_PROGRAM);

  current_token = get_next_token();
  node->left = parse_statement_list();

  eat(TOKEN_END);

  return node;
}

int main(int argc, char *argv[])
{
  ASTNode *node = parse_program();

  printNode(node, 0);

  return 0;
}