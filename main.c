/*
    Projeto de Compiladores 2023.2

    Compilador C-

    Alunos: Bruno Hideki Amadeu Ogata RA: 140884
            Mateus Campanha Pereira RA: 147884

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binario.h"

int TABLE_SIMBOL_SIZE = 0;

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Quantidade errada de argumentos!\nUso: ./main <nome_arquivo>");
        return 1;
    }

    char* filename = argv[1];

    FILE* fileLex = fopen(filename, "r");
    FILE *bison = fopen("bison.txt", "w");
    FILE* lexico = fopen("lexico.txt", "w");
    FILE* semantico = fopen("semantico.txt", "w");
    FILE* intermediario = fopen("intermediario.txt", "w");
    FILE* assembly = fopen("assembly.txt", "w");
    FILE* binario = fopen("binario.txt", "w");
    FILE* teste_processador = fopen("teste_processador.txt", "w");

    if (fileLex == NULL) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }
    if (lexico == NULL) {
        printf("Erro ao abrir o arquivo de saida do Analisador Lexico!\n");
        return 1;
    }
    if (bison == NULL) {
        printf("Erro ao abrir o arquivo de saida do Analisador Sintatico!\n");
        return 1;
    }
    if (semantico == NULL) {
        printf("Erro ao abrir o arquivo de saida do Analisador Semantico!\n");
        return 1;
    }
    if (intermediario == NULL) {
        printf("Erro ao abrir o arquivo de saida do Gerador de código intermediário!\n");
        return 1;
    }
    if (assembly == NULL) {
        printf("Erro ao abrir o arquivo de saida do Gerador de código assembly!\n");
        return 1;
    }
    if (binario == NULL) {
        printf("Erro ao abrir o arquivo de saida do Gerador de código binário!\n");
        return 1;
    }
    if (teste_processador == NULL) {
        printf("Erro ao abrir o arquivo de saida do Gerador de código binário!\n");
        return 1;
    }

    // montar tabela hash de palavras reservadas
    criaTabelaHash();
    
    // inicializar tabela de transicoes
    initTabelaTransicoes();

    // inicializar lista de tokens
    inicializa_lista();

    Token token = allocate_token(256);
    TreeNode* arvore;
    ListaQuadrupla* quadruplas = new_list_quadrupla();
    
    // parar while quando chegar ao fim do arquivo de entrada
    while(!feof(fileLex)){
        // obtem o proximo token do Analisador Lexico
        token = getNextToken(fileLex, &token);

        // determina o tamanho da tabela de simbolos
        if (token.tipo == 258) TABLE_SIMBOL_SIZE++;

        imprimeToken(lexico, &token);
    }

    // ativa o Analisador Sintatico
    arvore = parser();

    imprimir_arvore(bison, arvore, 1);

    TabelaHashSimbolos *tabelaSimbolos = criaTabelaSimbolos(3 * TABLE_SIMBOL_SIZE);

    preencheTabelaSimbolos(tabelaSimbolos, arvore);

    imprimeTabelaSimbolos(semantico, tabelaSimbolos);
    analise_semantico(semantico, tabelaSimbolos);
    
    imprimeTabela(semantico, tabelaSimbolos);

    ListaQuadrupla *quads;

    quads = Intermediario(arvore);
    imprimeListaQuad(intermediario, quads);

    ListaInstrucao *instrucoes;

    if(quads->first != NULL){
        instrucoes = Assembly(quads);
        imprimeListaInstrucoes(assembly, instrucoes);
        imprimir_labels(assembly);
    }

    ListaBinario* instrucoesBin;

    if(instrucoes->first != NULL){
        instrucoesBin = BinarioCode(instrucoes);
        imprimeListaBinario(binario, teste_processador, instrucoesBin);
    }
   
    //deallocate_token(&token); // REVISAR FUNÇÃO
    
    libera_lista();
    libera_arvore(arvore);
    libera_tabela(tabelaSimbolos);
    
    fclose(fileLex);
    fclose(lexico);
    fclose(bison);
    fclose(semantico);
    fclose(intermediario);
    fclose(assembly);
    fclose(binario);

    return 0;
}