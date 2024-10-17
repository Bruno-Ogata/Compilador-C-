%{
#define YYPARSER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#define YYSTYPE TreeNode*

Token token;
static TreeNode *arvore;
static int yylex(void);

// variavel para armazenar o escopo atual
char* escopo;

// constante para armazenar o escopo global
const char* ESCOPO_GLOBAL = "global";

// Função para criar um nó da árvore sintática
TreeNode* createNode(TokenType type, char* value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó da árvore sintática.\n");
        exit(EXIT_FAILURE);
    }
    node->linha = 0;
    node->tamanho = 0;
    node->statment = 0;
    node->ativacao = 0;
    node->tipoID = type;
    node->irmao = NULL;
    node->escopo = NULL;    
    node->tipoDado = NULL;

    if (value != NULL) {
        node->value = strdup(value); 
    } else {
        node->value = NULL;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->filhos[i] = NULL;
    }
    return node;
}

// Função para adicionar os nós irmãos
YYSTYPE add_no_sequencia(YYSTYPE pai, YYSTYPE filho){
    if (pai == NULL) return pai;
    YYSTYPE aux = pai;
    while(aux->irmao != NULL){
        aux = aux->irmao;
    }
    aux->irmao = filho;
    return pai;
}

// Função para adicionar o escopo a todos os nós abaixo do nó fornecido
void add_escopo(YYSTYPE node){
    if (node == NULL) return;

    if (node->escopo == NULL) node->escopo = strdup(escopo);

    // Percorrer os filhos
    for (int i = 0; i < MAX_CHILDREN; i++) {    
        if(node->filhos[i] != NULL) {
            add_escopo(node->filhos[i]);
        }
    }

    // Percorrer os irmãos
    if (node->irmao != NULL) {
        add_escopo(node->irmao);
    }
}

%}

%token TOKEN_ID TOKEN_TIPO TOKEN_INT TOKEN_RCOLCH TOKEN_SEMICOLON TOKEN_LCOLCH TOKEN_LPAREN TOKEN_RPAREN TOKEN_EOF
%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULT TOKEN_DIV TOKEN_ASSIGN TOKEN_COMP TOKEN_MENOR TOKEN_MENORIG TOKEN_MAIOR TOKEN_MAIORIG
%token TOKEN_LCHAVE TOKEN_RCHAVE TOKEN_COLON TOKEN_DIF START COMENTARIO BRANCO DONE SIMBOLO TOKEN_UNKNOWN TOKEN_WHILE TOKEN_IF TOKEN_ELSE TOKEN_RETURN
%token TOKEN_DECL TOKEN_PARAM TOKEN_CALL
%token YYEOF

// Aqui você definirá as regras gramaticais
%%
program             :   lista_decl 
                       {
                            $$ = $1;
                            arvore = $1;
                       }
                    ;
lista_decl          :   lista_decl decl
                        {
                            YYSTYPE nodeDecl = $1;
                            if (nodeDecl == NULL) $$ = $2;
                            else{
                                $$ = nodeDecl;
                                $$ = add_no_sequencia($$, $2);
                            }
                        }
                    |   decl { 
                        $$ = $1;
                     }
                    ;

decl                :   var_decl { 
                        YYSTYPE nodeVarDecl = $1;
                        nodeVarDecl->escopo = strdup(ESCOPO_GLOBAL);
                        $$ = nodeVarDecl; 
                    }
                    |   fun_decl { 
                        YYSTYPE nodeFunc = $1;
                        YYSTYPE temp;
                        int i;

                        add_escopo(nodeFunc);
                        
                        $$ = nodeFunc;
                     }
                    ;

var_decl            :    tipo_especificador identificador TOKEN_SEMICOLON //TIPO INT
                        {	
                            YYSTYPE nodeVar = $2;
                            nodeVar->tipoID = $1->tipoID;
                            nodeVar->tipoDado = "var";
                            nodeVar->statment = TOKEN_DECL;
                            $$ = nodeVar;
                        }
                    |   tipo_especificador identificador TOKEN_LCOLCH num TOKEN_RCOLCH TOKEN_SEMICOLON //TIPO INT (TIPO ID [NUM])
                        {
                            YYSTYPE nodeVarI = $2;
                            nodeVarI->tipoID = $1->tipoID;
                            nodeVarI->tipoDado = "var";
                            nodeVarI->statment = TOKEN_DECL;
                            nodeVarI->tamanho = atoi($4->value);  //NUMERO INDICE (TAMANHO)
                            $$ = nodeVarI;
                        }
                    ;

