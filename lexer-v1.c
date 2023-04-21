#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

enum TokenType
{
  TOKEN_INTEGER,
  TOKEN_VARIABLE,
  TOKEN_ADD_OP,
  TOKEN_ASSIGN_OP,
  TOKEN_PRINT,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SEMICOLON,
  TOKEN_END,
  TOKEN_UNKNOWN
};

typedef struct Token
{
  enum TokenType type;
  char lexeme[MAX_TOKEN_LENGTH];
} Token;

int is_digit(char c)
{
  return isdigit(c);
}

int is_letter(char c)
{
  return isalpha(c);
}

int is_variable_char(char c)
{
  return is_letter(c) || is_digit(c);
}

int is_whitespace(char c)
{
  return isspace(c);
}

Token get_next_token()
{
  char c;
  Token token;
  token.type = TOKEN_UNKNOWN;
  memset(token.lexeme, '\0', MAX_TOKEN_LENGTH);

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
    else if (is_digit(c))
    {
      token.type = TOKEN_INTEGER;
      int i = 0;
      while (is_digit(c))
      {
        token.lexeme[i++] = c;
        c = getchar();
      }
      ungetc(c, stdin);
      break;
    }
    else if (is_letter(c))
    {
      int i = 0;
      while (is_variable_char(c))
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
