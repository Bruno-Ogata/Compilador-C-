#include "assembly.h"
#include "parser.tab.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

ListaInstrucao *instrucoes;
int contInstrucoes;
int contVar;
int label_main_linha;
int cont = 0;

pParams pilhaParams;

char* escopo_atual = "global";

Label *label_first = NULL;
Var *var_first = NULL;

/*char* nomes_reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10",
                    "r11","r12","r13","r14","r15","r16","r17","r18","r19","r20",
                    "r21","r22","r23","r24","r25","r26","r27","r28","r29","r30","r31"};*/

//Funções para inserção e manipulação de variáveis
void add_var(char *nome, char *escopo, char *tipo, int pos){
	if (var_first == NULL){
		var_first = (struct var *) malloc (sizeof (struct var));

		var_first->nome = strdup (nome);
		var_first->escopo = strdup (escopo);
		var_first->tipo = strdup (tipo);
		var_first->pos = pos;
		return;
	}

	Var *node = var_first;

	while (node->prox){
		node = node->prox;
	}

	node->prox = (struct var*) malloc (sizeof (struct var));

	node->prox->nome = strdup (nome);
	node->prox->escopo = strdup (escopo);
	node->prox->tipo = strdup (tipo);
	node->prox->pos = pos;
	node->prox->prox = NULL;
}

void lista_var_free(){
	Var* node = var_first;
	while(node){
		Var *aux = node;
		node = node->prox;
		free(aux->nome);
		free(aux->escopo);
		free(aux->tipo);
	}
	node = NULL;
	var_first = NULL;
}

int get_posicao_var(char *nome, char *escopo){

	Var *node = var_first;
	int posicao = 0;

	while (node != NULL){
		Var *aux = node;

		if (strcmp(nome, node->nome) == 0 && strcmp(escopo, node->escopo) == 0){
			posicao = node->pos;
			break;
		}

		node = node->prox;
	}
	return posicao;
}

//Funções para inserção e manipulação de labels
void add_label(int linha, char* nome){
	if (label_first == NULL){
		label_first = (Label*) malloc (sizeof (Label));

		label_first->linha = linha;
		label_first->nome = strdup (nome);
		label_first->prox = NULL;
		return;
	}

	Label* node = label_first;

	while (node->prox){
		node = node->prox;
	}

	node->prox = (Label*) malloc (sizeof (Label));

	node->prox->linha = linha;
	node->prox->nome = strdup(nome);
	node->prox->prox = NULL;
}

void lista_label_free(){
	Label* node = label_first;
	while (node){
		Label* aux = node;
		node = node->prox;
		free(aux->nome);
		free(aux);
	}
	node = NULL;
	label_first = NULL;
}

int get_label_linha(char label []){
	Label* node = label_first;
	int posicao = 0;

	while (node != NULL){
		Label* aux = node;

		if (strcmp(label, node->nome) == 0){
			posicao = node->linha;
			break;
		}

		node = node->prox;
	}
	return posicao;
}

void imprimir_labels(FILE* assembly){

    Label* node = label_first;
    fprintf(assembly, "\nLables:\n");
    while (node != NULL) {
        fprintf(assembly, "Linha: %d, Label: %s\n", node->linha, node->nome);
        node = node->prox;
    }
}

void ajusta_desvios(ListaInstrucao* inst){
	
    NodeInstrucao* node = inst->first;
	char* label_linha;

	label_linha = malloc (4*sizeof (char));
	while (node->prox != NULL){
		if((strcmp (node->instrucao->nome, "beq") == 0) || (strcmp (node->instrucao->nome, "bne") == 0)){
			sprintf(label_linha, "%d", get_label_linha(node->instrucao->campo3));
			node->instrucao->campo3 = strdup(label_linha);
		}
        node = node->prox;
	}
	free(label_linha);
}

