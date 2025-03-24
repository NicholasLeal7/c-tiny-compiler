/*
Primeira versao do analisador sintatico.
Para gramática E ::= a | b | +EE | *EE

Para compilar no vscode use:
gcc ASDR1.c -Wall -Og -g -o ASDR1
*/


#include <stdio.h>
#include <stdlib.h>

// variavel global
char *buffer ="+*aba";

void E(); // prototipacao de funcao

int main(){
    printf("Analisando: %s ",buffer);

    E(); // chama o simbolo inicial da gramatica
    if(*buffer == '\0')//testa se chegar ao final do buffer
        printf("\nsintaxe correta.\n");
    else
        printf("\nErro sintatico, nao cheguei ao final do buffer.\n");

    printf("Fim de programa.\n");

    return 0;
}
// E ::= a | b | +EE | *EE           +*a   b
void E(){
    switch( *buffer ){
        case '+':
            //consome +
            buffer++;
            E();E();
            break;
        case '*':
            //consome *
            buffer++;
            E();E();
            break;
        case 'a':
            //consome a
            buffer++;
            break;
        case 'b':
            //consome a
            buffer++;
            break;
        default:
            printf("\nErro sintatico, preciso de dois operandos.\n");
            exit(1);
    }


}
