#include <stdio.h>
#include <stdlib.h>
#include "parser-v1.h"
#include <string.h>

int register_counter = 0;
int var_counter = 0;
typedef struct VarNode
{
  char *name;
  int var_count;
  struct VarNode *next;
} VarNode;

VarNode *var_list = NULL;

void add_var(char *name)
{
  VarNode *new_node = malloc(sizeof(VarNode));
  new_node->name = malloc(strlen(name) + 1);
  strcpy(new_node->name, name);
  new_node->var_count = var_counter++;
  new_node->next = var_list;
  var_list = new_node;
}

int lookup_var(char *name)
{
  VarNode *current = var_list;
  while (current != NULL)
  {
    if (strcmp(current->name, name) == 0)
    {
      return current->var_count;
    }
    current = current->next;
  }
  fprintf(stderr, "Variable %s not found", name);
  exit(1);
}

// TODO: Extract to common traversal function.
void make_var_list(ASTNode *node)
{
  if (!node)
    return;

  StatementList *current;
  switch (node->type)
  {
  case NODE_PROGRAM:
  case NODE_STATEMENT:
  case NODE_PRINT_STATEMENT:
  case NODE_ASSIGNMENT_STATEMENT:
  case NODE_EXPRESSION:
  case NODE_TERM:
  case NODE_INTEGER:
  case NODE_OP:
    make_var_list(node->left);
    make_var_list(node->right);
    break;
  case NODE_STATEMENT_LIST:
    current = node->data.statement_list;
    while (current != NULL)
    {
      make_var_list(current->statement);
      current = current->next;
    }
    break;
  case NODE_VARIABLE:
    add_var(node->data.string_value);
    break;
  default:
    fprintf(stderr, "[MakeVarList] Unknown node type: %s", node_type_to_string(node->type));
    exit(1);
  }
}

void generate_llvm_ir(ASTNode *node)
{
  if (!node)
    return;

  StatementList *current;
  int left_var_count;

  switch (node->type)
  {
  case NODE_PROGRAM:
  case NODE_STATEMENT:
  case NODE_EXPRESSION:
  case NODE_TERM:
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    break;
  case NODE_STATEMENT_LIST:
    current = node->data.statement_list;
    while (current != NULL)
    {
      generate_llvm_ir(current->statement);
      current = current->next;
    }
    break;
  case NODE_INTEGER:
    printf("  %%x%d = add i32 0, %d\n", register_counter++, node->data.int_value);
    break;

  case NODE_VARIABLE:
    printf("  %%x%d = load i32, i32* %%ptr_%d\n", register_counter++, lookup_var(node->data.string_value));
    break;

  case NODE_OP:

    left_var_count = register_counter;
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    if (node->data.op_type == OP_ADD)
    {
      printf("  %%x%d = add i32 %%x%d, %%x%d\n", register_counter, left_var_count, register_counter - 1);
    }
    else if (node->data.op_type == OP_MINUS)
    {
      printf("  %%x%d = sub i32 %%x%d, %%x%d\n", register_counter, left_var_count, register_counter - 1);
    }
    register_counter++;
    break;

  case NODE_ASSIGNMENT_STATEMENT:
    generate_llvm_ir(node->right);
    printf("  %%ptr_%d = alloca i32\n", lookup_var(node->left->data.string_value));
    printf("  store i32 %%x%d, i32* %%ptr_%d\n", register_counter - 1, lookup_var(node->left->data.string_value));
    break;

  case NODE_PRINT_STATEMENT:
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    printf("  call void @print_integer(i32 %%x%d)\n", register_counter - 1);
    break;

  default:
    fprintf(stderr, "Unsupported node type: %s\n", node_type_to_string(node->type));
    exit(1);
  }
}

int main()
{
  ASTNode *root = parse_program();
  make_var_list(root);

  printf("target triple = \"arm64-apple-macosx12.0.0\"\n");
  printf("; ModuleID = 'main'\n");
  printf("declare void @print_integer(i32)\n");
  printf("define i32 @main() {\n");

  generate_llvm_ir(root);

  printf("  ret i32 0\n");
  printf("}\n");

  return 0;
}
