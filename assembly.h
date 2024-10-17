#include "intermediario.h"

typedef enum{
    $zero, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9, $t10, 
    $t11, $t12, $t13, $t14, $t15, $t16, $t17, $t18, $t19, $t20, 
    $t21, $t22, $t23, $t24, $t25, $t26, $aux, $gp, $fp, $sp, $ra 
}registradores;

typedef enum{
    R,
    I,
    J,
    ES
} tipoInstrucao;

typedef struct instrucao{
    tipoInstrucao tipo;
    char* nome;
    //registradores Rs;
    //registradores Rt;
    //registradores Rd;
    char* campo1;
    char* campo2;
    char* campo3;
}Instrucao;

typedef struct label {
	int linha;
	char *nome;
	struct label *prox;
}Label;

typedef struct var {
	char *nome;
	char *escopo;
	char *tipo;
	int pos;
	struct var *prox;
}Var;

typedef struct paramReg{
    char *reg;
    struct paramReg *abaixo;
}tParam;

typedef struct {
    int tamanho;
    tParam* topo;
}pParams;

typedef struct noInstrucao{
    Instrucao* instrucao;
    struct noInstrucao* prox;
}NodeInstrucao;

typedef struct listaInstrucao{
    NodeInstrucao* first;
}ListaInstrucao;

void gera_assembly(NodeQuadrupla* quads);
ListaInstrucao* Assembly(ListaQuadrupla* quads);
void imprimeListaInstrucoes(FILE* assembly, ListaInstrucao* instrucoes);

Instrucao* new_instrucao();
NodeInstrucao* new_node_instrucao();
ListaInstrucao* new_list_instrucao();
void instrucao_insert(ListaInstrucao *instrucoes, char* nome, char* campo1, char* campo2, char* campo3, tipoInstrucao tipo);

char* get_operacao_assembly(Operacoes op);
int get_func_linha (char* label);

void add_label(int linha, char* nome);
void lista_label_free();
void imprimir_labels(FILE* assembly);

void ajusta_desvios(ListaInstrucao* inst);
void ajusta_jumps(ListaInstrucao* inst);

void add_var(char *nome, char *escopo, char *tipo, int pos);
void lista_var_free();
int get_posicao_var(char *nome, char *escopo);

void push(pParams *P, char *nome);
char* pop(pParams *P);
void inicializaPilha(pParams *P);
