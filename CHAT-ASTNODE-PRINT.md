# B:

Write a function in C that will print all ASTNodes as a tree. Here is the structure of an ASTNode
typedef struct ASTNode
{
enum NodeType type;
struct ASTNode *left;
struct ASTNode *right;
} ASTNode;

# C:

Here's a function in C that prints all `ASTNode` as a tree:

```
#include <stdio.h>
#include <stdlib.h>
#include "ASTNode.h"

void printTree(ASTNode *node, int depth) {
    if(node == NULL) {
        return;
    }

    for(int i = 0; i < depth; i++) {
        printf("\t");
    }

    printf("%d\n", node->type);

    printTree(node->left, depth+1);
    printTree(node->right, depth+1);
}

```

Here, we define a `printTree` function that takes an `ASTNode` pointer and an integer depth as arguments. The function recursively prints the `type` of each node at the given depth using a for-loop to indent the output. It then calls itself to print the left and right sub-trees of the node, increasing the depth by 1 in each call. The recursion will terminate when it reaches a `NULL` node.

Note: You'll also need to include the "ASTNode.h" header file in your program that contains the `ASTNode` structure definition.

# B:

I get this error: implicit declaration of function 'printTree' is invalid in C99 [-Werror,-Wimplicit-function-declaration]g

# C:

This error message is indicating that you have not declared the `printTree` function before calling it in your program. In C, you need to provide a function prototype before using a function.

To resolve this error, you can add a function prototype for `printTree` before you use it in your program. You can do this by adding the following line at the beginning of your code file (usually before the main function):

```
void printTree(ASTNode *node, int depth);
```

This tells the compiler that there is a function called `printTree` that takes an `ASTNode` pointer and an integer depth as arguments, and returns `void`.

Alternatively, you can move the entire definition of the `printTree` function above the point where it is called in your program. This way, the function prototype is not needed since the compiler has already seen the full definition of the function.
