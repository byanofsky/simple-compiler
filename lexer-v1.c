#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer-v1.h"

int is_numeric(char c)
{
  return isdigit(c);
}

int is_alpha(char c)
{
  return isalpha(c);
}

int is_alphanumeric(char c)
{
  return is_alpha(c) || is_numeric(c);
}

int is_whitespace(char c)
{
  return isspace(c);
}

Token get_next_token(void)
{
  char c;
  Token token = {TOKEN_UNKNOWN, {0}};

  while ((c = getchar()) != EOF)
  {
    if (is_whitespace(c))
    {
      continue;
    }
    else if (c == '+' || c == '-')
    {
      token.type = TOKEN_ADD_OP;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == '=')
    {
      token.type = TOKEN_ASSIGN_OP;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == '(')
    {
      token.type = TOKEN_LPAREN;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == ')')
    {
      token.type = TOKEN_RPAREN;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == ';')
    {
      token.type = TOKEN_SEMICOLON;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (is_numeric(c))
    {
      token.type = TOKEN_INTEGER;
      int i = 0;
      while (is_numeric(c))
      {
        token.lexeme[i++] = c;
        c = getchar();
      }
      ungetc(c, stdin);
      break;
    }
    else if (is_alpha(c))
    {
      int i = 0;
      while (is_alphanumeric(c))
      {
        token.lexeme[i++] = c;
        c = getchar();
      }
      ungetc(c, stdin);
      if (strcmp(token.lexeme, "print") == 0)
      {
        token.type = TOKEN_PRINT;
      }
      else
      {
        token.type = TOKEN_VARIABLE;
      }
      break;
    }
    else
    {
      token.type = TOKEN_UNKNOWN;
      break;
    }
  }

  if (c == EOF)
  {
    token.type = TOKEN_END;
  }

  return token;
}
