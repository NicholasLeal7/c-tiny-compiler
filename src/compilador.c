#include <stdio.h>
#include <ctype.h>
#include <string.h> // strncpy
#include <stdlib.h> // atof

// structs e enum
typedef enum
{
    ERRO,
    IDENTIFICADOR,
    CHARCONST,
    INTCONST,
    VOID,
    MAIN,
    ABRE_PAR,
    FECHA_PAR,
    ABRE_CHAVES,
    FECHA_CHAVES,
    PVIR,
    INT,
    CHAR,
    VIR,
    RECEBE,
    READINT,
    WRITEINT,
    IF,
    ELSE,
    WHILE,
    OR,
    AND,
    MENOR,
    MENOR_IGUAL,
    IGUAL,
    DIFERENTE,
    MAIOR,
    MAIOR_IGUAL,
    OP_SOMA,
    OP_SUBT,
    OP_MULT,
    OP_DIV,
    EOS
} Tatomo;

typedef struct
{
    Tatomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[16];
} TInfoAtomo;

// LEXICO
char *buffer;
char *entrada = "a 'a' '0' 0xABC123+0xA3/AAA\n\n\n\n\n\n\nAAAAA ASASASSSSS";
char strOperadores[] = {};
char *strAtomo[] = {
    "ERRO",          // 0
    "IDENTIFICADOR", // 1
    "CHARCONST",     // 2
    "INTCONST",      // 3
    "VOID",          // 4
    "MAIN",          // 5
    "ABRE_PAR",      // 6
    "FECHA_PAR",     // 7
    "ABRE_CHAVES",   // 8
    "FECHA_CHAVES",  // 9
    "PVIR",          // 10
    "INT",           // 11
    "CHAR",          // 12
    "VIR",           // 13
    "RECEBE",        // 14
    "READINT",       // 15
    "WRITEINT",      // 16
    "IF",            // 17
    "ELSE",          // 18
    "WHILE",         // 19
    "OR",            // 20
    "AND",           // 21
    "MENOR",         // 22
    "MENOR_IGUAL",   // 23
    "IGUAL",         // 24
    "DIFERENTE",     // 25
    "MAIOR",         // 26
    "MAIOR_IGUAL",   // 27
    "OP_SOMA",       // 28
    "OP_SUBT",       // 29
    "OP_MULT",       // 30
    "OP_DIV",        // 31
    "EOS"            // 32
};
int linha = 1;
Tatomo lookahead;
TInfoAtomo info_atomo;
// SINTATICO

// declaração de função
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_charconst();
TInfoAtomo reconhece_intconst();

int main()
{
    // apresentação
    printf("Compilador TINY-C\n\n");

    //******** INICIO - variaveis e leitura do arquivo *******
    FILE *arquivo = fopen("entrada.txt", "r");
    char conteudo[10000], c;
    int i = 0, tam = sizeof(conteudo) - 1;

    if (arquivo == NULL)
    {
        printf("Arquivo não encontrado.");
        return 0;
    }

    while ((c = fgetc(arquivo)) != EOF && i < tam)
    {
        conteudo[i++] = c;
    }

    // buffer = conteudo;
    // printf("Conteudo:\n %s", buffer);
    fclose(arquivo);
    //******** FIM - variaveis e leitura do arquivo *******

    //******** INICIO - compilador *******
    do
    {
        info_atomo = obter_atomo();
        printf("#  %d: %s\n", linha, strAtomo[info_atomo.atomo]);
    } while (info_atomo.atomo != ERRO && info_atomo.atomo != EOS);
    // lookahead = info_atomo.atomo;

    // chamar regra inicial da gramática
    //******** FIM - compilador *******

    return 0;
}

TInfoAtomo obter_atomo()
{
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;

    // delimitadores e retorno de carro
    while (*entrada == ' ' ||
           *entrada == '\n' ||
           *entrada == '\t' ||
           *entrada == '\r')
    {
        if (*entrada == '\n')
        {
            linha++;
        }
        entrada++;
    }

    // verifica se o buffer já foi esvaziado
    if (*entrada == '\0')
    {
        info_atomo.atomo = EOS;
    }

    // identificação dos outros variáveis, operadores, palavras reservadas e afins
    if (*entrada == '0')
    {
        info_atomo = reconhece_intconst();
    }
    else if (*entrada == '\'')
    {
        info_atomo = reconhece_charconst();
    }
    else if (isalpha(*entrada) || *entrada == '_')
    {
        info_atomo = reconhece_id();
    }
    else if (*entrada == '+')
    {
        entrada++;
        info_atomo.atomo = OP_SOMA;
    }
    else if (*entrada == '-')
    {
        entrada++;
        info_atomo.atomo = OP_SUBT;
    }
    else if (*entrada == '*')
    {
        entrada++;
        info_atomo.atomo = OP_MULT;
    }
    else if (*entrada == '/')
    {
        entrada++;
        info_atomo.atomo = OP_DIV;
    }

    return info_atomo;
}

TInfoAtomo reconhece_id()
{
    TInfoAtomo info_id;
    info_id.atomo = ERRO;
    int maximo_char = 0;

    // q0;
    if (isalpha(*entrada) || *entrada == '_')
    {
        entrada++;
        maximo_char++;
        goto q1;
    }
    return info_id;

q1:
    if (maximo_char > 15)
    {
        return info_id;
    }

    if (isdigit(*entrada) || isalpha(*entrada) || *entrada == '_')
    {
        entrada++;
        maximo_char++;
        goto q1;
    }

    info_id.atomo = IDENTIFICADOR;
    return info_id;
}

TInfoAtomo reconhece_charconst()
{
    TInfoAtomo info_charconst;
    info_charconst.atomo = ERRO;

    // q0:
    if (*entrada == '\'')
    {
        entrada++;
        goto q1;
    }
    return info_charconst;

q1:
    if (isdigit(*entrada) || isalpha(*entrada))
    {
        entrada++;
        goto q2;
    }
    return info_charconst;

q2:
    if (*entrada == '\'')
    {
        entrada++;
    }
    else
    {
        return info_charconst;
    }

    info_charconst.atomo = CHARCONST;
    return info_charconst;
}

TInfoAtomo reconhece_intconst()
{
    TInfoAtomo info_num;
    info_num.atomo = ERRO;

    // q0:
    if (*entrada == '0')
    {
        entrada++;
        goto q1;
    }
    return info_num;

q1:
    if (*entrada == 'x')
    {
        entrada++;
        goto q2;
    }
    return info_num;

q2:
    if (isdigit(*entrada) ||
        *entrada == 'A' ||
        *entrada == 'B' ||
        *entrada == 'C' ||
        *entrada == 'D' ||
        *entrada == 'E' ||
        *entrada == 'F')
    {
        entrada++;
        goto q3;
    }
    return info_num;

q3:
    if (isdigit(*entrada) ||
        *entrada == 'A' ||
        *entrada == 'B' ||
        *entrada == 'C' ||
        *entrada == 'D' ||
        *entrada == 'E' ||
        *entrada == 'F')
    {
        entrada++;
        goto q3;
    }
    else if (isalpha(*entrada))
    {
        return info_num;
    }

    info_num.atomo = INTCONST;
    return info_num;
}
