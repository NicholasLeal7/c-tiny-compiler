/*
Implementacao do mini analisador lexico conforme slides da aula Aula3(miniLexico).pdf
 IDENTIFICADOR -> LETRA_MINUSCULA(LETRA_MINUSCULA|DIGITO)*
 NUMERO -> DIGITO+.DIGITO+

Para compilar no vscode use:
gcc miniLexico.c -Wall -Og -g -o miniLexico

// teste de memoria
https://diveintosystems.org/book/C3-C_debug/valgrind.html

Rode o Valgrind com 
valgrind --leak-check=yes ./miniLexico 

caso não esteja instalado use
sudo apt update
sudo apt install valgrind
sudo apt upgrade
 
        
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h> // strncpy
#include <stdlib.h> // atof

// definicoes dos atamos
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    EOS // fim de buffer
}TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
}TInfoAtomo;

// declaracao de variaveis globais
char *strAtomo[]={"ERRO","IDENTIFICADOR","NUMERO","EOS"};
int contaLinha = 1;
char *entrada = "   \nvar1   12.4 1.1 1000.01\n\rvar1\n\n\n\nv vA";



// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_num();
TInfoAtomo reconhece_id();

int main(void){
    TInfoAtomo info_atm;
    do{
        info_atm = obter_atomo();

        printf("%03d# %s | ", info_atm.linha,strAtomo[info_atm.atomo]);
        if(info_atm.atomo == NUMERO)
            printf("%.2f",info_atm.atributo_numero);
        
        printf("\n");

    }while(info_atm.atomo != ERRO && info_atm.atomo != EOS);
    printf("fim de analise lexica\n");
}
// implementacao da funcao
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
        info_atomo.atomo = EOS;
    }
    if( isdigit(*entrada)){
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