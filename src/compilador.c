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
    COMENTARIO,
    EOS
} Tatomo;

typedef struct
{
    Tatomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[15];
} TInfoAtomo;

// LEXICO
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
    "COMENTARIO",    // 32
    "EOS"            // 33
};
int linha = 1;
Tatomo lookahead;
TInfoAtomo info_atomo;

// declaração de função do léxico
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_charconst();
TInfoAtomo reconhece_intconst();
TInfoAtomo reconhece_operadores();
TInfoAtomo reconhece_sinais();
TInfoAtomo reconhece_palavras_reservadas(char *str);
TInfoAtomo reconhece_comentarios();

// declaração de função do sintático
void consome(Tatomo atomo);
void program();
void compound_smt();
void var_decl();
void type_specifier();
void var_decl_list();
void variable_id();
void stmt();
void assig_stmt();
void cond_stmt();
void while_stmt();
void expr();
void conjunction();
void comparision();
void relation();
void sum();
void term();
void factor();

// declaração de funções auxiliares que funcionam como o isdigit e isalpha só que para operadores e sinais
int eh_operador(char operador);
int eh_sinal(char sinal);

// palavra em teste
char *entrada = "/*programa le a o maior*/void //main ( void ) {\nasas//asa\nwj//ej";

int main()
{
    //******** INICIO - compilador *******
    do
    {
        info_atomo = obter_atomo();
        printf("#  %d: %s", linha, strAtomo[info_atomo.atomo]);
        linha += info_atomo.linha;

        if (info_atomo.atomo == INTCONST)
        {
            printf(" | %d", info_atomo.atributo_numero);
        }
        else if (info_atomo.atomo == IDENTIFICADOR || info_atomo.atomo == CHARCONST)
        {
            printf(" | %s", info_atomo.atributo_ID);
        }

        printf("\n");
    } while (info_atomo.atomo != ERRO && info_atomo.atomo != EOS);
    //******** FIM - compilador *******
    return 0;
}

TInfoAtomo obter_atomo()
{
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    int linhaAuxiliar = 0;

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
    if (*entrada == '/')
    {
        // ele retorna token de comentários ou o operador de divisão
        info_atomo = reconhece_comentarios();
        linhaAuxiliar = info_atomo.linha;
    }
    else if (*entrada == '0')
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
        info_atomo = reconhece_palavras_reservadas(info_atomo.atributo_ID);
    }
    else if (eh_operador(*entrada))
    {
        info_atomo = reconhece_operadores();
    }
    else if (eh_sinal(*entrada))
    {
        info_atomo = reconhece_sinais();
    }

    info_atomo.linha = linhaAuxiliar;
    return info_atomo;
}

// reconhece - máquinas de estados
TInfoAtomo reconhece_id()
{
    TInfoAtomo info_id;
    info_id.atomo = ERRO;
    int maximo_char = 0;
    char str_id[15];
    char *ini_id = entrada;

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

    // armazenando a var
    strncpy(str_id, ini_id, entrada - ini_id);
    str_id[entrada - ini_id] = '\0';
    strcpy(info_id.atributo_ID, str_id);

    info_id.atomo = IDENTIFICADOR;
    return info_id;
}

TInfoAtomo reconhece_charconst()
{
    TInfoAtomo info_charconst;
    info_charconst.atomo = ERRO;
    char str_char[15];
    char *ini_char = entrada;

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

    // armazenando o char
    strncpy(str_char, ini_char + 1, 1);
    str_char[entrada - ini_char] = '\0';
    strcpy(info_charconst.atributo_ID, str_char);

    info_charconst.atomo = CHARCONST;
    return info_charconst;
}

