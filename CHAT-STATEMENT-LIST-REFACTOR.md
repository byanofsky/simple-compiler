Using an array or a linked list to represent the list of statements in the AST node would be a better way to represent the statement_list. This way, the ASTNode will have a pointer to the first element in the list, and each element in the list will have a pointer to the next element.

Here is an example implementation using a linked list:

```
typedef struct StatementList {
  ASTNode *statement;
  struct StatementList *next;
} StatementList;

ASTNode *parse_statement_list()
{
  ASTNode *node;
  StatementList *list_head = NULL, *current = NULL;

  node = create_node(NODE_STATEMENT_LIST);

  list_head = current = malloc(sizeof(StatementList));
  current->statement = parse_statement();
  current->next = NULL;

  while (current_token.type != TOKEN_END)
  {
    current = current->next = malloc(sizeof(StatementList));
    current->statement = parse_statement();
    current->next = NULL;
  }

  node->statement_list = list_head;
  return node;
}
```

In this implementation, the `node` has a pointer to the head of the `statement_list`, which is a linked list of `StatementList` nodes. Each `StatementList` node has a pointer to the corresponding statement `ASTNode` and a pointer to the next `StatementList` node in the list.

This implementation allows for more efficient traversal and modification of the statement list, and also enables more flexible handling of the statement list in later stages of the compilation process.
