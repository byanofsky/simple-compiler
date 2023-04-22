typedef struct ASTNode ASTNode;
typedef struct StatementList StatementList;

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

char *node_type_to_string(enum NodeType type);

ASTNode *parse_program();