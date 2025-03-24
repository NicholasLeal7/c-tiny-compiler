/*
Analisador Sintatico Descendente Recursivo (ASDR)
Para gramática reescrita na notação EBNF
<expressão> ::=  <termo> {(‘+’  |  ‘–’ ) <termo> }
<termo> ::= <fator> { (’*’ |  ‘/’ ) <fator> }
<fator> ::= ‘a’ | ‘b’ | ‘c’ |...| ‘1’ | ‘2’ | ‘3’ |...| ‘(‘ <expressão> ‘)’

Para compilar no vscode use:
gcc ASDR4.c -Wall -Og -g -o ASDR4

Testar com valgrind com 
valgrind --leak-check=yes ./ASDR4

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// LEXICO - variavel global
char *buffer ="(1+2)*b";

// SINTATICO - variavel global
char lookahead;


// SINTATICO - prototipacao de funcao
void expressao(); 
void termo();
void fator();
void consome( char atomo );

int main(){
    printf("Infixa : %s\n",buffer);
    printf("Posfixa: ");
    // Antes de comecar a analise sintatica, temos que inicializar a variavel lookahead.
    lookahead = *buffer++; // obter_atomo()

    expressao(); // chama o simbolo inicial da gramatica
    consome('\0');

    printf("\nFim de programa.\n");

    return 0;
}
// INICIO DO SINTATICO
// <expressão> ::=  <termo>{ (‘+’  |  ‘–’ ) <termo> }
void expressao(){
    termo();
    while( lookahead == '+' || lookahead == '-'){
        char operando = lookahead;
        consome(lookahead);
        termo();
        // imprime em posordem
        printf("%c",operando);
    }

   
}
//<termo> ::= <fator> { (’*’ |  ‘/’ ) <fator> }
void termo(){
    fator();
    while(lookahead == '*'|| lookahead == '/'){
        char operando = lookahead;
        consome(lookahead);
        fator();
        // imprime em posordem
        printf("%c",operando);
    }

}
// <fator> ::= ‘a’ | ‘b’ | ‘c’ |...| ‘1’ | ‘2’ | ‘3’ |...| ‘(‘ <expressão> ‘)’
void fator(){
    if( isdigit(lookahead)||isalpha(lookahead)){
        printf("%c",lookahead);
        consome(lookahead);
    }
    else{
        consome('(');
        expressao();
        consome(')');
    }
}

void consome( char atomo ){
    if( lookahead == atomo )
        lookahead =*buffer++; // obter_atomo();
    else{
        printf("\nErro sintatico: esperado [%c] encontrado [%c]\n",atomo,lookahead);
        exit(1);
    }
}
// FIM DO SINTATICO