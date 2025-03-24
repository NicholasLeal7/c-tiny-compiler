/*
Primeira versao do analisador sintatico.
Para gramática
E ::= numero | identificador | +EE | *EE

Para compilar no vscode use:
gcc ASDR3.c -Wall -Og -g -o ASDR3

Testar com valgrind com
valgrind --leak-check=yes ./ASDR3
*/

// LEXICO
#include <ctype.h>  //isupper
#include <string.h> // strncpy
#include <stdlib.h> // atof

// SINTATICO
#include <stdio.h>
#include <stdlib.h>

// ######## DECLARACOES LEXICO
//  definicoes dos atomos
typedef enum
{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    OP_SOMA,
    OP_MULT,
    EOS // fim de buffer
} TAtomo;

typedef struct
{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
} TInfoAtomo;

// declaracao de variaveis globais
char *strAtomo[] = {"ERRO", "IDENTIFICADOR", "NUMERO", "+", "*", "EOS"};
int contaLinha = 1;
char *entrada = "+ * 1.2  var1 ";

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_num();
TInfoAtomo reconhece_id();
// ######## FIM DECLARACOES LEXICO

// ######## DECLARACOES SINTATICO
//  variavel global
TAtomo lookahead;
TInfoAtomo info_atomo;

// E ::= a | b | +EE | *EE

// SINTATICO - prototipacao de funcao
void E();
void consome(TAtomo atomo);

// FIM DECLARCOES SINTATICO

int main()
{
    printf("Analisando: %s", entrada);
    // Antes de comecar a analise sintatica, temos que inicializar a variavel lookahead.
    info_atomo = obter_atomo();
    lookahead = info_atomo.atomo;

    E();          // chama o simbolo inicial da gramatica
    consome(EOS); // testa se chegou ao final da string

    printf("\nExpressao sintaticamente correta.\n");

    return 0;
}
// ######## IMPLEMENTACAO LEXICO
TInfoAtomo obter_atomo()
{
    TInfoAtomo info_atomo;

    info_atomo.atomo = ERRO;

    // eliminar delimitadores
    while (*entrada == ' ' ||
           *entrada == '\n' ||
           *entrada == '\r' ||
           *entrada == '\t')
    {
        if (*entrada == '\n')
            contaLinha++;
        entrada++;
    }
    if (*entrada == '\0')
    {
        entrada++;
        info_atomo.atomo = EOS;
    }
    if (*entrada == '+')
    {
        entrada++;
        info_atomo.atomo = OP_SOMA;
    }
    else if (*entrada == '*')
    {
        entrada++;
        info_atomo.atomo = OP_MULT;
    }
    else if (isdigit(*entrada))
    {
        info_atomo = reconhece_num();
    }
    else if (islower(*entrada))
    {
        info_atomo = reconhece_id();
    }

    info_atomo.linha = contaLinha;
    return info_atomo;
}

// NUMERO -> DIGITO+.DIGITO+
TInfoAtomo reconhece_num()
{
    TInfoAtomo info_num;
    char str_num[10];
    char *ini_num;

    info_num.atomo = ERRO;

    ini_num = entrada;
    // q0:
    if (isdigit(*entrada))
    {
        entrada++; // consome digito
        goto q1;
    }

    // se terminar a palavra retorna 0, pois
    // nao eh estado final
    return info_num; // erro lexico

q1:
    if (isdigit(*entrada))
    {
        entrada++; // consome digito
        goto q1;
    }
    if (*entrada == '.')
    {
        entrada++; // consome .
        goto q2;
    }
    //[outro]
    return info_num; // erro lexico
q2:
    if (isdigit(*entrada))
    {
        entrada++; // consome digito
        goto q3;
    }
    //[outro]
    return info_num; // erro lexico
q3:
    if (isdigit(*entrada))
    {
        entrada++; // consome digito
        goto q3;
    }
    // se vier letra
    if (isalpha(*entrada))
    {
        return info_num; // erro lexico
    }
    info_num.atomo = NUMERO;
    // man strncpy
    strncpy(str_num, ini_num, entrada - ini_num);
    // finalizar a string
    str_num[entrada - ini_num] = '\0';
    info_num.atributo_numero = atof(str_num);
    return info_num;
}

// IDENTIFICADOR -> LETRA_MINUSCULA(LETRA_MINUSCULA|DIGITO)*

TInfoAtomo reconhece_id()
{
    TInfoAtomo info_id;
    info_id.atomo = ERRO;

    if (islower(*entrada))
    {
        entrada++; // consome letra minuscula
        goto q1;
    }
    return info_id;
q1:
    if (islower(*entrada) || isdigit(*entrada))
    {
        entrada++; // consome letra minuscula
        goto q1;
    }
    if (isupper(*entrada))
        return info_id;

    info_id.atomo = IDENTIFICADOR;
    return info_id;
}
// ######## FIM IMPLEMENTACAO LEXICO

// ######## IMPLEMENTACAO SINTATICO
//  E ::= a | b | +EE | *EE
//"+ * 1.2  var1 "
void E()
{
    switch (lookahead)
    {
    case OP_SOMA:
        // consome +
        consome(OP_SOMA);
        E();
        E();
        break;
    case OP_MULT:
        // consome *
        consome(OP_MULT);
        E();
        E();
        break;
    case IDENTIFICADOR:
    case NUMERO:
        // consome a | b
        consome(lookahead);
        ;
        break;
    default:
        consome(IDENTIFICADOR);
    }
}

void consome(TAtomo atomo)
{
    if (lookahead == atomo)
    {
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else
    {
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n", strAtomo[atomo], strAtomo[lookahead]);
        exit(1);
    }
}
// ######## FIM IMPLEMENTACAO SINTATICO