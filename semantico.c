/*
    Projeto de Compiladores 2023.2

    Compilador C-

    Alunos: Bruno Hideki Amadeu Ogata RA: 140884
            Mateus Campanha Pereira RA: 147884

*/
#include <string.h>
#include <stdbool.h>
#include "semantico.h"

// imprime mensagem de erro semantico
void erro_semantico(FILE* semantico, const char *msg, int linha, char *nome, char *escopo){
    fprintf(semantico, "ERRO SEMANTICO: %s - LINHA: %d  NOME: %s ESCOPO: %s\n", msg, linha, nome, escopo);
    printf("ERRO SEMANTICO: %s - LINHA: %d  NOME: %s ESCOPO: %s\n", msg, linha, nome, escopo);
}

// funcao para calcular hash
unsigned int hash_semantico(char* palavra, int TABLE_SIZE) {
	unsigned int hash = 0;
    while (*palavra) {
        hash = ((hash * 97) + (*palavra)) % TABLE_SIZE;
        palavra++;
    }
    return hash;
}

// funcao para concatenar nome com escopo
char *concat_nome_escopo (char *nome, char *escopo) {
    char *IDfunc = (char*) malloc((strlen(nome) + strlen(escopo) + 1) * sizeof(char));
    strcpy(IDfunc, nome);
    strcat(IDfunc, escopo);
    return IDfunc;
}

// cria e aloca memoria para uma nova lista
list* new_list() {
    list *p = (list*) malloc(sizeof(list));
    p->first = NULL;
    return p;
}

// cria e aloca memoria para um novo no
Node* new_node_list() {
    Node *node = (Node*) malloc(sizeof(Node));
    node->linha = 0;
    node->escopo = NULL;
    node->nome = NULL;
    node->tipoDado = NULL;
    node->tipoID = 0;
    node->tamanho = 0;
    node->statment = 0;
    node->ativacao = 0;
    node->next = NULL;
    return node;
}

// cria e aloca memoria para uma nova tabela hash
TabelaHashSimbolos* criaTabelaSimbolos (int TABLE_SIZE) {
    TabelaHashSimbolos *tabelaSimbolos = (TabelaHashSimbolos*) malloc(sizeof(TabelaHashSimbolos));
    
    if (tabelaSimbolos == NULL) printf("nao da pra alocar memoria\n");
    else {
        tabelaSimbolos->TABLE_SIZE = TABLE_SIZE;
        tabelaSimbolos->data = (list**) malloc(TABLE_SIZE * sizeof(list));
        for (int i = 0; i < TABLE_SIZE; i++) 
            tabelaSimbolos->data[i] = NULL;
    }
    return tabelaSimbolos;
}

// funcao para inserir os nos na tabela hash
void preencheTabelaSimbolos (TabelaHashSimbolos *tabelaSimbolos, TreeNode* arvore) {
    static bool first = true;
    if (arvore == NULL) return;
    else {
        if (arvore->tipoID == 258 || arvore->tipoID == 259 || arvore->tipoID == 0 || arvore->tipoID == 1 || arvore->statment == 290) {
            Node *node;
            
            // insere funcoes input e output na tabela
            if(first) {
                node = new_node_list();
                if (node == NULL){
                    printf("nao da pra alocar memoria\n");
                    return;
                }
                node->nome = "input";
                node->tipoDado = "func";
                node->escopo = "global";
                node->tipoID = 258;
                node->linha = 0;
                node->tamanho = 0;
                node->ativacao = 0;
                node->statment = 0;
                node->next = NULL;
                
                hash_insert(tabelaSimbolos, node);

                node = new_node_list();
                if (node == NULL){
                    printf("nao da pra alocar memoria\n");
                    return;
                }
                node->nome = "output";
                node->tipoDado = "func";
                node->escopo = "global";
                node->tipoID = 258;
                node->linha = 0;
                node->tamanho = 0;
                node->ativacao = 0;
                node->statment = 0;
                node->next = NULL;

                hash_insert(tabelaSimbolos, node);
                first = false;
            }
            node = new_node_list();
            if (node == NULL){
                printf("nao da pra alocar memoria\n");
                return;
            }
            node->nome = arvore->value;
            node->tipoID = arvore->tipoID;
            node->tipoDado = arvore->tipoDado;
            node->escopo = arvore->escopo;
            node->linha = arvore->linha;
            node->tamanho = arvore->tamanho;
            node->statment = arvore->statment;
            node->ativacao = arvore->ativacao;
            node->next = NULL;
            hash_insert(tabelaSimbolos, node);
        }
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (arvore->filhos[i] != NULL) {
                preencheTabelaSimbolos(tabelaSimbolos, arvore->filhos[i]);
            }
        }
        if (arvore->irmao != NULL) {
            preencheTabelaSimbolos(tabelaSimbolos, arvore->irmao);
        }
    }
}