TInfoAtomo reconhece_intconst()
{
    TInfoAtomo info_num;
    info_num.atomo = ERRO;
    int maximo_num = 0;
    char str_num[10];
    char *ini_num = entrada, *endptr;

    // q0:
    if (*entrada == '0')
    {
        entrada++;
        maximo_num++;
        goto q1;
    }
    return info_num;

q1:
    if (*entrada == 'x')
    {
        entrada++;
        maximo_num++;
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
        maximo_num++;
        goto q3;
    }
    return info_num;

q3:
    if (maximo_num > 10)
    {
        return info_num;
    }

    if (isdigit(*entrada) ||
        *entrada == 'A' ||
        *entrada == 'B' ||
        *entrada == 'C' ||
        *entrada == 'D' ||
        *entrada == 'E' ||
        *entrada == 'F')
    {
        entrada++;
        maximo_num++;
        goto q3;
    }
    else if (isalpha(*entrada))
    {
        return info_num;
    }

    // armazenando o num
    strncpy(str_num, ini_num, entrada - ini_num);
    str_num[entrada - ini_num] = '\0';
    strcpy(info_num.atributo_ID, str_num);
    info_num.atributo_numero = strtol(info_num.atributo_ID, &endptr, 16);

    info_num.atomo = INTCONST;
    return info_num;
}

// reconhece - apenas consome
TInfoAtomo reconhece_operadores()
{
    TInfoAtomo info_operador;
    info_operador.atomo = ERRO;

    if (*entrada == '+')
    {
        entrada++;
        info_operador.atomo = OP_SOMA;
    }
    else if (*entrada == '-')
    {
        entrada++;
        info_operador.atomo = OP_SUBT;
    }
    else if (*entrada == '*')
    {
        entrada++;
        info_operador.atomo = OP_MULT;
    }
    else if (*entrada == '/')
    {
        entrada++;
        info_operador.atomo = OP_DIV;
    }
    else if (*entrada == '&')
    {
        entrada++;
        if (*entrada == '&')
        {
            entrada++;
            info_operador.atomo = AND;
        }
        else
        {
            info_operador.atomo = ERRO;
        }
    }
    else if (*entrada == '|')
    {
        entrada++;
        if (*entrada == '|')
        {
            entrada++;
            info_operador.atomo = OR;
        }
        else
        {
            info_operador.atomo = ERRO;
        }
    }
    else if (*entrada == '=')
    {
        entrada++;
        if (*entrada == '=')
        {
            entrada++;
            info_operador.atomo = IGUAL;
        }
        else
        {
            info_operador.atomo = RECEBE;
        }
    }
    else if (*entrada == '!')
    {
        entrada++;
        if (*entrada == '=')
        {
            entrada++;
            info_operador.atomo = DIFERENTE;
        }
        else
        {
            info_operador.atomo = ERRO;
        }
    }
    else if (*entrada == '>')
    {
        entrada++;
        if (*entrada == '=')
        {
            entrada++;
            info_operador.atomo = MAIOR_IGUAL;
        }
        else
        {
            info_operador.atomo = MAIOR;
        }
    }
    else if (*entrada == '<')
    {
        entrada++;
        if (*entrada == '=')
        {
            entrada++;
            info_operador.atomo = MENOR_IGUAL;
        }
        else
        {
            info_operador.atomo = MENOR;
        }
    }
    else
    {
        entrada++;
        info_operador.atomo = ERRO;
    }

    return info_operador;
}

TInfoAtomo reconhece_sinais()
{
    TInfoAtomo info_sinais;
    info_sinais.atomo = ERRO;

    if (*entrada == '(')
    {
        entrada++;
        info_sinais.atomo = ABRE_PAR;
    }
    else if (*entrada == ')')
    {
        entrada++;
        info_sinais.atomo = FECHA_PAR;
    }
    else if (*entrada == '{')
    {
        entrada++;
        info_sinais.atomo = ABRE_CHAVES;
    }
    else if (*entrada == '}')
    {
        entrada++;
        info_sinais.atomo = FECHA_CHAVES;
    }
    else if (*entrada == ',')
    {
        entrada++;
        info_sinais.atomo = VIR;
    }
    else if (*entrada == ';')
    {
        entrada++;
        info_sinais.atomo = PVIR;
    }
    else
    {
        entrada++;
        info_sinais.atomo = ERRO;
    }

    return info_sinais;
}

