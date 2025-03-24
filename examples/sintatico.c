#include <stdio.h>
#include <stdlib.h>
#include "sintatico.h"


// declaracao de variaveis globais
char *strAtomo[]={"ERRO","IDENTIFICADOR","NUMERO","+","*","EOS"};
TAtomo lookahead;
TInfoAtomo info_atomo;


// E ::= a | b | +EE | *EE
void E(){
    switch( lookahead ){
        case OP_SOMA:
            //consome +
            consome(OP_SOMA);
            E();E();
            break;
        case OP_MULT:
            //consome *
            consome(OP_MULT);
            E();E();
            break;
        case IDENTIFICADOR:
        case NUMERO:
            //consome a | b
            consome(lookahead);;
            break;
        default:
            consome(IDENTIFICADOR);
    }


}

void consome( TAtomo atomo ){
    if( lookahead == atomo ){
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else{
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}