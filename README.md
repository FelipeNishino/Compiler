# Compiler

## Funcionalidades

- [&#128994;] Variáveis (Normal, Constantes);
- [&#10060;] Escopo local;
- [&#128994;] Tipos (Int, Float, String, Bool);
- [&#128994;] Sequências de controle e laços de repetição (if, else, for, while, do...while);
- [&#10060;] Funções;

## Build

Para buildar o programa:
```
make
```

## Execução

Para rodar o programa:
```
./nl-interpret.out [nome-arquivo] [flags]
```

Ao final da execução o interpretador vai printar todas as variáveis no escopo global.
Este repositório contém um arquivo de exemplo chamado `input.txt`.


### Flags disponíveis

- `-l`: Roda somente o lexer e printa todos os tokens reconhecidos.

## Gramática

Como as funções ainda não foram implementadas, o interpretador entra pela regra de produção <block>.

```
program ::= function
function ::= signature block
signature ::= identifier '(' argument* ')' ('->' type)?
argument ::= identifier type_annotation
type_annotation ::= ':' type
block ::=  '{' (statement|control_sequence)+ '}'

statement ::= (control_sequence|simple_statement)

simple_statement ::= (declaration|assignment|return)
declaration ::= declaration_directive identifier type_annotation assignment_expression? 
declaration_directive ::= ('let'|'var')
assignment ::= identifier assignment_expression
assignment_expression ::= '=' expression

control_sequence ::= (std_while|do_while|for|if)
condition ::= '(' expression ')'
std_while ::= 'while' condition block
do_while ::= 'do' block 'while' condition
for ::= 'for' '(' statement? ';' expression ';' statement ')' block
if ::= 'if' condition (block|statement) else?
else ::= 'else' block

return ::= 'return' expression

expression ::= simple_expression (relational_operator simple_expression)?
simple_expression ::= unary_operator? term (lesser_precedence_operator term)*
term ::= factor (higher_precedence_operator factor)*
factor ::= identifier | string | number | bool | grouped_expression | '!' factor
grouped_expression ::= '(' expression ')'

relational_operator ::= ('<' | '>' | '<=' | '>=' | '==' | '!=') 
unary_operator ::= ('+' | '-' | '!') 
lesser_precedence_operator ::= ('+' | '-' | '||')
higher_precedence_operator ::= ('/' | '%' | '*' | '**' | '&&')


number ::= sign? (integer|float)
integer ::= [0-9]+
float ::= [0-9]* '.' [0-9]* ('e' (lesser_precedence_binary_arithmetic_operator)? [0-9]+)?

string ::= '"' [\w]* '"'
bool ::= (False|True)

identifier ::= ([a-z]|[A-Z]) ([a-z]|[A-Z]|[0-9])*
type ::= ('Int'|'Float'|'Bool'|'String'|'Null')
```