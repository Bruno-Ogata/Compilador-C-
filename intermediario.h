#include "semantico.h"

typedef enum{
    FUN,
    LAB,
    LOAD,
    STORE,
    HALT,
    ASSIGN,
    MULT,
    DIV,
    SUM,
    SUB,
    IGUAL,
    MENOR_IGUAL,
    MAIOR_IGUAL,
    MENOR,
    MAIOR,
    DIF,
    GOTO,
    END,
    PARAM,
    ARG,
    CALL,
    IFF,
    RET,
    ALLOC
} Operacoes;

typedef struct param{
    char* value;
    struct param* prox;
}Param;

typedef struct listaParams{
    Param* first;
}ListaParams;

typedef struct quadrupla{
    Operacoes op;
    char* var1;
    char* var2; 
    char* resultado;
}Quadrupla;

typedef struct noQuadrupla{
    Quadrupla* quad;
    struct noQuadrupla* prox;
}NodeQuadrupla;

typedef struct listaQuadrupla{
    NodeQuadrupla* first;
    NodeQuadrupla* last;
}ListaQuadrupla;

Quadrupla* new_quadrupla();
NodeQuadrupla* new_node_quadrupla();
ListaQuadrupla* new_list_quadrupla();
void quadrupla_insert(ListaQuadrupla *quads, Operacoes op, char* var1, char* var2, char* resultado);
void imprimeListaQuad(FILE* intermediario, ListaQuadrupla *quadruplas);

char* get_operacao(Operacoes op);
char* get_reg (int num);
char* get_label (int num);
char* getReg (ListaQuadrupla* quad, TreeNode* arv);
Operacoes get_op_token(TokenType token);

Param* new_param();
ListaParams* new_list_params();
void param_insert(ListaParams *params, char* value);
void libera_params();

void geraInterCode(TreeNode* arvore);
ListaQuadrupla* Intermediario(TreeNode* arvore);

extern ListaQuadrupla Quadruplas; 


