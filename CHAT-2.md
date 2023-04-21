B:
Help me write a lexer for a programming language with the following grammar. It should be written in C.

<program> ::= <statement_list>

<statement_list> ::= <statement> | <statement> <statement_list>

<statement> ::= <variable> '=' <expression> ';' | 'print' '(' <expression> ')' ';'

<expression> ::= <term> | <term> <add_op> <expression>

<term> ::= <integer> | <variable> | '(' <expression> ')'

<add_op> ::= '+' | '-'

<integer> ::= <digit> | <integer> <digit>

<variable> ::= <letter> | <variable> <letter> | <variable> <digit>

<digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

<letter> ::= 'a' | 'b' | ... | 'z' | 'A' | 'B' | ... | 'Z'

C:
Sure, I can help you with that! Here's a basic lexer for your programming language in C:

c
Copy code
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

enum TokenType {
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

struct Token {
enum TokenType type;
char lexeme[MAX_TOKEN_LENGTH];
};

int is_add_op(char c) {
return c == '+' || c == '-';
}

int is_digit(char c) {
return isdigit(c);
}

int is_letter(char c) {
return isalpha(c);
}

int is_variable_char(char c) {
return is_letter(c) || is_digit(c);
}

int is_whitespace(char c) {
return isspace(c);
}

struct Token get_next_token() {
char c;
struct Token token;
token.type = TOKEN_UNKNOWN;
memset(token.lexeme, '\0', MAX_TOKEN_LENGTH);

    while ((c = getchar()) != EOF) {
        if (is_whitespace(c)) {
            continue;
        } else if (is_add_op(c)) {
            token.type = TOKEN_ADD_OP;
            sprintf(token.lexeme, "%c", c);
            break;
        } else if (c == '=') {
            token.type = TOKEN_ASSIGN_OP;
            sprintf(token.lexeme, "%c", c);
            break;
        } else if (c == '(') {
            token.type = TOKEN_LPAREN;
            sprintf(token.lexeme, "%c", c);
            break;
        } else if (c == ')') {
            token.type = TOKEN_RPAREN;
            sprintf(token.lexeme, "%c", c);
            break;
        } else if (c == ';') {
            token.type = TOKEN_SEMICOLON;
            sprintf(token.lexeme, "%c", c);
            break;
        } else if (is_digit(c)) {
            token.type = TOKEN_INTEGER;
            int i = 0;
            while (is_digit(c)) {
                token.lexeme[i++] = c;
                c = getchar();
            }
            ungetc(c, stdin);
            break;
        } else if (is_letter(c)) {
            int i = 0;
            while (is_variable_char(c)) {
                token.lexeme[i++] = c;
                c = getchar();
            }
            ungetc(c, stdin);
            if (strcmp(token.lexeme, "print") == 0) {
                token.type = TOKEN_PRINT;
            } else {
                token.type = TOKEN_VARIABLE;
            }
            break;
        } else {
            token.type = TOKEN_UNKNOWN;
            break;
        }
    }

    if (c == EOF) {
        token.type = TOKEN_END;
    }

    return token;

}
This lexer uses getchar() to read characters from standard input and returns a Token structure with the appropriate type and lexeme. It supports the following tokens:

TOKEN_INTEGER: a sequence of digits
TOKEN_VARIABLE: a sequence of letters and digits starting with a letter that is not the keyword "print"
TOKEN_ADD_OP: either "+" or "-"
TOKEN_ASSIGN_OP: "="
TOKEN_PRINT: the keyword "print"
TOKEN_LPAREN: "("
TOKEN_RPAREN: ")"
TOKEN_SEMICOLON: ";"
TOKEN_UNKNOWN: any other character that doesn't match any of the above
To use the lexer, you can call get_next_token() repeatedly until it returns a TOKEN_END token,

B:
What does ungetc do in this program?

C:
ungetc() is a function in C that allows you to push a character back onto the input stream. This can be useful when you've read a character from the input stream that you didn't expect to see, and you want to "undo" the read operation so that the character can be read again later.

In the lexer program I provided, ungetc() is used in two places:

When the lexer encounters a sequence of digits, it reads each digit character one at a time until it encounters a non-digit character. However, the last character it reads may be a non-digit character that belongs to the next token. To handle this case, the lexer uses ungetc() to push the last non-digit character back onto the input stream so that it can be read again later.
When the lexer encounters a sequence of letters and digits that may be a variable name, it reads each character one at a time until it encounters a non-variable character. However, the last character it reads may be a non-variable character that belongs to the next token. To handle this case, the lexer uses ungetc() to push the last non-variable character back onto the input stream so that it can be read again later.
In both cases, ungetc() is used to ensure that the next call to get_next_token() will correctly identify the next token in the input stream. Without ungetc(), the lexer might read past the boundary between two tokens and misidentify them.
