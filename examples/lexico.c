#include <stdio.h>
#include <ctype.h>  //isupper
#include <string.h> // strncpy
#include <stdlib.h> // atof

#include "lexico.h"

// declaracao de variaveis globais
int contaLinha = 1;
char *entrada = "+ * 1.2  var1 ";

TInfoAtomo obter_atomo(){
    TInfoAtomo info_atomo;

    info_atomo.atomo = ERRO;

    // eliminar delimitadores
    while(*entrada == ' '|| 
          *entrada == '\n'||
          *entrada == '\r'||
          *entrada == '\t'){
        if(*entrada == '\n')
            contaLinha++;
        entrada++;
    }
    if(*entrada == '\0'){
        entrada++;
        info_atomo.atomo = EOS;
    }
    if( *entrada == '+'){
        entrada++;
        info_atomo.atomo = OP_SOMA;
    }
    else if( *entrada == '*'){
        entrada++;
        info_atomo.atomo = OP_MULT;
    }
    else if( isdigit(*entrada)){
        info_atomo = reconhece_num();
    }
    else if( islower(*entrada)){
        info_atomo = reconhece_id();
    }
    
    info_atomo.linha = contaLinha;
    return info_atomo;
}

// NUMERO -> DIGITO+.DIGITO+
TInfoAtomo reconhece_num(){
    TInfoAtomo info_num;
    char str_num[10];
    char *ini_num;

    info_num.atomo = ERRO;

    ini_num = entrada;
//q0:
    if(isdigit(*entrada)){
        entrada++; // consome digito
        goto q1;
    }
    
    //se terminar a palavra retorna 0, pois 
    //nao eh estado final
    return info_num; // erro lexico

q1:
    if(isdigit(*entrada)){
        entrada++; // consome digito
        goto q1;
    }
    if(*entrada == '.'){
        entrada++; // consome .
        goto q2;
    }
    //[outro]
    return info_num; // erro lexico
q2:
    if(isdigit(*entrada)){
        entrada++; // consome digito
        goto q3;
    }
    //[outro]
    return info_num; // erro lexico
q3:
    if(isdigit(*entrada)){
        entrada++; // consome digito
        goto q3;
    }
    // se vier letra
    if(isalpha(*entrada)){
        return info_num; // erro lexico
    }
    info_num.atomo = NUMERO;
    //man strncpy
    strncpy(str_num,ini_num,entrada - ini_num);
    //finalizar a string
    str_num[entrada - ini_num]='\0';
    info_num.atributo_numero = atof(str_num);
    return info_num;
}
// IDENTIFICADOR -> LETRA_MINUSCULA(LETRA_MINUSCULA|DIGITO)*

TInfoAtomo reconhece_id(){
    TInfoAtomo info_id;
    info_id.atomo = ERRO;

    if(islower(*entrada)){
        entrada ++;// consome letra minuscula
        goto q1;
    }
    return info_id;
q1:
    if(islower(*entrada) || isdigit(*entrada)){
        entrada ++;// consome letra minuscula
        goto q1;
    }
    if(isupper(*entrada))
        return info_id;
    
    info_id.atomo = IDENTIFICADOR;
    return info_id;

}