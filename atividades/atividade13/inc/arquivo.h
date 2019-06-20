#ifndef ARQUIVO_H_
#define ARQUIVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_POS(vet, colum_size, i, j) *(vet + (i * colum_size) + j)

enum arquivos_t {
	MATRIZ = 1,
	VETOR = 2,
	RESULTADO = 3
} arquivos_t;

enum status_t {
	SUCESSO = 1,
	ERRO = 0
} status_t;

static size_t tamanho = 10;
static char *lineVet;

char* getNextLine(FILE* file);
double **initMatrix(int line_size, int column_size);
double **lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas);
void imprimirMatriz(double **m, int linhas, int colunas);
void escreverMatriz(char *nomeDoArquivo, double **m, int linhas, int colunas);
void liberarMatriz(double **m);

#endif