fun_decl            :    tipo_especificador identificador TOKEN_LPAREN params TOKEN_RPAREN composto_decl //TIPO INT OU VOID
                        {
                            YYSTYPE nodeFunc = $2;
                            nodeFunc->tipoID = $1->tipoID;
                            nodeFunc->tipoDado = "func";
                            nodeFunc->escopo = strdup(ESCOPO_GLOBAL);
                            nodeFunc->filhos[0] = $4;
                            nodeFunc->filhos[1] = $6;
                            escopo = strdup(nodeFunc->value);
                            $$ = nodeFunc;
                        }
                    ;

tipo_especificador  :   TOKEN_TIPO
                        {
                            //printf("teste\n");
                            YYSTYPE nodeTp = createNode(TOKEN_ID, "");
                            if(strcmp("void", token.token)==0) {
                                nodeTp->value = "void";
                                nodeTp->tipoID = 0;
                            } else {
                                nodeTp->value = "int";
                                nodeTp->tipoID = 1;
                            }
                            $$ = nodeTp;
                        }
                    ;

params              :   param_list { $$ = $1; }
                    |   tipo_especificador //TIPO VOID
                        { 
                          $$ = $1;
                          $$->tipoDado = "arg_void";
                          $$->tipoID = TOKEN_TIPO;
						}
                    ;

param_list          :   param_list TOKEN_COLON param
                        {
                            YYSTYPE nodeParams = $1;
                            if (nodeParams == NULL) $$ = $3;
                            else{
                                $$ = nodeParams;
                                $$ = add_no_sequencia($$, $3);
                            }
                        }
                    |   param { $$ = $1; }
                    ;

param               :   tipo_especificador identificador //TIPO INT
                        {
                            YYSTYPE nodeParam = $2;
                            nodeParam->tipoID = $1->tipoID;
                            nodeParam->tipoDado = "arg";
						    $$ = nodeParam;
                        }
                    |   tipo_especificador identificador TOKEN_LCOLCH TOKEN_RCOLCH
                        {
						    YYSTYPE nodeParamI = $2;
                            nodeParamI->tipoID = $1->tipoID;         //TRATAMENTO DE VETOR NO SEMANTICO 
                            nodeParamI->tipoDado = "arg";
						    $$ = nodeParamI;
                        }
                    ;
composto_decl       :   TOKEN_LCHAVE local_decl statement_lista TOKEN_RCHAVE
                        {
                            YYSTYPE nodeCompDecl = $2;
                            if (nodeCompDecl == NULL) $$ = $3;
                            else{
                                $$ = nodeCompDecl;
                                $$ = add_no_sequencia($$, $3);
                            }
                        }
                    |   TOKEN_LCHAVE local_decl TOKEN_RCHAVE { $$ = $2; }
                    |   TOKEN_LCHAVE statement_lista TOKEN_RCHAVE { $$ = $2; }
                    |   TOKEN_LCHAVE TOKEN_RCHAVE  {}
                    ;

local_decl          :   local_decl var_decl
                        {
                            YYSTYPE nodeLocalDecl = $1;
                            if (nodeLocalDecl == NULL) $$ = $2;
                            else{
                                $$ = nodeLocalDecl;
                                $$ = add_no_sequencia($$, $2);
                            }
                        }
                    |   var_decl { $$ = $1; }
                    ;

statement_lista      :   statement_lista statement
                        {
                            YYSTYPE nodeStmtLista = $1;
                            if (nodeStmtLista == NULL) $$ = $2;
                            else{
                                $$ = nodeStmtLista;
                                $$ = add_no_sequencia($$, $2);
                            }
                        }
                    |   statement { $$ = $1; }
                    ;