// reconhece - palavras reservadas e comentários
TInfoAtomo reconhece_palavras_reservadas(char *str)
{
    TInfoAtomo info_palavra_reservada;
    info_palavra_reservada.atomo = ERRO;

    // Comparações para palavras reservadas
    if (strcmp(str, "int") == 0)
    {
        info_palavra_reservada.atomo = INT;
    }
    else if (strcmp(str, "char") == 0)
    {
        info_palavra_reservada.atomo = CHAR;
    }
    else if (strcmp(str, "void") == 0)
    {
        info_palavra_reservada.atomo = VOID;
    }
    else if (strcmp(str, "main") == 0)
    {
        info_palavra_reservada.atomo = MAIN;
    }
    else if (strcmp(str, "if") == 0)
    {
        info_palavra_reservada.atomo = IF;
    }
    else if (strcmp(str, "else") == 0)
    {
        info_palavra_reservada.atomo = ELSE;
    }
    else if (strcmp(str, "while") == 0)
    {
        info_palavra_reservada.atomo = WHILE;
    }
    else if (strcmp(str, "readint") == 0)
    {
        info_palavra_reservada.atomo = READINT;
    }
    else if (strcmp(str, "writeint") == 0)
    {
        info_palavra_reservada.atomo = WRITEINT;
    }
    else
    {
        // se não for palavra reservada, é um identificador
        strcpy(info_palavra_reservada.atributo_ID, str);
        info_palavra_reservada.atomo = IDENTIFICADOR;
    }

    return info_palavra_reservada;
}

TInfoAtomo reconhece_comentarios()
{
    TInfoAtomo info_comentarios;
    info_comentarios.atomo = ERRO;
    info_comentarios.linha = 0;

    if (*entrada == '/')
    {
        entrada++;
        if (*entrada == '/')
        {
            // comentário de uma linha
            info_comentarios.atomo = COMENTARIO;
            while (*entrada != '\0')
            {
                if (*entrada == '\n')
                {
                    break;
                }
                entrada++;
            }
        }
        else if (*entrada == '*')
        {
            // comentário de muitas linha
            info_comentarios.atomo = COMENTARIO;
            while (*entrada != '\0')
            {
                if (*entrada == '*')
                {
                    entrada++;
                    if (*entrada == '/')
                    {
                        entrada++; // consumimos / pois se não ele será lido como operador div
                        break;
                    }
                }
                else if (*entrada == '\n')
                {
                    info_comentarios.linha++;
                }
                entrada++;
            }
        }
        else
        {
            info_comentarios.atomo = OP_DIV;
        }
    }

    return info_comentarios;
}

// auxiliares
int eh_operador(char operador)
{
    if (operador == '+' ||
        operador == '-' ||
        operador == '*' ||
        operador == '/' ||
        operador == '&' ||
        operador == '|' ||
        operador == '=' ||
        operador == '!' ||
        operador == '>' ||
        operador == '<')
    {
        return 1;
    }
    return 0;
}

int eh_sinal(char sinal)
{
    if (sinal == '(' ||
        sinal == ')' ||
        sinal == '{' ||
        sinal == '}' ||
        sinal == ';' ||
        sinal == ',')
    {
        return 1;
    }
    return 0;
}

// funções sintáticas e afins.
void consome(Tatomo atomo)
{
    if (lookahead == atomo)
    {
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else
    {
        printf("Erro sintático");
    }
}

void program()
{
}

void compound_smt()
{
}

void var_decl()
{
}

void type_specifier()
{
}

void var_decl_list()
{
}

void variable_id()
{
}

void stmt()
{
}

void assig_stmt()
{
}

void cond_stmt()
{
}

void while_stmt()
{
}

void expr()
{
}

void conjunction()
{
}

void comparision()
{
}

void relation()
{
}

void sum()
{
}

void term()
{
}

void factor()
{
}