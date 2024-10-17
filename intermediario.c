#include "intermediario.h"
#include "parser.tab.h"
#include <string.h>

static int label = 0;
static int t = 1;
static int param = 0;
static int cont_args = 0;
static int args_func = 0;
int ativacao = 0;
int isnumber = 0;
ListaQuadrupla *quads;
ListaParams *params;

char* get_reg (int num){
    char* reg = malloc(4*sizeof(char));
    sprintf(reg,"$t%d",num%32);
    return reg;
}

char* get_label (int num){
    char* label = malloc(3*sizeof(char));
    sprintf(label,"L%d",num);
    return label;
}

char* getReg (ListaQuadrupla* quad, TreeNode* arv){
    char* reg;
    if(quad->first != NULL){
        NodeQuadrupla* aux = quads->first;
        while(aux->prox != NULL){
            if(aux->quad->op == LOAD){
                /*printf("LOAD\n");
                printf("VALORES JA ARMAZENADOS: %s\n", aux->quad->var1);
                printf("ARVORE VALUE: %s\n", arv->value);*/
                if(strcmp(aux->quad->var1, arv->value) == 0) {
                    reg = aux->quad->resultado;
                    return reg;
                }
            }
            aux = aux->prox;
        }
        if(aux->quad->op == LOAD){
            if(strcmp(aux->quad->var1, arv->value) == 0) {
                reg = aux->quad->resultado;
                return reg;
            }
        }
    }
}

