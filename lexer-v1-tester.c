#include "lexer-v1.c"

int main(int argc, char *argv[])
{
  struct Token token;
  do
  {
    token = get_next_token();
    printf("Token type: %d, lexeme: %s\n", token.type, token.lexeme);
  } while (token.type != TOKEN_END);
  return 0;
}