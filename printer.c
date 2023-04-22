#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printer.h"

void print_node(ASTNode *node, int depth);

void print_indent(int depth)
{
  for (int i = 0; i < depth; i++)
  {
    printf("  ");
  }
}

void print_op(ASTNode *node, int depth)
{
  print_indent(depth);
  printf("%s ", node_type_to_string(node->type));
  if (node->data.op_type == OP_ADD)
  {
    printf("+\n");
  }
  else
  {
    printf("-\n");
  }
}

void print_integer(ASTNode *node, int depth)
{
  print_indent(depth);
  printf("%s %d\n", node_type_to_string(node->type), node->data.int_value);
}

void print_variable(ASTNode *node, int depth)
{
  print_indent(depth);
  printf("%s %s\n", node_type_to_string(node->type), node->data.string_value);
}

void print_node(ASTNode *node, int depth)
{
  if (node == NULL)
  {
    return;
  }

  switch (node->type)
  {
  case NODE_PROGRAM:
  case NODE_STATEMENT_LIST:
  case NODE_STATEMENT:
  case NODE_PRINT_STATEMENT:
  case NODE_ASSIGNMENT_STATEMENT:
  case NODE_EXPRESSION:
  case NODE_TERM:
    print_indent(depth);
    printf("%s\n", node_type_to_string(node->type));
    break;
  case NODE_INTEGER:
    print_integer(node, depth);
    break;
  case NODE_VARIABLE:
    print_variable(node, depth);
    break;
  case NODE_OP:
    print_op(node, depth);
    break;
  default:
    fprintf(stderr, "[PrintNode] Unknown node type: %d\n", node->type);
    exit(1);
    break;
  }

  if (node->left != NULL)
  {
    print_node(node->left, depth + 1);
  }
  if (node->right != NULL)
  {
    print_node(node->right, depth + 1);
  }
}
