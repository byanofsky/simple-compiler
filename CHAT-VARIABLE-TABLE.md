Sure, here's some example code in C that you can use to traverse the AST and create a variable lookup table:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ASTNode ASTNode;

typedef struct VarNode {
    char *name;
    int value;
    struct VarNode *next;
} VarNode;

VarNode *var_list = NULL;

enum NodeType {
    ASSIGNMENT,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    NUMBER,
    STRING
};

enum OpType {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE
};

struct ASTNode {
    enum NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;

    union {
        int int_value;
        char *string_value;
        enum OpType op_type;
        ASTNode *statement_list;
    } data;
};

void add_variable(char *name, int value) {
    VarNode *var = (VarNode *) malloc(sizeof(VarNode));
    var->name = strdup(name);
    var->value = value;
    var->next = var_list;
    var_list = var;
}

int lookup_variable(char *name) {
    VarNode *var = var_list;
    while (var != NULL) {
        if (strcmp(name, var->name) == 0) {
            return var->value;
        }
        var = var->next;
    }
    fprintf(stderr, "Error: variable '%s' not defined\n", name);
    exit(1);
}

void print_variable_table() {
    VarNode *var = var_list;
    printf("Variable table:\n");
    while (var != NULL) {
        printf("%s = %d\n", var->name, var->value);
        var = var->next;
    }
}

void traverse_ast(ASTNode *node) {
    if (node == NULL) {
        return;
    }

    if (node->type == ASSIGNMENT) {
        char *name = node->left->data.string_value;
        int value = evaluate_expression(node->right);
        add_variable(name, value);
    }

    traverse_ast(node->left);
    traverse_ast(node->right);
}

int evaluate_expression(ASTNode *node) {
    if (node == NULL) {
        fprintf(stderr, "Error: null expression\n");
        exit(1);
    }

    switch (node->type) {
        case NUMBER:
            return node->data.int_value;
        case STRING:
            return lookup_variable(node->data.string_value);
        case ADDITION:
            return evaluate_expression(node->left) + evaluate_expression(node->right);
        case SUBTRACTION:
            return evaluate_expression(node->left) - evaluate_expression(node->right);
        case MULTIPLICATION:
            return evaluate_expression(node->left) * evaluate_expression(node->right);
        case DIVISION:
            return evaluate_expression(node->left) / evaluate_expression(node->right);
        default:
            fprintf(stderr, "Error: unknown expression type %d\n", node->type);
            exit(1);
    }
}

int main() {
    ASTNode *root = (ASTNode *) malloc(sizeof(ASTNode));
    root->type = ASSIGNMENT;
    root->left = (ASTNode *) malloc(sizeof(ASTNode));
    root->left->type = STRING;
    root->left->data.string_value = strdup("x");
    root->right = (ASTNode *) malloc(sizeof(ASTNode));
    root->right->type = NUMBER;
    root->right->data.int_value = 42;

    traverse_ast(root);

    print_variable_table();

    return 0;
}
```

In this example,
