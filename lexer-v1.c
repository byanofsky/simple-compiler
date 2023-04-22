#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer-v1.h"

int current_position = 0;

const char *token_type_to_string(TokenType type)
{
  switch (type)
  {
  case TOKEN_UNKNOWN:
    return "Unknown";
  case TOKEN_INTEGER:
    return "Integer";
  case TOKEN_VARIABLE:
    return "Variable";
  case TOKEN_PRINT:
    return "Print";
  case TOKEN_ADD_OP:
    return "Add Operator";
  case TOKEN_ASSIGN_OP:
    return "Assign Operator";
  case TOKEN_LPAREN:
    return "Left Parenthesis";
  case TOKEN_RPAREN:
    return "Right Parenthesis";
  case TOKEN_SEMICOLON:
    return "Semicolon";
  case TOKEN_END:
    return "End";
  default:
    return "Invalid TokenType";
  }
}

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
      current_position++;
      continue;
    }
    else if (c == '+' || c == '-')
    {
      token.type = TOKEN_ADD_OP;
      token.position = current_position++;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == '=')
    {
      token.type = TOKEN_ASSIGN_OP;
      token.position = current_position++;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == '(')
    {
      token.type = TOKEN_LPAREN;
      token.position = current_position++;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == ')')
    {
      token.type = TOKEN_RPAREN;
      token.position = current_position++;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (c == ';')
    {
      token.type = TOKEN_SEMICOLON;
      token.position = current_position++;
      sprintf(token.lexeme, "%c", c);
      break;
    }
    else if (is_numeric(c))
    {
      token.type = TOKEN_INTEGER;
      token.position = current_position;
      int i = 0;
      while (is_numeric(c))
      {
        token.lexeme[i++] = c;
        c = getchar();
      }
      current_position += i;
      ungetc(c, stdin);
      break;
    }
    else if (is_alpha(c))
    {
      int i = 0;
      int start_position = current_position;
      while (is_alphanumeric(c))
      {
        token.lexeme[i++] = c;
        c = getchar();
      }
      ungetc(c, stdin);
      current_position += i;
      if (strcmp(token.lexeme, "print") == 0)
      {
        token.type = TOKEN_PRINT;
        token.position = start_position;
      }
      else
      {
        token.type = TOKEN_VARIABLE;
        token.position = start_position;
      }
      break;
    }
    else
    {
      token.type = TOKEN_UNKNOWN;
      token.position = current_position;
      break;
    }
  }

  if (c == EOF)
  {
    token.type = TOKEN_END;
    token.position = current_position;
  }

  return token;
}
