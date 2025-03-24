#ifndef	_LEXICO_H
#define _LEXICO_H

//######## DECLARACOES LEXICO 
// definicoes dos atomos
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    OP_SOMA,
    OP_MULT,
    EOS // fim de buffer
}TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
}TInfoAtomo;

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_num();
TInfoAtomo reconhece_id();

#endif