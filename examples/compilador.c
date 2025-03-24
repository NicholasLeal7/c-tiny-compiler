#include <stdio.h>
#include "lexico.h"
#include "sintatico.h"
/*
Versao compilador lexico e sintatico em arquivos separados
E ::= numero | identificador | +EE | *EE

Para compilar no vscode use:
gcc compilador.c lexico.c sintatico.c -Wall -Og -g -o compilador

Testar com valgrind com 
valgrind --leak-check=yes ./compilador 
*/
// declaradas como glogais ao projeto no sintatico.c
extern TAtomo lookahead;
extern TInfoAtomo info_atomo;

int main(){
    //printf("Analisando: %s",entrada);
    // Antes de comecar a analise sintatica, temos que inicializar a variavel lookahead.
    info_atomo = obter_atomo();
    lookahead = info_atomo.atomo;

    E(); // chama o simbolo inicial da gramatica
    consome(EOS); // testa se chegou ao final da string

    printf("\nExpressao sintaticamente correta.\n");

    return 0;
}