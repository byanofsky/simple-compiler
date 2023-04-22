To extract the creation of a node for reuse, you can create a separate function that takes the `type` parameter as an argument and returns a newly allocated `ASTNode` with the given type. Here's an example of how you could modify the code:

```
ASTNode *create_node(int type)
{
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  return node;
}

ASTNode *parse_print_statement()
{
  ASTNode *node = create_node(NODE_PRINT_STATEMENT);

  eat(TOKEN_PRINT);
  eat(TOKEN_LPAREN);
  node->left = parse_expression();
  eat(TOKEN_RPAREN);

  return node;
}
```

In the modified code, the `create_node` function creates and initializes a new `ASTNode` with the given `type`, and returns a pointer to it. The `parse_print_statement` function then calls `create_node` to create a new `ASTNode` with `NODE_PRINT_STATEMENT` type, and sets its `left` child node by calling `parse_expression`. The `eat` function is assumed to be a function that consumes the next token in the input stream.

By using the `create_node` function, you can avoid duplicating the code for initializing an `ASTNode` with a specific `type` in each parsing function that creates a node.