void ajusta_jumps(ListaInstrucao* inst){
	NodeInstrucao* node = inst->first;
	char* label_linha;

	label_linha = malloc (4*sizeof (char));
    printf("linha MAIN: %d\n", label_main_linha);
	while (node->prox != NULL){

		if((strcmp (node->instrucao->nome, "jump") == 0) && cont == 0){
            printf("Instrucao: %s label atual: %s\n", node->instrucao->nome, node->instrucao->campo1);
            sprintf(label_linha, "%d", label_main_linha);
			node->instrucao->campo1 = strdup(label_linha);
            cont++;
		}else if((strcmp (node->instrucao->nome, "jump") == 0) || (strcmp (node->instrucao->nome, "jal") == 0)){
            printf("Instrucao: %s label atual: %s\n", node->instrucao->nome, node->instrucao->campo1);
            sprintf(label_linha, "%d", get_label_linha(node->instrucao->campo1));
			node->instrucao->campo1 = strdup(label_linha);
            cont++;
        }
        node = node->prox;
	}
	free(label_linha);	
}

void inicializaPilha(pParams *P){
	P = malloc(sizeof(pParams*));
	P->tamanho = 0;
	P->topo = NULL;
}

void push(pParams *P, char *reg){
	tParam* regNome = malloc(sizeof(tParam*));
	regNome->reg = strdup(reg);
	regNome->abaixo = P->topo;
	P->topo = regNome;
	P->tamanho++;
}

char* pop(pParams *P){
	char *reg = NULL;
	if (P->tamanho != 0){// pilha não vazia
		reg = strdup(P->topo->reg);
		tParam *topoAnt;
		topoAnt = P->topo;
		P->topo = topoAnt->abaixo;
		P->tamanho--;
		reg = strdup(topoAnt->reg);
		free(topoAnt);
	}
	return reg;
}


char* get_operacao_assembly(Operacoes op){
    char* operacao = malloc(4*sizeof(char));
    switch (op)
    {
    case MULT:
        operacao = strdup("mult");
        break;
    case SUM:
        operacao = strdup("add");
        break;
    case SUB:
        operacao = strdup("sub");
        break;
    case IGUAL:
        operacao = strdup("xnor");
        break;
    case DIF:
        operacao = strdup("xor");
        break; 
    case DIV:
        operacao = strdup("div");
        break;
    case STORE:
        operacao = strdup("STORE");
        break;
    case LOAD:
        operacao = strdup("LOAD");
        break;
    case HALT:
        operacao = strdup("HALT");
        break;
    case LAB:
        operacao = strdup("LAB");
        break;
    case GOTO:
        operacao = strdup("GOTO");
        break;
    case END:
        operacao = strdup("END");
        break;
    case PARAM:
        operacao = strdup("PARAM");
        break;
    case CALL:
        operacao = strdup("CALL");
        break;
    case ASSIGN:
        operacao = strdup("ASSIGN");
        break;
    case ALLOC:
        operacao = strdup("ALLOC");
        break;
    case FUN:
        operacao = strdup("FUN");
        break;
    case MENOR:
        operacao = strdup("slt");
        break;
    case MAIOR:
        operacao = strdup("sgt");
        break;
    case MAIOR_IGUAL:
        operacao = strdup("GEQ");
        break;
    case MENOR_IGUAL:
        operacao = strdup("LEQ");
        break;
    case IFF:
        operacao = strdup("IFF");
        break;
    case RET:
        operacao = strdup("RET");
        break;
    case ARG:
        operacao = strdup("ARG");
        break;
    default:
        break;
    }
    return operacao;
}

int get_func_linha (char* label){
	Label* node = label_first;
	int pos = 0;

	while (node != NULL){
		Label* aux = node;
		if (strcmp(label, node->nome) == 0){
			pos = node->linha;
			break;
		}
		node = node->prox;
	}
	return pos;
}

void imprimeListaInstrucoes(FILE* assembly, ListaInstrucao *inst){
    NodeInstrucao* node = inst->first;
    int i = 0;
    
    do {
        printf("%d: %s %s %s %s\n", i, node->instrucao->nome, node->instrucao->campo1, node->instrucao->campo2, node->instrucao->campo3);
        fprintf(assembly, "%d: %s %s %s %s\n", i, node->instrucao->nome, node->instrucao->campo1, node->instrucao->campo2, node->instrucao->campo3);
        node = node->prox;
        i++;
    }while(node->prox != NULL);
    printf("%d: %s %s %s %s\n", i, node->instrucao->nome, node->instrucao->campo1, node->instrucao->campo2, node->instrucao->campo3);
    fprintf(assembly, "%d: %s %s %s %s\n", i, node->instrucao->nome, node->instrucao->campo1, node->instrucao->campo2, node->instrucao->campo3);
}

