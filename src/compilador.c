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
    "PONTO_VIRGULA", // 10
    "INT",           // 11
    "CHAR",          // 12
    "VIRGULA",       // 13
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
char *strAtomoSimbolos[] = {
    "ERRO",          // 0
    "identificador", // 1
    "charconst",     // 2
    "intconst",      // 3
    "void",          // 4
    "main",          // 5
    "(",             // 6
    ")",             // 7
    "{",             // 8
    "}",             // 9
    ";",             // 10
    "int",           // 11
    "char",          // 12
    ",",             // 13
    "=",             // 14
    "readint",       // 15
    "writeint",      // 16
    "if",            // 17
    "else",          // 18
    "while",         // 19
    "||",            // 20
    "&&",            // 21
    "<",             // 22
    "<=",            // 23
    "==",            // 24
    "!=",            // 25
    ">",             // 26
    ">=",            // 27
    "+",             // 28
    "-",             // 29
    "*",             // 30
    "/",             // 31
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
// char *entrada = "/* \nprograma le dois numessros inteir e encontra o maior  \n*/ \nvoid main ( void ) { \n int aaaaaaaa, num_2, maior; \nreadint(num_1); \nreadint(num_2); \nif ( num_1 > num_2 )  \n    maior = num_1; \nelse \n    maior = num_2; \n \nwriteint(maior); // imprime o maior valor \n}";
char *entrada;

#include <stdio.h>
#include <stdlib.h>

int main()
{
    //******** INICIO - apresentação e leitura do arquivo *******
    printf("Compilador TINY-C\n\n");

    char nome_arquivo[256];

    printf("Digite o nome do arquivo:\n");
    scanf("%255s", nome_arquivo);
    //******** FIM - apresentação e leitura do arquivo *******

    //******** INICIO - leitura do arquivo *******
    FILE *arquivo = fopen(nome_arquivo, "r");
    char conteudo[100000], c;
    int i = 0, tam = sizeof(conteudo) - 1;

    if (arquivo == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        return 0;
    }

    while ((c = fgetc(arquivo)) != EOF && i < tam)
    {
        conteudo[i++] = c;
    }

    // garante que a string acabe corretamente e aloca memória dinamicamente a entrada.
    conteudo[i] = '\0';
    entrada = (char *)malloc(i + 1);

    // verifica se o arquivo existe
    if (entrada == NULL)
    {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // copia o conteúdo para entrada.
    strcpy(entrada, conteudo);

    fclose(arquivo);
    //******** FIM - leitura do arquivo *******

    //******** INICIO - compilador *******
    info_atomo = obter_atomo();
    lookahead = info_atomo.atomo;

    program();

    printf("\n%d linhas analisadas, programa sintaticamente correto.", linha);
    //******** FIM - compilador *******

    // libera memória alocada aqui.
    free(entrada);

    return 0;
}

TInfoAtomo obter_atomo()
{
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;

    int eh_comentario = 0;

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
        if (info_atomo.atomo == COMENTARIO)
        {
            eh_comentario = 1;
        }
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

    // condicional para armazenar em qual linha o comentário começou
    // sem ele, o comentário só contaria a linha que terminou e não a primeira linha
    if (!eh_comentario)
    {
        info_atomo.linha = linha;
    }

    // condicional para encerrar o programa caso um erro léxico seja encontrado.
    // retorna o caracter que gerou o erro lexico
    if (info_atomo.atomo == ERRO)
    {
        printf("#  %02d: Erro lexico: caracter [%c] nao reconhecido.\n", linha, *entrada);
        exit(1);
    }
    return info_atomo;
}

// reconhece - máquinas de estados
TInfoAtomo reconhece_id()
{
    TInfoAtomo info_id;
    info_id.atomo = ERRO;
    int maximo_char = 0;
    char str_id[16];
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
        printf("#  %02d: Erro lexico: identificador ultrapassa 15 caracteres.\n", linha);
        exit(1);
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
    char str_num[11];
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
        printf("#  %02d: Erro lexico: intconst ultrapassa 10 caracteres.\n", linha);
        exit(1);
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

    if (*entrada == '/')
    {
        entrada++;
        if (*entrada == '/')
        {
            // comentário de uma linha
            info_comentarios.atomo = COMENTARIO;
            info_comentarios.linha = linha;
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
            info_comentarios.linha = linha;
            while (*entrada != '\0')
            {
                int alteradoPreviamente = 0;

                if (*entrada == '\n')
                {
                    linha++;
                }

                if (*entrada == '*')
                {
                    entrada++;
                    alteradoPreviamente = 1;
                    if (*entrada == '/')
                    {
                        entrada++; // consumimos / pois se não ele será lido como operador div
                        break;
                    }
                }

                if (!alteradoPreviamente)
                {
                    entrada++;
                }
            }
        }
        else
        {
            info_comentarios.atomo = OP_DIV;
            info_comentarios.linha = linha;
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

    while (lookahead == COMENTARIO)
    {
        // exibição do token reconhecido
        printf("#  %02d: %s\n", info_atomo.linha, strAtomo[lookahead]);

        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }

    if (lookahead == atomo)
    {
        // exibição do token reconhecido
        printf("#  %02d: %s", info_atomo.linha, strAtomo[lookahead]);
        if (info_atomo.atomo == IDENTIFICADOR || info_atomo.atomo == CHARCONST)
        {
            printf(" | %s", info_atomo.atributo_ID);
        }
        else if (info_atomo.atomo == INTCONST)
        {
            printf(" | %d", info_atomo.atributo_numero);
        }
        printf("\n");

        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else
    {
        printf("#  %02d: Erro sintatico: esperado [%s], encontrado [%s]", linha, strAtomoSimbolos[atomo], strAtomoSimbolos[lookahead]);
        exit(1);
    }
}

void program()
{
    consome(VOID);
    consome(MAIN);
    consome(ABRE_PAR);
    consome(VOID);
    consome(FECHA_PAR);
    compound_smt();
}

void compound_smt()
{
    consome(ABRE_CHAVES);
    var_decl();
    while (lookahead == READINT || lookahead == WRITEINT || lookahead == WHILE || lookahead == IF || lookahead == IDENTIFICADOR)
    {
        stmt();
    }
    consome(FECHA_CHAVES);
}

void var_decl()
{
    if (lookahead == INT || lookahead == CHAR)
    {
        type_specifier();
        var_decl_list();
        consome(PVIR);
    }
}

void type_specifier()
{
    if (lookahead == INT || lookahead == CHAR)
    {
        consome(lookahead);
    }
    else
    {
        consome(IDENTIFICADOR); // erro
    }
}

void var_decl_list()
{
    variable_id();
    while (lookahead == VIR)
    {
        consome(VIR);
        variable_id();
    }
}

void variable_id()
{
    consome(IDENTIFICADOR);
    if (lookahead == RECEBE)
    {
        consome(RECEBE);
        expr();
    }
}

void stmt()
{
    if (lookahead == ABRE_CHAVES)
    {
        compound_smt();
    }
    else if (lookahead == IDENTIFICADOR)
    {
        assig_stmt();
    }
    else if (lookahead == IF)
    {
        cond_stmt();
    }
    else if (lookahead == WHILE)
    {
        while_stmt();
    }
    else if (lookahead == READINT)
    {
        consome(READINT);
        consome(ABRE_PAR);
        consome(IDENTIFICADOR);
        consome(FECHA_PAR);
        consome(PVIR);
    }
    else if (lookahead == WRITEINT)
    {
        consome(WRITEINT);
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
        consome(PVIR);
    }
    else
    {
        consome(IDENTIFICADOR); // erro
    }
}

void assig_stmt()
{
    consome(IDENTIFICADOR);
    consome(RECEBE);
    expr();
    consome(PVIR);
}

void cond_stmt()
{
    consome(IF);
    consome(ABRE_PAR);
    expr();
    consome(FECHA_PAR);
    stmt();

    if (lookahead == ELSE)
    {
        consome(ELSE);
        stmt();
    }
}

void while_stmt()
{
    consome(WHILE);
    consome(ABRE_PAR);
    expr();
    consome(FECHA_PAR);
    stmt();
}

void expr()
{
    conjunction();
    while (lookahead == OR)
    {
        consome(OR);
        conjunction();
    }
}

void conjunction()
{
    comparision();
    while (lookahead == AND)
    {
        consome(AND);
        comparision();
    }
}

void comparision()
{
    sum();
    if (lookahead == MENOR || lookahead == MENOR_IGUAL || lookahead == IGUAL || lookahead == DIFERENTE || lookahead == MAIOR || lookahead == MAIOR_IGUAL)
    {
        relation();
        sum();
    }
}

void relation()
{
    if (lookahead == MENOR || lookahead == MENOR_IGUAL || lookahead == IGUAL || lookahead == DIFERENTE || lookahead == MAIOR || lookahead == MAIOR_IGUAL)
    {
        consome(lookahead);
    }
    else
    {
        consome(IDENTIFICADOR); // erro
    }
}

void sum()
{
    term();
    while (lookahead == OP_SOMA || lookahead == OP_SUBT)
    {
        consome(lookahead);
        term();
    }
}

void term()
{
    factor();
    while (lookahead == OP_MULT || lookahead == OP_DIV)
    {
        consome(lookahead);
        factor();
    }
}

void factor()
{
    if (lookahead == INTCONST || lookahead == CHARCONST || lookahead == IDENTIFICADOR)
    {
        consome(lookahead);
    }
    else if (lookahead == ABRE_PAR)
    {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    }
    else
    {
        consome(IDENTIFICADOR); // erro
    }
}