// funcao para inserir os nos na tabela hash
void hash_insert (TabelaHashSimbolos *tabelaSimbolos, Node *node) {
    if (tabelaSimbolos == NULL) {
        printf("Tabela hash nao existe!\n");
        return;
    }
    
    // concatena nome com escopo para calcular hash
    char *IDfunc = concat_nome_escopo(node->nome, node->escopo);

    //if  nome é x e escopo é global
    if(strcmp(node->escopo, "global") == 0) {
        int a;
        a = 0;
    }

    int pos = hash(IDfunc, tabelaSimbolos->TABLE_SIZE);
    
        
    if (tabelaSimbolos->data[pos] == NULL) {
        tabelaSimbolos->data[pos] = new_list();
    } else {
        Node *aux = tabelaSimbolos->data[pos]->first;
        while (aux != NULL) {
            if (strcmp(aux->nome, node->nome) == 0 && strcmp(aux->escopo, node->escopo) == 0) {
                break;
            } else {
                pos++;
                
                pos = pos % tabelaSimbolos->TABLE_SIZE;

                if(tabelaSimbolos->data[pos] != NULL) {
                    aux = tabelaSimbolos->data[pos]->first;
                } else {
                    aux = NULL;
                    tabelaSimbolos->data[pos] = new_list();
                }
            }
        }
    }
    list_insert(tabelaSimbolos->data[pos], node);
}


// funcao para inserir os nos na lista
void list_insert (list *p, Node* data) {
    Node *node = new_node_list();
    
    if (node == NULL) {
        printf("Não é possível alocar memória\n");
        return;
    }
    node = data;
    // insert first
    if (p->first == NULL) {
        p->first = node;
        return;
    } else {
        // insert middle
        Node *aux = p->first;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = node;
        return;
    }
}

// função principal para analise semantica
void analise_semantico (FILE *semantico, TabelaHashSimbolos *tabelaSimbolos) {
    bool ja_decl = false;
    bool nao_decl = false;
    bool first = false;
    int pos;
    char *IDfunc;
    Node *temp;
    
    for (int i = 0; i < tabelaSimbolos->TABLE_SIZE; i++) {
        
        if(tabelaSimbolos->data[i] == NULL) continue;

        Node *aux = tabelaSimbolos->data[i]->first;
        if (aux != NULL){
            first = true;

            //if (aux->tipoDado == NULL){
            if (aux->statment != 291){    
                nao_decl = true;
            }else{
                if (strcmp(aux->tipoDado, "var") == 0 && aux->tipoID == 0) {    //VARIÁVEL DECLARADA COM TIPO VOID
                    erro_semantico(semantico, "VARIAVEL DECLARADA COM TIPO INDEVIDO (VOID)", aux->linha, aux->nome, aux->escopo);
                }     
            } 

            // VERIFICA DECLARACAO DE FUNCOES
            if(aux->tipoDado != NULL) {
                if (strcmp(aux->tipoDado, "func") == 0 && strcmp(aux->escopo, "global") != 0){
                    pos = procura_tabela(tabelaSimbolos, aux->nome, "global");
                    
                    if(pos != -1) {
                        temp = tabelaSimbolos->data[pos]->first;
                    } else temp = NULL;
                    
                    if(temp == NULL) erro_semantico(semantico, "FUNCAO USADA SEM DECLARACAO", aux->linha, aux->nome, aux->escopo);
                    else {
                        if(temp->linha > aux->linha) erro_semantico(semantico, "FUNCAO USADA ANTES DE DECLARACAO", aux->linha, aux->nome, aux->escopo);
                        if(aux->ativacao == 1 && temp->tipoID == 0) erro_semantico(semantico, "FUNCAO TIPO VOID ATRIBUIDA A UMA VARIAVEL", aux->linha, aux->nome, aux->escopo);
                    }
                }

                if(strcmp(aux->tipoDado, "var") == 0 && strcmp(aux->escopo, "global") != 0) {
                    pos = procura_tabela(tabelaSimbolos, aux->nome, "global");
                    if(pos != -1){
                        temp = tabelaSimbolos->data[pos]->first;
                        if(strcmp(temp->tipoDado, "func") == 0) erro_semantico(semantico, "VARIAVEL DECLARADA COMO FUNCAO", aux->linha, aux->nome, aux->escopo);
                    }
                }
            }

            do {           
                // VERIFICA SE VARIAVEL ESTA SENDO USADA SEM SER DECLARADA

                if (strcmp(aux->tipoDado, "var") == 0 && nao_decl && aux->tipoID != 0 && aux->statment != 291){
                    pos = procura_tabela(tabelaSimbolos, aux->nome, "global");
                    if (pos == -1) temp = NULL;
                    else temp = tabelaSimbolos->data[pos]->first;
                    if (temp == NULL) erro_semantico(semantico, "VARIAVEL USADA SEM SER DECLARADA!", aux->linha, aux->nome, aux->escopo);
                }else nao_decl = false;
                
                // VERIFICA UNICIDADE EM DECLARAÇÕES
                if (aux->statment == 291) ja_decl = true;
                if (aux->tipoDado != NULL && ja_decl && !first) erro_semantico(semantico, "VARIAVEL JA DECLARADA", aux->linha, aux->nome, aux->escopo);

                // VERIFICAÇÃO DE TIPOS
                if (aux->statment == 290){   //VERIFICA RETURN
                    int tipoReturn = aux->tipoID;
                    pos = procura_tabela(tabelaSimbolos, aux->escopo, "global");
                    temp = tabelaSimbolos->data[pos]->first;
                    if (temp->tipoID != tipoReturn) erro_semantico(semantico, "RETORNO COM TIPO INDEVIDO", aux->linha, aux->nome, aux->escopo);
                } 
                aux = aux->next;
                first = false;
                ja_decl = false;
            } while (aux != NULL);
        } 
    }
    // VERIFICA SE FUNÇÃO MAIN FOI DECLARADA
    pos = procura_tabela(tabelaSimbolos, "main", "global");
    if(pos == -1) erro_semantico(semantico, "FUNCAO MAIN NAO DECLARADA", 0, "", "");
}


