#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

// linked list structs
typedef struct Node {
    char* nome;
    char* escopo;
    char* tipoDado;
    int linha;
    int tipoID;
    int tamanho;
    int ativacao;
    int statment;
    struct Node *next;
}Node;

typedef struct list {
    Node *first;
}list;

// hash table struct
typedef struct hashSimbolos {
    int TABLE_SIZE;
    list **data;
} TabelaHashSimbolos;



TabelaHashSimbolos* criaTabelaSimbolos(int TABLE_SIZE);

void preencheTabelaSimbolos (TabelaHashSimbolos *tabelaSimbolos, TreeNode* arvore);

void hash_insert (TabelaHashSimbolos *tabelaSimbolos, Node *node);

void imprimeTabelaSimbolos (FILE* semantico, TabelaHashSimbolos *tabelaSimbolos);

list* new_list();

Node* new_node_list();

void list_insert (list *p, Node* data);

void analise_semantico (FILE *semantico, TabelaHashSimbolos *tabelaSimbolos); 

void imprimeTabela(FILE* semantico, TabelaHashSimbolos *tabelaSimbolos);

int procura_tabela(TabelaHashSimbolos *tabelaSimbolos, char *nome, char *escopo);

void libera_tabela(TabelaHashSimbolos *tabelaSimbolos);