char* get_operacao(Operacoes op){
    char* operacao = malloc(4*sizeof(char));
    switch (op)
    {
    case MULT:
        operacao = strdup("MUL");
        break;
    case SUM:
        operacao = strdup("ADD");
        break;
    case SUB:
        operacao = strdup("SUB");
        break;
    case IGUAL:
        operacao = strdup("EQUAL");
        break;
    case DIF:
        operacao = strdup("NEQ");
        break; 
    case DIV:
        operacao = strdup("DIV");
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
        operacao = strdup("LT");
        break;
    case MAIOR:
        operacao = strdup("GT");
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

Operacoes get_op_token(TokenType token){
    Operacoes op;

    switch (token)
    {
    case TOKEN_DIF:
        op = DIF;
        break;
    case TOKEN_PLUS:
        op = SUM;
        break;
    case TOKEN_MINUS:
        op = SUB;
        break;
    case TOKEN_COMP:
        op = IGUAL;
        break; 
    case TOKEN_DIV:
        op = DIV;
        break;
    case TOKEN_MULT:
        op = MULT;
        break;
    case TOKEN_MENOR:
        op = MENOR;
        break;
    case TOKEN_MAIOR:
        op = MAIOR;
        break;
    case TOKEN_MAIORIG:
        op = MAIOR_IGUAL;
        break;
    case TOKEN_MENORIG:
        op = MENOR_IGUAL;
        break;
    default:
        break;
    }
    return op;
}

Quadrupla* new_quadrupla(){
    Quadrupla *novaQuadrupla = (Quadrupla*) malloc(sizeof(Quadrupla));
    if (novaQuadrupla == NULL) printf("Erro ao alocar memória para quádrupla\n");

    novaQuadrupla->op = -1;
    novaQuadrupla->resultado = NULL;
    novaQuadrupla->var1 = NULL;
    novaQuadrupla->var2 = NULL;

    return novaQuadrupla;
}

// cria e aloca memoria para uma nova lista de quadruplas
ListaQuadrupla* new_list_quadrupla() {
    ListaQuadrupla *q = (ListaQuadrupla*) malloc(sizeof(ListaQuadrupla));
    q->first = NULL;
    q->last = NULL;
    return q;
}

ListaParams* new_list_params() {
    ListaParams *p = (ListaParams*) malloc(sizeof(ListaParams));
    p->first = NULL;
    return p;
}

Param* new_param(){
    Param *NewParam = (Param*) malloc(sizeof(Param));
    NewParam->prox = NULL;
    NewParam->value = NULL;

    return NewParam; 
}

void param_insert(ListaParams *params, char* value){
    
    Param *NewParam = new_param();
    NewParam->prox = NULL;
    NewParam->value = strdup(value);
        
    if (params->first != NULL){
        Param* aux = params->first;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = NewParam;
    }else params->first = NewParam;
    
}

/*void libera_params(){
    if (params->first != NULL){
        Param* aux = params->first;
        Param* atual;
        while(aux->prox != NULL){
            printf("desaloca\n");
            atual = aux;
            aux = aux->prox;
            atual->value = NULL;
            printf("desaloca\n");
        }
        aux->value = NULL;
    }else free(params->first);
}*/

// cria e aloca memoria para um novo no de quadrupla 
NodeQuadrupla* new_node_quad() {
    NodeQuadrupla *node = (NodeQuadrupla*) malloc(sizeof(NodeQuadrupla));
    node->quad = new_quadrupla();
    node->prox = NULL;
    return node;
}

void quadrupla_insert(ListaQuadrupla *quads, Operacoes op, char* var1, char* var2, char* resultado){

    NodeQuadrupla* newQuad = new_node_quad();

    newQuad->quad->op = op;
    newQuad->quad->resultado = resultado;
    newQuad->quad->var1 = var1;
    newQuad->quad->var2 = var2;

    if(quads->first != NULL){
        NodeQuadrupla* aux = quads->first;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = newQuad;
        quads->last = newQuad;
    }else{
        quads->first = newQuad;
        quads->last = newQuad;
    }
}

void imprimeListaQuad(FILE* intermediario, ListaQuadrupla *quadruplas){
    NodeQuadrupla* node = quadruplas->first;
    char* op;
    do {
        op = get_operacao(node->quad->op);
        //printf("(%s,%s,%s,%s)\n",op, node->quad->resultado, node->quad->var1, node->quad->var2);
        fprintf(intermediario, "(%s,%s,%s,%s)\n",op, node->quad->resultado, node->quad->var1, node->quad->var2);
        node = node->prox;
    }while(node->prox != NULL);
    op = get_operacao(node->quad->op);
    //printf("(%d,%s,%s,%s)\n",op, node->quad->resultado, node->quad->var1, node->quad->var2);
    fprintf(intermediario, "(%s,%s,%s,%s)\n",op, node->quad->resultado, node->quad->var1, node->quad->var2);
}

void geraInterCode(TreeNode* arvore){
    //Quadrupla *quadrupla;
    TreeNode *exp, *if_part, *else_part, *var1, *var2, *statement;
    TokenType token;
    char *nome, *operacao;
    int t1, t2, label1, label2, v1, v2;
    Operacoes op;

    if (arvore == NULL) return;
    else{
        switch (arvore->tipoID)
        {
        case TOKEN_IF:
            exp = arvore->filhos[0]; 
            if_part = arvore->filhos[1];
            else_part = arvore->filhos[2];

            geraInterCode(exp);

            label1 = label;         //POSSÍVEL ERRO FUTURO (CÓDIGOS MAIORES)
            label++;
            label2 = label;
            label++;

            quadrupla_insert(quads, IFF, get_label(label1), "-", get_reg(t));
            t++;
            geraInterCode(if_part);
            quadrupla_insert(quads, GOTO, "-", "-", get_label(label2));
            quadrupla_insert(quads, LAB, "-", "-", get_label(label1));

            geraInterCode(else_part);
            quadrupla_insert(quads, GOTO, "-", "-", get_label(label2));
            quadrupla_insert(quads, LAB, "-", "-",get_label(label2));

            break;
        case TOKEN_WHILE:
            exp = arvore->filhos[0];
            statement = arvore->filhos[1];

            label1 = label;         //POSSÍVEL ERRO FUTURO (CÓDIGOS MAIORES)
            label++;
            label2 = label;
            label++;

            geraInterCode(exp);

            quadrupla_insert(quads, LAB, "-", "-",get_label(label1));
            quadrupla_insert(quads, IFF, get_label(label2), "-", get_reg(t));
            t++;

            geraInterCode(statement);

            quadrupla_insert(quads, GOTO, "-", "-", get_label(label1));
            quadrupla_insert(quads, LAB, "-", "-",get_label(label2));
            
            break;
        case TOKEN_COMP:            //Operadores relacionais (variáveis já devem estar carregadas num reg)
        case TOKEN_MENOR:
        case TOKEN_DIF:
        case TOKEN_MAIOR:
        case TOKEN_MAIORIG:
        case TOKEN_MENORIG:
            token = arvore->tipoID;
            char *reg1, *reg2;

            op = get_op_token(token);
            var1 = arvore->filhos[0];
            var2 = arvore->filhos[1];

            if (var1->tipoID != TOKEN_INT && var1->filhos[1] == NULL) reg1 = getReg(quads, var1); //Retorna o reg da 1 variável
            else{ 
                reg1 = get_reg(t);
                geraInterCode(var1);
            }
            //printf("reg1:%s\n", reg1);
            //geraInterCode(var1);
            //if (t - t1 > 1) t1 = t - 1;
            if (isnumber){
                v1 = 1;
                isnumber = 0;
            }
            
            operacao = get_operacao(op);
            if (var2->tipoID != TOKEN_INT && var2->filhos[1] == NULL) reg2 = getReg(quads, var2); //Retorna o reg da 2 variável
            else{
                reg2 = get_reg(t);
                geraInterCode(var2);
            }
            //printf("reg2:%s\n", reg2);
            //geraInterCode(var2);
            //if (t - t2 > 1) t2 = t - 1;
            if (isnumber){
                v2 = 1;
                isnumber = 0;
            }

            if (v1 == 1 && v2 == 1){
                quadrupla_insert(quads, op, var1->value, var2->value, get_reg(t));
            }else if (v1 == 1){
                quadrupla_insert(quads, op, var1->value, reg2, get_reg(t));
            }else if (v2 == 1){
                quadrupla_insert(quads, op, reg1, var2->value, get_reg(t));
            }else quadrupla_insert(quads, op, reg1, reg2, get_reg(t));
            if (token == TOKEN_PLUS || token == TOKEN_MINUS || token == TOKEN_MULT || token == TOKEN_DIV) t++;
            break;
        case TOKEN_PLUS:            //Opearadores aritméticos
        case TOKEN_MULT:
        case TOKEN_MINUS:
        case TOKEN_DIV:
            token = arvore->tipoID;
            
            op = get_op_token(token);
            var1 = arvore->filhos[0];
            var2 = arvore->filhos[1];

            t1 = t;
            geraInterCode(var1);
            if (t - t1 > 1) t1 = t - 1;
            if (isnumber){
                v1 = 1;
                isnumber = 0;
            }
            
            operacao = get_operacao(op);

            t2 = t;
            geraInterCode(var2);
            if (t - t2 > 1) t2 = t - 1;
            if (isnumber){
                v2 = 1;
                isnumber = 0;
            }

            if (v1 == 1 && v2 == 1){
                quadrupla_insert(quads, op, var1->value, var2->value, get_reg(t));
            }else if (v1 == 1){
                quadrupla_insert(quads, op, var1->value, get_reg(t2), get_reg(t));
            }else if (v2 == 1){
                quadrupla_insert(quads, op, get_reg(t1), var2->value, get_reg(t));
            }else quadrupla_insert(quads, op, get_reg(t1), get_reg(t2), get_reg(t));
            if (token == TOKEN_PLUS || token == TOKEN_MINUS || token == TOKEN_MULT || token == TOKEN_DIV) t++;
            break;
        case TOKEN_INT:
            //quadrupla_insert(quads, LOAD, arvore->value, "-", get_reg(t));
            //t++;
            isnumber = 1;
            break;
        case TOKEN_ID:
        case 0:
        case 1:
            //casos possíveis com ID faltando (variáveis indexadas)
            if (arvore->ativacao == 2) goto Call;
            if ((strcmp(arvore->tipoDado, "func") == 0 && strcmp(arvore->escopo, "global") == 0)){
                t = 1; //reinicia contagem dos regs quando entra em outra função 
                if (arvore->tipoID == 0) quadrupla_insert(quads, FUN, arvore->value, "-", "void");
                else quadrupla_insert(quads, FUN, arvore->value, "-", "int");

                var1 = arvore->filhos[0];
                var2 = arvore->filhos[1];

                geraInterCode(var1);

                if (strcmp(var1->tipoDado, "arg_void") != 0){ //tratamento para quadruplas de argumentos de funções 
                    Param* aux = params->first;
                    int i = 0;
                    do{
                        if ((i >= args_func && args_func != 0) || (i <= cont_args && args_func == 0)){
                            quadrupla_insert(quads, LOAD, aux->value, "-", get_reg(t));
                            t++;
                            i++;
                            aux = aux->prox;
                        }else{
                            aux = aux->prox;
                            i++;
                        }
                    }while(aux->prox != NULL);
                    args_func = cont_args;
                    //params->first = NULL;
                    //libera_params();
                    //params = new_list_params();

                    quadrupla_insert(quads, LOAD, aux->value, "-", get_reg(t));
                    t++;
                }

                geraInterCode(var2);

                quadrupla_insert(quads, END, "-", "-", arvore->value);
            }else if (strcmp(arvore->tipoDado, "arg") == 0){  //argumento 
                quadrupla_insert(quads, ARG, arvore->value, arvore->escopo, "int");
                param_insert(params, arvore->value);
                cont_args++;
            }else if (arvore->statment == TOKEN_DECL){   //declaração
                char* tamanho = malloc(4*sizeof(char));
                sprintf(tamanho,"%d",arvore->tamanho);
                if (arvore->tamanho != 0) quadrupla_insert(quads, ALLOC, arvore->escopo, tamanho, arvore->value); //vetor
                else quadrupla_insert(quads, ALLOC, arvore->escopo, "-", arvore->value); 
            }else{
                quadrupla_insert(quads, LOAD, arvore->value, "-", get_reg(t));
                t++;
            }
            break;
        case TOKEN_RETURN:
            var1 = arvore->filhos[0];

            if (var1 != NULL) geraInterCode(var1);
            t1 = t - 1;
            quadrupla_insert(quads, RET, "-", "-", get_reg(t1));
            break;
        case TOKEN_CALL:
Call:
            var1 = arvore->filhos[0];
            char* param_char = malloc(4*sizeof(char));

            while (var1 != NULL)
            {
                ativacao = 1;
                geraInterCode(var1); //parâmetros da chamada de função 
                param++;

                t1 = t - 1;
                quadrupla_insert(quads, PARAM, "-", "-", get_reg(t1));
                var1 = var1->irmao;
            }

            ativacao = 0;
            sprintf(param_char,"%d",param);
            quadrupla_insert(quads, CALL, arvore->value, param_char, get_reg(t)); 
            param = 0;
            t++;
            break;
        case TOKEN_ASSIGN:
            var1 = arvore->filhos[0];
            var2 = arvore->filhos[1];
            nome = var1->value;

            t1 = t;
            geraInterCode(var1);
            
            t2 = t;
            geraInterCode(var2);
            if (t - t2 > 1) t2 = t - 1;

            quadrupla_insert(quads, ASSIGN, get_reg(t2), "-", get_reg(t1));
            if (var1->filhos[0] != NULL) quadrupla_insert(quads, STORE, get_reg(t1), get_reg(t1 - 1), nome); //vetor
            else quadrupla_insert(quads, STORE, get_reg(t1), "-", nome);
            break;
        default:
            break;
        }
        
        if (arvore->irmao != NULL && ativacao != 1) {
            geraInterCode(arvore->irmao);
        }
    }
}

ListaQuadrupla* Intermediario(TreeNode* arvore){
    quads = new_list_quadrupla();
    params = new_list_params();

    geraInterCode(arvore);
    quadrupla_insert(quads, HALT, "-", "-", "-");

    return quads;
}