// this procedure follow a hash key search
int procura_tabela (TabelaHashSimbolos *tabelaSimbolos, char *nome, char *escopo) {
    int i;
    if (tabelaSimbolos == NULL) {
        printf("lista vazia\n");
        return 0;
    } else {
        for(i = 0; i < tabelaSimbolos->TABLE_SIZE; i++) {
            if(tabelaSimbolos->data[i] == NULL) continue;

            Node *aux = tabelaSimbolos->data[i]->first;

            if (strcmp(tabelaSimbolos->data[i]->first->nome, nome) == 0 && strcmp(tabelaSimbolos->data[i]->first->escopo, escopo) == 0) {
                return i;
            } 
        }
        if(i == tabelaSimbolos->TABLE_SIZE) return -1;
    }
}

void imprimeTabelaSimbolos (FILE* semantico, TabelaHashSimbolos *tabelaSimbolos) {
    if (tabelaSimbolos == NULL) {
        fprintf(semantico, "Tabela hash nao existe!\n");
        return;
    }
    for (int i = 0; i < tabelaSimbolos->TABLE_SIZE; i++) {
        if (tabelaSimbolos->data[i] != NULL) {
            Node *aux = tabelaSimbolos->data[i]->first;
            while (aux != NULL) {
                fprintf(semantico, "Nome: %s Tipo: %d TipoDado: %s Escopo: %s Linha: %d\n", aux->nome, aux->tipoID, aux->tipoDado, aux->escopo, aux->linha);
                aux = aux->next;
            }
        }
        fprintf(semantico, "valor i: %d\n", i);
    }
}


void imprimeTabela(FILE* semantico, TabelaHashSimbolos *tabelaSimbolos){ 
    int i;
    fprintf(semantico, "\n\n");
    fprintf(semantico, "NOME          ESCOPO           TIPO_ID         TIPO_DATA         LINHAS\n");
    fprintf(semantico, "--------      ------------     ------------    ------------      ------------\n");
  
    for (i = 0; i < tabelaSimbolos->TABLE_SIZE; i++) { 
        if (tabelaSimbolos->data[i] != NULL) {

            list *aux = tabelaSimbolos->data[i];

            if (aux != NULL) { 
            
                Node *t = aux->first;
                
                if (strcmp(t->nome, "return") == 0 || strcmp(t->nome, "void") == 0) continue;
                
                fprintf(semantico, "%-14s  ", t->nome);
                fprintf(semantico, "%-14s  ", t->escopo);

                if(t->tipoID == 1) {
                    char *tipo = "int";
                    fprintf(semantico, "%-14s  ", tipo);
                }
                else if(t->tipoID == 0) {
                    char *tipo = "void";
                    fprintf(semantico, "%-14s  ", tipo);
                }

                fprintf(semantico, "%-14s  ", t->tipoDado);	
                
                
                while (t != NULL) { 
                    fprintf(semantico, "%4d ",t->linha);
                    t = t->next;
                }

                fprintf(semantico, "\n");
            }
        }
    }
}

// procedimento para liberar memoria da tabela hash
void libera_tabela(TabelaHashSimbolos *tabelaSimbolos) {
    if (tabelaSimbolos == NULL) {
        printf("Tabela hash nao existe!\n");
        return;
    }
    for (int i = 0; i < tabelaSimbolos->TABLE_SIZE; i++) {
        if (tabelaSimbolos->data[i] != NULL) {
            Node *aux = tabelaSimbolos->data[i]->first;
            while (aux != NULL) {
                Node *temp = aux;
                aux = aux->next;
                free(temp);
                temp == NULL;
            }
            free(tabelaSimbolos->data[i]);
            tabelaSimbolos->data[i] = NULL;
        }
    }
    free(tabelaSimbolos->data);
    tabelaSimbolos->data = NULL;

    free(tabelaSimbolos);
    tabelaSimbolos = NULL;
}
