#define MAX_TOKEN_LENGTH 100

typedef enum
{
  TOKEN_UNKNOWN,
  TOKEN_INTEGER,
  TOKEN_VARIABLE,
  TOKEN_PRINT,
  TOKEN_ADD_OP,
  TOKEN_ASSIGN_OP,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SEMICOLON,
  TOKEN_END
} TokenType;

typedef struct
{
  TokenType type;
  char lexeme[MAX_TOKEN_LENGTH];
  int position;
} Token;

Token get_next_token(void);

const char *token_type_to_string(TokenType type);