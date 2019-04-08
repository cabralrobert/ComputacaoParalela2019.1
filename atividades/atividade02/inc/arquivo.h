#ifndef ARQUIVO_H_
#define ARQUIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum arquivos_t {
	MATRIZ = 1,
	VETOR = 2,
	RESULTADO = 3
} arquivos_t;

enum status_t {
	SUCESSO = 1,
	ERRO = 0
} status_t;

FILE *fileMatriz, *fileVetor, *fileResultado;

static size_t tamanho = 10;
static char *linha, *coluna, *vetorLenAux, *lineVet;

void init();
void desaloc();
int abrirArquivos(char* matrizName, char* vetorName, char* resultName);
int getRow();
int getColumn();
int getVetLen();
char* getNextLine(FILE* file);

#endif
