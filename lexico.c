/*
    Projeto de Compiladores 2023.2

    Compilador C-

    Alunos: Bruno Hideki Amadeu Ogata RA: 140884
            Mateus Campanha Pereira RA: 147884

*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "parser.tab.h"

Token tabelaTransicao[400][128];
int token_indice = 0;
int i = 0;

Lista token_buffer;
Token *token_atual;

//Inicializa a lista
void inicializa_lista() {
	token_buffer.topo = NULL;
}

//Novo elemento na lista
void lisa_token(Lista *lista, Token token) {
	Token *novo = (Token *) malloc(sizeof(Token));
	novo->tipo = token.tipo;
	strcpy(novo->token, token.token);
	novo->proxEstado = token.proxEstado;
	novo->linha = token.linha;
	novo->prox = NULL;
	novo->ant = NULL;

	if (lista->topo == NULL) {
		lista->topo = novo;
	} else {
		Token *aux = lista->topo;
		while (aux->prox != NULL) {
			aux = aux->prox;
		}
		aux->prox = novo;
		novo->ant = aux;
	}
}

// Libera a lista
void libera_lista() {
	Token *aux = token_buffer.topo;
	while (aux != NULL) {
		Token *aux2 = aux->prox;
		free(aux);
		aux = aux2;
	}
}

// Aloca um novo token
Token allocate_token(int tam) {
  Token Token;
  //Token.token = (char *)malloc(tam * sizeof(char));
  Token.tipo = TOKEN_UNKNOWN;
  Token.proxEstado = 0;
  Token.linha = 1;
  Token.prox = NULL;
  Token.ant = NULL;

  return Token;
}

// Desaloca um token
void deallocate_token(Token *Token) {
  free(Token->token);
  Token->tipo = TOKEN_UNKNOWN;
}


// inicializa a tabela hash de palavras reservadas
TabelaHashReservadas hashTable[HASH_TABLE_SIZE];
char* palavrasReservadas[6] = {"if", "while", "else", "return", "void", "int"};
int tokensReservados[6] = {TOKEN_IF, TOKEN_WHILE, TOKEN_ELSE, TOKEN_RETURN, TOKEN_TIPO, TOKEN_TIPO};

// função de hash
unsigned int hash(char* palavra, int TABLE_SIZE) {
	unsigned int hash = 0;
    while (*palavra) {
        hash = (hash * 97) + (*palavra);
        palavra++;
    }
    return hash % TABLE_SIZE;
}

// cria a tabela hash de palavras reservadas
void criaTabelaHash () {
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		hashTable[i].palavra = (char *) malloc(7 * sizeof(char));
	}
	//preenche tabela com as palavras reservadas
	int indice;
	for(int i = 0; i < 6; i++){
		indice = hash(palavrasReservadas[i], HASH_TABLE_SIZE);
		strcpy(hashTable[indice].palavra, palavrasReservadas[i]);
		hashTable[indice].tipo = tokensReservados[i];
	}
}

// checa se o token é uma palavra reservada
void checaHashReservadas(Token *Token) {
	int indice = hash(Token->token, HASH_TABLE_SIZE);
	
	if (strcmp(hashTable[indice].palavra, Token->token) == 0) {
		Token->tipo = hashTable[indice].tipo;
	}
}

// imprime o token no arquivo de saída lexico.txt
void imprimeToken(FILE *lexico, Token *Token){

  switch (Token->tipo)
  {
  case TOKEN_TIPO:
    fprintf(lexico, "Linha: %d Tipo: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_ID:
    fprintf(lexico, "Linha: %d Identificador: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_INT:
    fprintf(lexico, "Linha: %d Digito: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_ASSIGN:
  case TOKEN_DIV:
  case TOKEN_MINUS:
  case TOKEN_MULT:
  case TOKEN_PLUS:
    fprintf(lexico, "Linha: %d Operador: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_ELSE:
  case TOKEN_RETURN:
  case TOKEN_IF:
  case TOKEN_WHILE:
    fprintf(lexico, "Linha: %d Palavra Reservada: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_COMP:
  case TOKEN_LCHAVE:
  case TOKEN_LCOLCH:
  case TOKEN_LPAREN:
  case TOKEN_MAIOR:
  case TOKEN_MAIORIG:
  case TOKEN_MENOR:
  case TOKEN_MENORIG:
  case TOKEN_RCHAVE:
  case TOKEN_RCOLCH:
  case TOKEN_RPAREN:
  case TOKEN_COLON:
  case TOKEN_SEMICOLON:
    fprintf(lexico, "Linha: %d Simbolo: %s\n", Token->linha, Token->token);
    break;
  case TOKEN_EOF:
    fprintf(lexico, "\nFIM DO ARQUIVO\n");
    break;
  default:
    fprintf(stdout, "ERRO LEXICO: %s - Linha: %d\n", Token->token, Token->linha);  
	fclose(lexico);
	exit(1);
    break;
  }
}

// inicializa a tabela de transições
void initTabelaTransicoes(){
  	for (int i = 258; i < 350; i++) {
    	for (int j = 0; j < 128; j++) {
      		tabelaTransicao[i][j].proxEstado = DONE;
    	}
  	}

  	//transições para ids e nums
  	for (char c = 'a'; c <= 'z'; c++) {
    	tabelaTransicao[START][c].proxEstado = TOKEN_ID; tabelaTransicao[START][c].tipo = TOKEN_ID;
  	}
  	for (char c = 'A'; c <= 'Z'; c++) {
    	tabelaTransicao[START][c].proxEstado = TOKEN_ID; tabelaTransicao[START][c].tipo = TOKEN_ID;
  	}
  	for (char c = 'a'; c <= 'z'; c++) {
    	tabelaTransicao[TOKEN_ID][c].proxEstado = TOKEN_ID; tabelaTransicao[START][c].tipo = TOKEN_ID;
  	}
  	for (char c = 'A'; c <= 'Z'; c++) {
   		tabelaTransicao[TOKEN_ID][c].proxEstado = TOKEN_ID; tabelaTransicao[TOKEN_ID][c].tipo = TOKEN_ID;
  	}
  	for (char c = '0'; c <= '9'; c++) {
    	tabelaTransicao[START][c].proxEstado = TOKEN_INT; tabelaTransicao[START][c].tipo = TOKEN_INT;
  	}
  	for (char c = '0'; c <= '9'; c++) {
		tabelaTransicao[TOKEN_INT][c].proxEstado = TOKEN_INT; tabelaTransicao[TOKEN_INT][c].tipo = TOKEN_INT;
	}
	
	//transições para os comentarios
	tabelaTransicao[START]['/'].proxEstado = TOKEN_DIV;
	tabelaTransicao[START]['/'].tipo = TOKEN_DIV;
	tabelaTransicao[TOKEN_DIV]['*'].proxEstado = COMENTARIO;
	tabelaTransicao[COMENTARIO]['/'].proxEstado = START;
	
	//transição para fim de arquivo
	tabelaTransicao[START][EOF].proxEstado = DONE; tabelaTransicao[START][EOF].tipo = TOKEN_EOF;
	
	//transições para espaços em branco
	tabelaTransicao[START][' '].proxEstado = BRANCO; tabelaTransicao[BRANCO][' '].proxEstado = BRANCO;
	tabelaTransicao[START]['\t'].proxEstado = BRANCO; tabelaTransicao[BRANCO]['\n'].proxEstado = BRANCO;
	tabelaTransicao[START]['\n'].proxEstado = BRANCO; tabelaTransicao[BRANCO]['\t'].proxEstado = BRANCO;
	
	//transições para símbolos únicos
	tabelaTransicao[START]['+'].proxEstado = DONE; tabelaTransicao[START]['+'].tipo = TOKEN_PLUS;
	tabelaTransicao[START]['-'].proxEstado = DONE; tabelaTransicao[START]['-'].tipo = TOKEN_MINUS;
	tabelaTransicao[START]['*'].proxEstado = DONE; tabelaTransicao[START]['*'].tipo = TOKEN_MULT;
	tabelaTransicao[START]['('].proxEstado = DONE; tabelaTransicao[START]['('].tipo = TOKEN_LPAREN;
	tabelaTransicao[START]['['].proxEstado = DONE; tabelaTransicao[START]['['].tipo = TOKEN_LCOLCH;
	tabelaTransicao[START]['{'].proxEstado = DONE; tabelaTransicao[START]['{'].tipo = TOKEN_LCHAVE;
	tabelaTransicao[START][')'].proxEstado = DONE; tabelaTransicao[START][')'].tipo = TOKEN_RPAREN;
	tabelaTransicao[START][']'].proxEstado = DONE; tabelaTransicao[START][']'].tipo = TOKEN_RCOLCH;
  	tabelaTransicao[START]['}'].proxEstado = DONE; tabelaTransicao[START]['}'].tipo = TOKEN_RCHAVE;
	tabelaTransicao[START][';'].proxEstado = DONE; tabelaTransicao[START][';'].tipo = TOKEN_SEMICOLON;
	tabelaTransicao[START][','].proxEstado = DONE; tabelaTransicao[START][','].tipo = TOKEN_COLON;
	
	//transições com mais de um símbolo (ex ==)
	tabelaTransicao[START]['='].proxEstado = TOKEN_ASSIGN;
  	tabelaTransicao[START]['='].tipo = TOKEN_ASSIGN;
	tabelaTransicao[TOKEN_ASSIGN]['='].proxEstado = TOKEN_COMP;
  	tabelaTransicao[TOKEN_ASSIGN]['='].tipo = TOKEN_COMP;
	tabelaTransicao[START]['<'].proxEstado = TOKEN_MENOR;
	tabelaTransicao[START]['<'].tipo = TOKEN_MENOR;
	tabelaTransicao[TOKEN_MENOR]['='].proxEstado = TOKEN_MENORIG;
	tabelaTransicao[TOKEN_MENOR]['='].tipo = TOKEN_MENORIG;
  	tabelaTransicao[START]['>'].proxEstado = TOKEN_MAIOR;
	tabelaTransicao[START]['>'].tipo = TOKEN_MAIOR;
	tabelaTransicao[TOKEN_MAIOR]['='].proxEstado = TOKEN_MAIORIG;
	tabelaTransicao[TOKEN_MAIOR]['='].tipo = TOKEN_MAIORIG;
  	tabelaTransicao[START]['!'].proxEstado = TOKEN_DIF;
	tabelaTransicao[START]['!'].tipo = TOKEN_UNKNOWN;
  	tabelaTransicao[TOKEN_DIF]['='].proxEstado = TOKEN_DIF;
	tabelaTransicao[TOKEN_DIF]['='].tipo = TOKEN_DIF;
}

// função principal que realiza a análise léxica
Token getNextToken(FILE *file, Token *token) {
    TokenType estado = START;
	Token transicao;
    char tokenValue[256];
    int tokenValueindice = 0;

    int c = fgetc(file);
	int temp = c;
	int temp_tipo;

    while (estado != DONE) {
Start:
        transicao = tabelaTransicao[estado][c];
        switch (estado) {
			case START:
				if (transicao.proxEstado == TOKEN_DIV) {
					temp = c;
					c = fgetc(file);
				}
				if (transicao.proxEstado == DONE) {
					tokenValue[tokenValueindice++] = (char)c;
					c = fgetc(file);
				}
				break;
            case COMENTARIO:
Repete:
				do{
					if (c == '\n') token->linha++;
					c = fgetc(file);
				} while(c != '*');
					c = fgetc(file);
					if(c != '/') goto Repete;
					else {
						c = fgetc(file);
						estado = START;
						goto Start;
					}
                break;
            case BRANCO:
				while (isspace(c)){
					if (c == '\n') token->linha++;
					c = fgetc(file);
				}
                estado = START;
				goto Start;
                break;
            case TOKEN_ID:
				while(isalpha(c)){
					c = fgetc(file);
					if (isalpha(c)) tokenValue[tokenValueindice++] = (char)c;
				}
                transicao.proxEstado = DONE;
                transicao.tipo = TOKEN_ID;
                break;
            case TOKEN_INT:
                while(isdigit(c)){
					c = fgetc(file);
					if (isdigit(c)) tokenValue[tokenValueindice++] = (char)c;
				}
                transicao.proxEstado = DONE;
                transicao.tipo = TOKEN_INT;
                break;
            case TOKEN_DIV:
                transicao = tabelaTransicao[estado][c];
                if (transicao.proxEstado != COMENTARIO){
                  	//ungetc(c, file);
					c = fgetc(file);	//ATENÇÃO AQUI 
                  	tokenValue[tokenValueindice++] = (char)temp;
                  	transicao.tipo = TOKEN_DIV;
                  	transicao.proxEstado = DONE;
                }
                break;  
            case TOKEN_MENOR:
			case TOKEN_MAIOR:
			case TOKEN_ASSIGN:
			case TOKEN_DIF:
				temp_tipo = estado;
				c = fgetc(file);
                transicao = tabelaTransicao[estado][c];
                if (transicao.proxEstado == TOKEN_COMP || transicao.proxEstado == TOKEN_MAIORIG || transicao.proxEstado == TOKEN_MENORIG || transicao.proxEstado == TOKEN_DIF){
                  	tokenValue[tokenValueindice++] = (char)c;
				  	c = fgetc(file);
					transicao.proxEstado = DONE;
                }else{
                  transicao.tipo = temp_tipo;
				  transicao.proxEstado = DONE;
                }
                break;
        }
		if (transicao.proxEstado != BRANCO){
			if (transicao.proxEstado != DONE && transicao.proxEstado != COMENTARIO && transicao.proxEstado != TOKEN_DIV) {
            	tokenValue[tokenValueindice++] = (char)c;
        	} else {
            	ungetc(c, file);  // Devolve o último caractere não consumido.
				token->tipo = transicao.tipo;
        	}
		}
        estado = transicao.proxEstado;
    }

    strncpy(token->token, tokenValue, tokenValueindice);
    token->token[tokenValueindice++] = '\0';

    if (token->tipo == TOKEN_ID) checaHashReservadas(token);

	lisa_token(&token_buffer, *token);

    return *token;
}

// retorna o próximo token da lista
Token get_token(){
	static int primeiro = 1;
	if (primeiro && token_buffer.topo != NULL) {
		token_atual = token_buffer.topo;
		primeiro = 0;
	} else if (token_atual->prox != NULL) {
		token_atual = token_atual->prox;
	}
	return *token_atual;
}

// retorna o token anterior da lista
Token get_previous_token(){
  
  	if (token_atual->ant != NULL) {
		token_atual = token_atual->ant;
	}
	return *token_atual;
}