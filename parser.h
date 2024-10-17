#include "lexico.h"
#ifndef PARSER_H
#define PARSER_H

#define MAX_CHILDREN 3

typedef struct treeNode{
    char* value;
    char* escopo;
    char* tipoDado;
    int linha;
    int statment;
    int tamanho;
    int ativacao;
    TokenType tipoID;
    struct treeNode* filhos[MAX_CHILDREN]; // Máximo de filhos
    struct treeNode* irmao;
} TreeNode;

TreeNode* parser();

void yyerror(const char *msg);

TreeNode* createNode(TokenType type, char* value);

void imprimir_arvore(FILE *bison, TreeNode* node, int nivel);

const char* token_type_to_string(TokenType type);

void libera_arvore(TreeNode* node);


#endif