statement           :   expressao_decl { $$ = $1;  /*comando de expressao*/}
                    |   composto_decl { $$ = $1;    /*comando composto*/} 
                    |   selecao_decl { $$ = $1;   /*comando de selecao*/}
                    |   iteracao_decl { $$ = $1;    /*comando de iteracao*/}
                    |   return_decl { $$ = $1;      /*comando de retorno*/}
                    ;

expressao_decl     :   expressao TOKEN_SEMICOLON { $$ = $1; }
                    |   TOKEN_SEMICOLON {}
                    ;
selecao_decl       :   TOKEN_IF TOKEN_LPAREN expressao TOKEN_RPAREN statement 
                        {
                            YYSTYPE nodeIf = createNode(TOKEN_IF, "if-statement");
                            nodeIf->filhos[0] = $3;
                            nodeIf->filhos[1] = $5;
                            nodeIf->linha = token.linha;
                            $$ = nodeIf;
                        }
                    |   TOKEN_IF TOKEN_LPAREN expressao TOKEN_RPAREN statement TOKEN_ELSE statement
                        {
							YYSTYPE nodeIfElse = createNode(TOKEN_IF, "if-else-statement");
                            nodeIfElse->filhos[0] = $3;
                            nodeIfElse->filhos[1] = $5;
                            nodeIfElse->filhos[2] = $7;
                            nodeIfElse->linha = token.linha;
                            $$ = nodeIfElse;
                        }
                    ;

iteracao_decl       :   TOKEN_WHILE TOKEN_LPAREN expressao TOKEN_RPAREN statement
                        {
                            YYSTYPE nodeWhile = createNode(TOKEN_WHILE, "while-statement");
                            nodeWhile->filhos[0] = $3;
                            nodeWhile->filhos[1] = $5;
                            nodeWhile->linha = token.linha;
                            $$ = nodeWhile;
                        }
                   ;

return_decl        :   TOKEN_RETURN TOKEN_SEMICOLON
                       {    
                            $$ = createNode(TOKEN_RETURN, "return");
                            $$->tipoID = 0;
                            $$->tipoDado = "void";
                       }
                   |   TOKEN_RETURN expressao TOKEN_SEMICOLON
                       {
                            // comando de retorno com expressao
                            YYSTYPE nodeReturn = createNode(TOKEN_RETURN, "return");

                            if($2->tipoID != TOKEN_INT)  {
                                $2->tipoID = 1;
                                $2->statment = TOKEN_RETURN;
                            }
                            //if ($2->ativacao == 1) $2->tipoID = TOKEN_CALL;
                                                        
                            nodeReturn->filhos[0] = $2;
                            nodeReturn->linha = token.linha;
                            nodeReturn->tipoDado = "var";
                            $$ = nodeReturn;
                       }
                   ;

expressao         :   var TOKEN_ASSIGN expressao
                        {
                            // atribuicao de valor a uma variavel
                            YYSTYPE nodeAssign = createNode(TOKEN_ASSIGN, "=");
                            nodeAssign->filhos[0] = $1;
                            if($3->ativacao == 2) $3->ativacao = 1;
                            //if($3->ativacao == 1) $3->tipoID = TOKEN_CALL;
                            nodeAssign->filhos[1] = $3;
                            nodeAssign->linha = token.linha;
                            $$ = nodeAssign;
                        }
                    |   simples_expressao { 
                            $$ = $1; 
                            if($$->ativacao == 1) {
                                $$->ativacao = 2;
                                //$$->tipoID = TOKEN_CALL;
                            }
                        }
                   ;

var                :   identificador { 
                            $$ = $1; /*variavel simples*/ 
                            $$->tipoDado = "var";
                        }

                   |   identificador TOKEN_LCOLCH expressao TOKEN_RCOLCH //VARIAVEL COM EXPR DENTRO DO COLCHETE (OLHAR DPS)
                       {
                            // variavel vetor
                            YYSTYPE nodeVetor = $1;
                            nodeVetor->filhos[0] = $3;
                            nodeVetor->tipoDado = "var";
                            $$ = nodeVetor;
                       }
                    ;

