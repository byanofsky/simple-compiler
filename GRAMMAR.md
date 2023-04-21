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
