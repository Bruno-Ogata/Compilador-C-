#ifndef LEX_H
#define LEX_H

typedef int TokenType;

typedef struct Token {
    TokenType tipo;
    char token[256];
    int proxEstado;
    int linha;
    struct Token *prox;
    struct Token *ant;
}Token;

// Lista duplamente encadeada
typedef struct lista {
    Token *topo;
} Lista;

/****************************************************/
#define HASH_TABLE_SIZE 97
void criaTabelaHash();
unsigned int hash(char* palavra, int TABLE_SIZE);
void checaHashReservadas(Token *Token);

typedef struct {
    char* palavra;
    int tipo;
} TabelaHashReservadas;
/****************************************************/


void inicializa_lista();

void lisa_token(Lista *lista, Token token);

void libera_lista();

Token allocate_token(int size);

void deallocate_token(Token* Token);

Token getNextToken(FILE* file, Token* Token);

Token get_token();

Token get_previous_token();

void initTabelaTransicoes();

void imprimeToken(FILE* lexico, Token* Token);

#endif