simples_expressao   : soma_expressao relacional soma_expressao
                       {
                            // expressao simples com operador relacional
                            YYSTYPE opRel = $2;
                            opRel->filhos[0] = $1;
                            opRel->filhos[1] = $3;
                            $$ = opRel;
                       }
                    |  soma_expressao { $$ = $1; /*expressao simples sem operador relacional*/ }
                    ;

relacional          :  TOKEN_COMP
                       {    //operador relacional igual
                            $$ = createNode(TOKEN_COMP, "==");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;                  
                       }
                    |  TOKEN_DIF
                       {    //operador relacional diferente
                            $$ = createNode(TOKEN_DIF, "!=");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;                      
                       }
                    |  TOKEN_MENOR
                       {    //operador relacional menor
                            $$ = createNode(TOKEN_MENOR, "<");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
                       }
                    |  TOKEN_MENORIG
                       {    //operador relacional menor ou igual
                            $$ = createNode(TOKEN_MENORIG, "<=");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
                       }
                    |  TOKEN_MAIOR
                       {    //operador relacional maior
                            $$ = createNode(TOKEN_MAIOR, ">");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
                       }
                    |  TOKEN_MAIORIG
                       {    //operador relacional maior ou igual
                            $$ = createNode(TOKEN_MAIORIG, ">=");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
                       }
                    ;

soma_expressao      :  soma_expressao soma termo
                       {    // expressao de soma ou subtracao
                            YYSTYPE opAd = $2;
                            opAd->filhos[0] = $1;
                            opAd->filhos[1] = $3;
                            $$ = opAd;
                       }
                    |  termo { $$ = $1; /*expressao de soma sem operador de soma*/}
                    ;

soma                 : TOKEN_PLUS
                       {    //operador de soma
                            $$ = createNode(TOKEN_PLUS, "+");
                            $$->value = strdup(token.token);                
                            $$->linha = token.linha;
                       }
                    |  TOKEN_MINUS
                       {    //operador de subtracao
                            $$ = createNode(TOKEN_MINUS, "-");
                            $$->value = strdup(token.token);                        
                            $$->linha = token.linha;
                       }
                    ;
termo                :  termo mult fator
                       {    // expressao de multiplicacao ou divisao
                            YYSTYPE opMult = $2;
                            opMult->filhos[0] = $1;
                            opMult->filhos[1] = $3;
                            $$ = opMult;
                       }
                   |   fator { $$ = $1; }
                    ;

mult                :  TOKEN_MULT
                       {    //operador de multiplicacao
                            $$ = createNode(TOKEN_MULT, "*");
                            $$->value = strdup(token.token);                         
                            $$->linha = token.linha;
                       }
                   |   TOKEN_DIV
                       {    //operador de divisao
                            $$ = createNode(TOKEN_DIV, "/");
                            $$->value = strdup(token.token);                        
                            $$->linha = token.linha;
                       }
                    |
                    ;
fator              :  TOKEN_LPAREN expressao TOKEN_RPAREN
                       {    // expressao entre parenteses
                            $$ = $2;
                       }
                   |   var
                       {    // variavel
                            $$ = $1;
                       }
                   |   ativacao
                       {    // chamada de funcao
                            $$ = $1;
                            $$->ativacao = 1; 
                       }
                   |   num
                       {    // numero
                            $$ = $1;
                       }
                    ;
ativacao            :  identificador TOKEN_LPAREN arg_list TOKEN_RPAREN
                       {    // chamada de funcao com argumentos
                            YYSTYPE ativacao = $1;
                            ativacao->tipoDado = "func";
                            ativacao->tipoID = TOKEN_CALL;
                            ativacao->ativacao = 1;
                            ativacao->filhos[0] = $3;
                            $$ = ativacao;
                       }
                    |  identificador TOKEN_LPAREN TOKEN_RPAREN 
					   {    // chamada de funcao sem argumentos
                            YYSTYPE ativacaoSem = $1;
                            ativacaoSem->tipoDado = "func";
                            ativacaoSem->tipoID = TOKEN_CALL;
                            ativacaoSem->ativacao = 1;
                            $$ = ativacaoSem;
                       }   
                    ;
