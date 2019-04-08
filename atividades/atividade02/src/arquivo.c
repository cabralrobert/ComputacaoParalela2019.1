#include "arquivo.h"

void init(){
	linha = malloc(tamanho);
	coluna = malloc(tamanho);
	vetorLenAux = malloc(tamanho);
	lineVet = malloc(tamanho);
}

void desaloc(){
	if (linha) 
		free(linha);
	if (coluna) 
		free(coluna);
	if (vetorLenAux) 
		free(vetorLenAux);
	if (lineVet) 
		free(vetorLenAux);
}

int abrirArquivos(char* matrizName, char* vetorName, char* resultName){
	fileMatriz = fopen(matrizName,"rt");
	fileVetor = fopen(vetorName, "rt");
	fileResultado = fopen(resultName, "wt");

	if(fileMatriz == NULL || fileVetor == NULL || fileResultado == NULL)
		return ERRO;
	return SUCESSO;
}

int getRow(){
	if(fileMatriz == ERRO)
		return ERRO;

	getline(&linha, &tamanho, fileMatriz);	
	return atoi(linha);
}

int getColumn(){
	if(fileMatriz == ERRO)
		return ERRO;

	getline(&coluna, &tamanho, fileMatriz);
	return atoi(coluna);	
}

int getVetLen(){
	if(fileVetor == ERRO)
		return ERRO;

	getline(&vetorLenAux, &tamanho, fileVetor);
	return atoi(vetorLenAux);
}

char* getNextLine(FILE* file){	
	getline(&lineVet, &tamanho, file);
	return lineVet;
}
