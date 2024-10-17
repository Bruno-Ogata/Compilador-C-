#include "binario.h"
#include "parser.tab.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

ListaBinario *binario;
int linhas=0;

void imprimeListaBinario(FILE* binario, FILE* teste_processador, ListaBinario *bin){
    NodeBinario* node = bin->first;
    int i = 0;
    
    do {
        printf("%d: %s%s%s%s%s%s\n", i, node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
        fprintf(binario, "%d: %s %s %s %s %s %s\n", i, node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
        fprintf(teste_processador, "%s%s%s%s%s%s\n", node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
        node = node->prox;
        i++;
    }while(node->prox != NULL);
    printf("%d: %s%s%s%s%s%s\n", i, node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
    fprintf(binario, "%d: %s %s %s %s %s %s\n", i, node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
    fprintf(teste_processador, "%s%s%s%s%s%s\n", node->binario->opCode, node->binario->Rs, node->binario->Rt, node->binario->Rd, node->binario->Shamt, node->binario->Funct);
}

// cria e aloca memoria para uma nova lista de instruções binárias 
ListaBinario* new_list_binario(){
    ListaBinario *bin = (ListaBinario*) malloc(sizeof(ListaBinario));
    bin->first = NULL;
    return bin;
}

Binario* new_instrucao_bin(){
    Binario *novaInstrucaoBin = (Binario*) malloc(sizeof(Binario));
    if (novaInstrucaoBin == NULL) printf("Erro ao alocar memória para instrução binária\n");

    novaInstrucaoBin->opCode = NULL;
    novaInstrucaoBin->Rs = NULL;
    novaInstrucaoBin->Rt = NULL;
    novaInstrucaoBin->Rd = NULL;
    novaInstrucaoBin->Shamt = NULL;
    novaInstrucaoBin->Funct =NULL;

    return novaInstrucaoBin;
}

// cria e aloca memoria para um novo no binário 
NodeBinario* new_node_binario(){
    NodeBinario *node = (NodeBinario*) malloc(sizeof(NodeBinario));
    node->binario = new_instrucao_bin();
    node->prox = NULL;
    return node;
}

void instrucao_insert_binario(ListaBinario* bin, char* opcode, char* Rs, char* Rt, char* Rd, char* shamt, char* funct){

    NodeBinario* newBinario = new_node_binario();

    newBinario->binario->opCode = opcode;
    newBinario->binario->Rs = Rs;
    newBinario->binario->Rt = Rt;
    newBinario->binario->Rd = Rd;
    newBinario->binario->Shamt = shamt;
    newBinario->binario->Funct = funct;
    //newInstrucao->prox = NULL;

    //printf("teste:%d\n", inst->first->instrucao->tipo);

    if(bin->first != NULL){
        NodeBinario* aux = bin->first;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = newBinario;
    }else{
        bin->first = newBinario;
    }
}

char* reg_to_bin(char* reg){

	char* reg_bin;
	
    if (strcmp (reg, "$zero") == 0){
        reg_bin = "00000";   
    }else if(strcmp (reg, "$t1") == 0){
        reg_bin = "00001";
    }else if(strcmp (reg, "$t2") == 0){
        reg_bin = "00010";
    }else if(strcmp (reg, "$t3") == 0){
        reg_bin = "00011";
    }else if(strcmp (reg, "$t4") == 0){
        reg_bin = "00100";
    }else if(strcmp (reg, "$t5") == 0){
        reg_bin = "00101";
    }else if(strcmp (reg, "$t6") == 0){
        reg_bin = "00110";
    }else if(strcmp (reg, "$t7") == 0){
        reg_bin = "00111";
    }else if(strcmp (reg, "$t8") == 0){
        reg_bin = "01000";
    }else if(strcmp (reg, "$t9") == 0){
        reg_bin = "01001";
    }else if(strcmp (reg, "$t10") == 0){
        reg_bin = "01010";
    }else if(strcmp (reg, "$t11") == 0){
        reg_bin = "01011";
    }else if(strcmp (reg, "$t12") == 0){
        reg_bin = "01100";
    }else if(strcmp (reg, "$t13") == 0){
        reg_bin = "01101";
    }else if(strcmp (reg, "$t14") == 0){
        reg_bin = "01110";
    }else if(strcmp (reg, "$t15") == 0){
        reg_bin = "01111";
    }else if(strcmp (reg, "$t16") == 0){
        reg_bin = "10000";
    }else if(strcmp (reg, "$t17") == 0){
        reg_bin = "10001";
    }else if(strcmp (reg, "$t18") == 0){
        reg_bin = "10010";
    }else if(strcmp (reg, "$t19") == 0){
        reg_bin = "10011";
    }else if(strcmp (reg, "$t20") == 0){
        reg_bin = "10100";
    }else if(strcmp (reg, "$t21") == 0){
        reg_bin = "10101";
    }else if(strcmp (reg, "$t22") == 0){
        reg_bin = "10110";
    }else if(strcmp (reg, "$t23") == 0){
        reg_bin = "10111";
    }else if(strcmp (reg, "$t24") == 0){
        reg_bin = "11000";
    }else if(strcmp (reg, "$t25") == 0){
        reg_bin = "11001";
    }else if(strcmp (reg, "$t26") == 0){
        reg_bin = "11010";
    }else if(strcmp (reg, "$aux") == 0){
        reg_bin = "11011";
    }else if(strcmp (reg, "$gp") == 0){
        reg_bin = "11100";
    }else if(strcmp (reg, "$fp") == 0){
        reg_bin = "11101";
    }else if(strcmp (reg, "$sp") == 0){
        reg_bin = "11110";
    }else if(strcmp (reg, "$ra") == 0){
        reg_bin = "11111";
    }else{
        reg_bin = "Erro! Registrador inválido!";
    }

	return reg_bin;
}

char* int_to_bin(int num, int tam) {
    // Aloca memória para a string de bits (+1 para o caractere nulo)
    char* binario = (char*)malloc((tam + 1) * sizeof(char));
    
    // Verifica se a memória foi alocada corretamente
    if (binario == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Loop para cada bit, começando do mais significativo
    for (int i = 0; i < tam; i++) {
        // Máscara para verificar o bit na posição i
        int mascara = 1 << (tam - 1 - i);
        // Define '1' ou '0' na posição correspondente
        binario[i] = (num & mascara) ? '1' : '0';
    }

    // Adiciona o caractere nulo ao final da string
    binario[tam] = '\0';

    return binario;
}

void instrucao_insert_R(ListaBinario* bin, Instrucao* inst){

    if (strcmp (inst->nome, "add") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000000");
    }else if (strcmp (inst->nome, "sub") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000001");
    }else if (strcmp (inst->nome, "mult") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000010");
    }else if(strcmp (inst->nome, "div") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000011");
    }else if(strcmp (inst->nome, "or") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000100");
    }else if(strcmp (inst->nome, "and") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000101");
    }else if(strcmp (inst->nome, "not") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000110");
    }else if(strcmp (inst->nome, "slt") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "000111");
    }else if(strcmp (inst->nome, "xor") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "001000");
    }else if(strcmp (inst->nome, "nor") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "001001");
    }else if(strcmp (inst->nome, "xnor") == 0){
        instrucao_insert_binario(binario, "000000", reg_to_bin(inst->campo2), reg_to_bin(inst->campo3), reg_to_bin(inst->campo1), "00000", "001010");
    }else{  //default
        instrucao_insert_binario(binario, "000000", "00000", "00000", "00000", "00000", "000000");
    }

}

void instrucao_insert_I(ListaBinario* bin, Instrucao* inst){

    if (strcmp (inst->nome, "addi") == 0){
        instrucao_insert_binario(binario, "000001", reg_to_bin(inst->campo2), reg_to_bin(inst->campo1), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else if (strcmp (inst->nome, "subi") == 0){
        instrucao_insert_binario(binario, "000010", reg_to_bin(inst->campo2), reg_to_bin(inst->campo1), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else if (strcmp (inst->nome, "lw") == 0){
        instrucao_insert_binario(binario, "000011", reg_to_bin(inst->campo1), reg_to_bin(inst->campo2), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else if (strcmp (inst->nome, "sw") == 0){
        instrucao_insert_binario(binario, "000100", reg_to_bin(inst->campo1), reg_to_bin(inst->campo2), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else if (strcmp (inst->nome, "beq") == 0){
        instrucao_insert_binario(binario, "000101", reg_to_bin(inst->campo2), reg_to_bin(inst->campo1), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else if (strcmp (inst->nome, "bne") == 0){
        instrucao_insert_binario(binario, "000110", reg_to_bin(inst->campo2), reg_to_bin(inst->campo1), "", "", int_to_bin(atoi(inst->campo3), 16));
    }else{  //default
        instrucao_insert_binario(binario, "000000", "00000", "00000", "00000", "00000", "000000");
    }

}

void instrucao_insert_J(ListaBinario* bin, Instrucao* inst){

    if (strcmp (inst->nome, "jump") == 0){ //Ok funcionou no processador
        instrucao_insert_binario(binario, "000111", "", "", "", int_to_bin(atoi(inst->campo1), 26), "");
    }else if (strcmp (inst->nome, "jal") == 0){ //Ok funcionou no processador
        instrucao_insert_binario(binario, "001100", "", "", "", int_to_bin(atoi(inst->campo1), 26), "");
    }else if (strcmp (inst->nome, "jr") == 0){ 
        instrucao_insert_binario(binario, "001001", reg_to_bin(inst->campo1), "", "", "000000000000000000000", "");
    }else{  //default
        instrucao_insert_binario(binario, "000000", "00000", "00000", "00000", "00000", "000000");
    }

}

void instrucao_insert_ES(ListaBinario* bin, Instrucao* inst){

    if (strcmp (inst->nome, "output") == 0){
        instrucao_insert_binario(binario, "001010", reg_to_bin(inst->campo1), "", "", "", "111111111111111111111");
    }else if (strcmp (inst->nome, "input") == 0){
        instrucao_insert_binario(binario, "001011", "00000", reg_to_bin(inst->campo1), "", "", "1111111111111111");
    }else if (strcmp (inst->nome, "wait") == 0){
        instrucao_insert_binario(binario, "001101", "00000", "00000", "00000", "00000", "000000");
    }else{ //default
        instrucao_insert_binario(binario, "000000", "00000", "00000", "00000", "00000", "000000");
    }

}

void gera_binario(NodeInstrucao* instrucoes){
    Instrucao* inst = instrucoes->instrucao;

    switch (instrucoes->instrucao->tipo)
    {
    case R:
        instrucao_insert_R(binario, inst);
        break;
    case I: 
        instrucao_insert_I(binario, inst);
        break;
    case J:
        instrucao_insert_J(binario, inst);
        break;
    case ES:
        instrucao_insert_ES(binario, inst);
        break;
    default:
        instrucao_insert_binario(binario, "000000", "00000", "00000", "00000", "00000", "000000");
        break;
    }

    if(instrucoes->prox != NULL){
        NodeInstrucao* aux = instrucoes->prox;
        gera_binario(aux);
    }

    //return instrucoes;
}

ListaBinario* BinarioCode(ListaInstrucao* instrucoes){

    binario = new_list_binario();
    
    if (instrucoes->first != NULL){
        NodeInstrucao* aux = instrucoes->first;
        gera_binario(aux);
    }

    return binario;
    
}