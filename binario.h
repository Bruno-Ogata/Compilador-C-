#include "assembly.h"

typedef struct binario{
    char* opCode;
    char* Rs;
    char* Rt;
    char* Rd;
    char* Shamt;
    char* Funct;
}Binario;

typedef struct noBinario{
    Binario* binario;
    struct noBinario* prox;
}NodeBinario;

typedef struct listaBinario{
    NodeBinario* first;
}ListaBinario;

void imprimeListaBinario(FILE* binario, FILE* teste_processador, ListaBinario *bin);
Binario* new_instrucao_bin();
NodeBinario* new_node_binario();

void instrucao_insert_binario(ListaBinario *bin, char* opcode, char* Rs, char* Rt, char* Rd, char* shamt, char* funct);
void instrucao_insert_R(ListaBinario *bin, Instrucao* inst);
void instrucao_insert_I(ListaBinario *bin, Instrucao* inst);
void instrucao_insert_J(ListaBinario *bin, Instrucao* inst);
void instrucao_insert_ES(ListaBinario *bin, Instrucao* inst);
void gera_binario(NodeInstrucao* instrucoes);
ListaBinario* BinarioCode(ListaInstrucao* instrucoes);

char* reg_to_bin(char* reg);
char* int_to_bin(int num, int tam);