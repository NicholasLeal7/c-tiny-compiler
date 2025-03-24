/*
Segunda versao do analisador sintatico.
Para gramática E ::= a | b | +EE | *EE

Para compilar no vscode use:
gcc ASDR2.c -Wall -Og -g -o ASDR2

*/

#include <stdio.h>
#include <stdlib.h>

// LEXICO - variavel global
char *buffer ="+*ab";

// SINTATICO - variavel global
char lookahead;

// E ::= a | b | +EE | *EE

// SINTATICO - prototipacao de funcao
void E(); 
void consome( char atomo );

int main(){
    printf("Analisando: %s ",buffer);
    // Antes de comecar a analise sintatica, temos que inicializar a variavel lookahead.
    lookahead = *buffer++; // obter_atomo()

    E(); // chama o simbolo inicial da gramatica
    consome('\0');

    printf("\nExpressao sintaticamente correta.\n");

    return 0;
}
// INICIO DO SINTATICO
// E ::= a | b | +EE | *EE
void E(){
    switch( lookahead ){
        case '+':
            //consome +
            consome('+');
            E();E();
            break;
        case '*':
            //consome *
            consome('*');;
            E();E();
            break;
        case 'a':
        case 'b':
            //consome a | b
            consome(lookahead);;
            break;
        default:
            consome('a');
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