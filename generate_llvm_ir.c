#include <stdio.h>
#include <stdlib.h>
#include "parser-v1.h"

int var_counter = 0;

void generate_llvm_ir(ASTNode *node)
{
  if (!node)
    return;

  StatementList *current;

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
    printf("  %%x%d = add i32 0, %d\n", var_counter++, node->data.int_value);
    break;

  case NODE_VARIABLE:
    printf("  %%x%d = load i32, i32* @%s\n", var_counter++, node->data.string_value);
    break;

  case NODE_OP:
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    if (node->data.op_type == OP_ADD)
    {
      printf("  %%x%d = add i32 %%x%d, %%x%d\n", var_counter, var_counter - 2, var_counter - 1);
    }
    else if (node->data.op_type == OP_MINUS)
    {
      printf("  %%x%d = sub i32 %%x%d, %%x%d\n", var_counter, var_counter - 2, var_counter - 1);
    }
    var_counter++;
    break;

  case NODE_ASSIGNMENT_STATEMENT:
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    printf("  store i32 %%x%d, i32* @%s\n", var_counter - 1, node->left->data.string_value);
    break;

  case NODE_PRINT_STATEMENT:
    generate_llvm_ir(node->left);
    generate_llvm_ir(node->right);
    printf("  call void @print_integer(i32 %%x%d)\n", var_counter - 1);
    break;

  default:
    fprintf(stderr, "Unsupported node type: %s\n", node_type_to_string(node->type));
    exit(1);
  }
}

int main()
{
  ASTNode *root = parse_program();

  printf("; ModuleID = 'main'\n");
  printf("declare void @print_integer(i32)\n");
  printf("define i32 @main() {\n");

  generate_llvm_ir(root);

  printf("  ret i32 0\n");
  printf("}\n");

  return 0;
}