// cria e aloca memoria para uma nova lista de instruções
ListaInstrucao* new_list_instrucao() {
    ListaInstrucao *inst = (ListaInstrucao*) malloc(sizeof(ListaInstrucao));
    inst->first = NULL;
    return inst;
}

Instrucao* new_instrucao(){
    Instrucao *novaInstrucao = (Instrucao*) malloc(sizeof(Instrucao));
    if (novaInstrucao == NULL) printf("Erro ao alocar memória para quádrupla\n");

    novaInstrucao->nome = NULL;
    novaInstrucao->campo1 = NULL;
    novaInstrucao->campo2 = NULL;
    novaInstrucao->campo3 = NULL;
    novaInstrucao->tipo = -1;

    return novaInstrucao;
}

// cria e aloca memoria para um novo no de quadrupla 
NodeInstrucao* new_node_instrucao(){
    NodeInstrucao *node = (NodeInstrucao*) malloc(sizeof(NodeInstrucao));
    node->instrucao = new_instrucao();
    node->prox = NULL;
    return node;
}

void instrucao_insert(ListaInstrucao *inst, char* nome, char* campo1, char* campo2, char* campo3, tipoInstrucao tipo){

    NodeInstrucao* newInstrucao = new_node_instrucao();

    newInstrucao->instrucao->tipo = tipo;
    newInstrucao->instrucao->nome = nome;
    newInstrucao->instrucao->campo1 = campo1;
    newInstrucao->instrucao->campo2 = campo2;
    newInstrucao->instrucao->campo3 = campo3;
    //newInstrucao->prox = NULL;

    //printf("teste:%d\n", inst->first->instrucao->tipo);

    if(inst->first != NULL){
        NodeInstrucao* aux = inst->first;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = newInstrucao;
    }else{
        inst->first = newInstrucao;
    }
}