arg_list            :  arg_list TOKEN_COLON expressao
                       {    // lista de argumentos
                            YYSTYPE nodeArgsLista = $1;
                            if (nodeArgsLista == NULL) $$ = $3;
                            else{
                                $$ = nodeArgsLista;
                                $$ = add_no_sequencia($$, $3);
                            }
                        }
                    |   expressao
                        {   // argumento
                            $$ = $1;
                            //$$->tipoID = TOKEN_PARAM;
                        }
                    ;
identificador       :   TOKEN_ID
                        {   // identificador
                            $$ = createNode(TOKEN_ID, "id");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
                        }
                    ;
num                 :   TOKEN_INT
                        {   // numero
                            $$ = createNode(TOKEN_INT, "num");
                            $$->value = strdup(token.token);
                            $$->linha = token.linha;
						}
                    ;

%%

// Função para obter o próximo token
static int yylex(void){
    token = get_token();
    return token.tipo;
}

// Função para tratar erros
void yyerror(const char *msg) {
    Token aux;
    aux = get_previous_token();

    if(token.tipo == 266) return;
    if(aux.linha != token.linha) { // SE TOKEN PROX.LINHA != TOKEN ENTÃO CONSIDERAR LINHA ANTERIOR
        token.linha--;
        printf("ERRO SINTATICO: LINHA %d - erro sintatico proximo a %s\n", token.linha, token.token);
    }
    else printf("ERRO SINTATICO: LINHA %d - erro sintatico proximo a %s\n", token.linha, token.token);
    exit(1);
}

void imprimir_arvore(FILE *bison, TreeNode* node, int nivel) {
    if (node == NULL) {
        return;
    }

    // Indentar de acordo com o nível para uma saída mais legível
    for (int i = 0; i < nivel; i++) {
        fprintf(bison, "   ");
    }

    // Imprimir o valor do nó (token ou nome)
    if (node->tipoID == TOKEN_ID || node->tipoID == 259 || node->tipoID == 260 || node->tipoID == 0 || node->tipoID == 1 || node->tipoID == TOKEN_RETURN || node->tipoID == TOKEN_IF || node->tipoID == TOKEN_WHILE || node->tipoID == TOKEN_MENOR || node->tipoID == TOKEN_COMP || node->tipoID == TOKEN_MENORIG || node->tipoID == TOKEN_MAIOR || node->tipoID == TOKEN_MAIORIG || node->tipoID == TOKEN_DIF || node->tipoID == TOKEN_MULT || node->tipoID == TOKEN_MINUS || node->tipoID == TOKEN_PLUS || node->tipoID == TOKEN_DIV || node->tipoID == TOKEN_CALL || node->tipoID == TOKEN_ASSIGN) {
        if(node->escopo != NULL) {
            fprintf(bison, "%s ESCOPO: %s - LINHA: %d\n", node->value, node->escopo, node->linha);
        } else {
            fprintf(bison, "%s\n", node->value);
        }
    } else {
        fprintf(bison, "%s\n", token_type_to_string(node->tipoID));
    }

    // Percorrer os filhos e imprimir recursivamente
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if(node->filhos[i] != NULL) {
            imprimir_arvore(bison, node->filhos[i], nivel + 1);
        }
    }

    // Percorrer os irmãos e chamar a função para imprimir seus filhos
    if (node->irmao != NULL) {
        imprimir_arvore(bison, node->irmao, nivel);
    }
}

// Função auxiliar para converter um tipo de token em uma string
const char* token_type_to_string(int type) {
    switch (type) {
        case TOKEN_ID:
            return "TOKEN_ID";
        case TOKEN_INT:
            return "TOKEN_NUM";
        // Outros casos de tipo de token, se necessário
        default:
            return "UNKNOWN_TOKEN";
    }
}

// Função para liberar a memória alocada para a árvore sintática
void libera_arvore(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    // Percorrer os filhos e liberar recursivamente
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if(node->filhos[i] != NULL) {
            libera_arvore(node->filhos[i]);
        }
    }

    // Percorrer os irmãos e chamar a função para liberar seus filhos
    if (node->irmao != NULL) {
        libera_arvore(node->irmao);
    }

    // Liberar nó atual
    free(node);
    node = NULL;
}

// Função principal
TreeNode* parser() {
    yyparse();
    return arvore;
}