void gera_assembly(NodeQuadrupla* quads){
    char *operacao;
    char *posicao = (char*) malloc(sizeof(char));
    int pos, tam_vet=0;
    char* aux_var_cont_um;
    char* aux_var_cont;
    char* aux_tam_vet;

    switch (quads->quad->op)
    {
    case SUM:
    case SUB:
    case DIV:
    case MULT:
    case DIF:
    case MENOR:
    case IGUAL:
        operacao = get_operacao_assembly(quads->quad->op);
        if (isdigit(quads->quad->var1[0])){
            instrucao_insert(instrucoes, "addi", "$aux", "$zero", quads->quad->var1, I);
            instrucao_insert(instrucoes, operacao, quads->quad->resultado, "$aux", quads->quad->var2, R);
            contInstrucoes = contInstrucoes + 2;
        }else if (isdigit(quads->quad->var2[0])){
            instrucao_insert(instrucoes, "addi", "$aux", "$zero", quads->quad->var2, I);
            instrucao_insert(instrucoes, operacao, quads->quad->resultado, quads->quad->var1, "$aux", R);
            contInstrucoes = contInstrucoes + 2;
        }else{
            instrucao_insert(instrucoes, operacao, quads->quad->resultado, quads->quad->var1, quads->quad->var2, R);
            contInstrucoes++;
        }
        break;
    /*case IGUAL: //Atenção nesse passo (possível impacto no fluxo do programa - verificado em IFF posteriormente)
        if (isdigit(quads->quad->var2[0])){
            instrucao_insert(instrucoes, "addi", "$aux", "$zero", quads->quad->var2, I);
            instrucao_insert(instrucoes, "xor", "$aux", quads->quad->var1, "$aux", R);
            instrucao_insert(instrucoes, "nor", quads->quad->resultado, "$aux", "$zero", R);
            contInstrucoes = contInstrucoes + 3;
        }else{
            instrucao_insert(instrucoes, "xor", "$aux", quads->quad->var1, quads->quad->var2, R);
            instrucao_insert(instrucoes, "nor", quads->quad->resultado, "$aux", "$zero", R);
            contInstrucoes = contInstrucoes + 2;
        }
        break;*/
    case MAIOR: 
        instrucao_insert(instrucoes, "slt", quads->quad->resultado, quads->quad->var2, quads->quad->var1, R); //lógica invertida de slt para implementar o sgt
        break;
    case IFF:
        instrucao_insert(instrucoes, "bne", quads->quad->resultado, "$zero", quads->quad->var1, I);
        contInstrucoes++;
        break;
    case FUN:   
        add_label(contInstrucoes, quads->quad->var1); //adiciona escopo da função na lista de labels para linkar quando a função for chamada

        escopo_atual = strdup(quads->quad->var1);

        if (strcmp(escopo_atual, "main") == 0) {
            contVar = 0;
            label_main_linha = contInstrucoes; //linha da main salva para inserir no jump
        }else{
            contVar = 2;
        }

        if(strcmp (escopo_atual, "main") != 0){ //funções diferentes da main
            instrucao_insert(instrucoes, "sw", "$fp", "$ra", "1", I);
            instrucao_insert(instrucoes, "addi", "$sp", "$sp", "1", I);
			contInstrucoes = contInstrucoes + 2;
		}
        break;
    case END:

        lista_var_free();

		if(strcmp (quads->quad->resultado, "main") != 0){ //para funções que nao sejam a main
				instrucao_insert(instrucoes, "lw", "$fp", "$ra", "1", I);
				instrucao_insert(instrucoes, "jr", "$ra", "$zero", "$zero", J);
				contInstrucoes = contInstrucoes + 2;
			}
        break;
    case ALLOC:
        if(strcmp(quads->quad->var2, "-") != 0){ //alocação de vetor
            if(strcmp (escopo_atual, "global") == 0){
                add_var(quads->quad->resultado, quads->quad->var1, "int", contVar);
				//add_var_lista(c2, c3, "", contVar);

				sprintf(aux_var_cont_um, "%d", contVar+1);
				sprintf(aux_var_cont, "%d", contVar);

				contVar++;

                instrucao_insert(instrucoes, "addi", "$aux", "$zero", aux_var_cont_um, I);
                instrucao_insert(instrucoes, "sw", "$zero", "$aux", aux_var_cont, I);
				contInstrucoes = contInstrucoes + 2;
			}else{ //REVISAR AQUI
				tam_vet = atoi(quads->quad->var2);
				if (tam_vet > 0){
				    sprintf(aux_var_cont_um, "%d", contVar+1);
				    sprintf(aux_var_cont, "%d", contVar);
					//lista_add_asm("addi","$fp","$aux", aux_var_cont_um);
					//lista_add_asm("addi","$fp","$aux", aux_var_cont);
					contInstrucoes = contInstrucoes + 2;
				}

				tam_vet = tam_vet + 1;
				contVar = contVar + tam_vet;

				sprintf(aux_tam_vet, "%d", tam_vet);

				//lista_add_asm("addi","$sp","$sp", aux_tam_vet);
				contInstrucoes++;
			}
        }else{  //variável inteira
            add_var(quads->quad->resultado, quads->quad->var1, "int", contVar); //insere variável em lista de variáveis (armazena posição tbm)
            contVar++;

            if (strcmp (quads->quad->var1, "global") != 0){
				instrucao_insert (instrucoes, "addi", "$sp", "$sp", "1", I);
				contInstrucoes++;
		    }else {
				instrucao_insert (instrucoes, "addi", "$sp", "$zero", "1", I);
				contInstrucoes++;
			}
        }
        break;
    case ARG: //Falta implementação de vetores
			add_var(quads->quad->var1, quads->quad->var2, quads->quad->resultado, contVar); //adiciona argumento a lista de variáveis do escopo 
			contVar++;

			instrucao_insert(instrucoes, "addi", "$sp", "$sp", "1", I); //aloca espaço necessário no stack 
			contInstrucoes++;
        break;
    case LOAD: //Falta implementação de vetores 
        
        if (!isdigit(quads->quad->var1[0])){ 
            //posicao = malloc(sizeof(char));

            pos = get_posicao_var(quads->quad->var1, escopo_atual);     //REVISAR AQUI 
			sprintf(posicao, "%d", pos);
					
			if(strcmp(escopo_atual, "global") == 0){ //alocação global
				instrucao_insert (instrucoes, "lw", "$zero", quads->quad->resultado, posicao, I);
				contInstrucoes++;
			}else{
				instrucao_insert(instrucoes, "lw", "$fp", quads->quad->resultado, posicao, I);
				contInstrucoes++;
			}
		}else{ //load imediato 
			instrucao_insert(instrucoes, "li", quads->quad->resultado, quads->quad->var1, "", I);
			contInstrucoes++;
		}
        break;
    case STORE: //Falta implementação de vetores 
        //posicao = malloc(sizeof(char)); 

        pos = get_posicao_var(quads->quad->resultado, escopo_atual);     //REVISAR AQUI (revisado)
		sprintf(posicao, "%d", pos);
				
        if(strcmp(escopo_atual, "global") == 0){
			instrucao_insert(instrucoes, "sw", "$zero", quads->quad->var1, posicao, I);				
			contInstrucoes++;
		}else{
			instrucao_insert(instrucoes, "sw", "$fp", quads->quad->var1, posicao, I);				
			contInstrucoes++;
		}
        break;
    case GOTO:
        instrucao_insert(instrucoes, "jump", quads->quad->resultado, "", "", J);
        contInstrucoes++;
        break;
    case ASSIGN:
        instrucao_insert(instrucoes, "addi", quads->quad->resultado, quads->quad->var1, "0", I);
        contInstrucoes++;
        break;
    case LAB:
        add_label(contInstrucoes, quads->quad->resultado);
        break;
    case PARAM:
        push(&pilhaParams, quads->quad->resultado);
        break;
    case RET:
        instrucao_insert(instrucoes, "addi", "$gp", quads->quad->resultado, "0", I); //observar melhor
        contInstrucoes++;
        break;
    case CALL:

        if(strcmp(quads->quad->var1, "input") == 0){
            instrucao_insert(instrucoes, "wait", "", "", "", ES); //wait até a entrada ser inserida
            instrucao_insert(instrucoes, "input", quads->quad->resultado, "", "", ES); //função de input 
            contInstrucoes = contInstrucoes + 2;
        }else if(strcmp(quads->quad->var1, "output") == 0){
            instrucao_insert(instrucoes, "output", pop(&pilhaParams), "", "", ES); //função de output 
            contInstrucoes++;
        }else{ //chamada de funções normais
            int params = atoi(quads->quad->var2);
            int i;
			char* pos_param;
            char* aux;

            instrucao_insert(instrucoes, "sw", "$sp", "$fp", "0", I);
			instrucao_insert(instrucoes, "addi", "$fp", "$sp", "0", I);
			instrucao_insert(instrucoes, "addi", "$sp", "$sp", "1", I);

			contInstrucoes = contInstrucoes + 3;

			pos_param = malloc (4*sizeof(char));

			for (i=params+1; i>=2; i--){
				sprintf(pos_param, "%d", i);
                aux = strdup(pos_param);
                instrucao_insert(instrucoes, "sw", "$fp", pop(&pilhaParams), aux, I);
				contInstrucoes++;
			}

			instrucao_insert(instrucoes, "jal", quads->quad->var1, "", "", J);
            instrucao_insert(instrucoes, "wait", "", "", "", ES);
            instrucao_insert(instrucoes, "addi", "$fp", "$sp", "0", I);
            instrucao_insert(instrucoes, "lw", "$fp", "$fp", "0", I);
            instrucao_insert(instrucoes, "addi", quads->quad->resultado, "$gp", "0", I);
			contInstrucoes = contInstrucoes + 5;

			free(pos_param);
        }

        break;
    case HALT:
        //Não faz nada 
    default:
        //instrucao_insert(instrucoes, "default_teste", "1", "1", "1", R);
        //contInstrucoes++;
        break;
    }

    if(quads->prox != NULL){
        NodeQuadrupla* aux = quads->prox;
        gera_assembly(aux);
    }

    //return instrucoes;
}

ListaInstrucao* Assembly(ListaQuadrupla* quads){

    instrucoes = new_list_instrucao();

    //jump para a main
    instrucao_insert(instrucoes, "jump", "", "", "", J);
    contInstrucoes++;
    
    if (quads->first != NULL){
        NodeQuadrupla* aux = quads->first;
        gera_assembly(aux);
    }

    ajusta_desvios(instrucoes);
    ajusta_jumps(instrucoes);

    return instrucoes;
    
}