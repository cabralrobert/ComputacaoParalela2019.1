#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>
#include "arquivo.h"


int main(int argc, char* argv[]){

	if(!argc == 4)
		return -1;

	init();

	if(abrirArquivos(argv[MATRIZ], argv[VETOR], argv[RESULTADO]) == ERRO)
		return -1;

	int row, column, vetLen;
	row = getRow();
	column = getColumn();
	vetLen = getVetLen();

	// como somente pode-se multiplicar uma matriz por vetor se A[i][j] * V[j], então:
	if(vetLen != column){
		printf("Numero de colunas diferente do tamanho do vetor.\n");
		return -1;
	}

	// criação da matriz e dos vetores
	float matriz[row][column];
	float vetor[vetLen], vetResult[row];
	int i, j;

	// variavel para pegar tratar arquivo
	size_t lineLen = column;
	char *line = malloc(lineLen);

	// elemento colhido do arquivo salvada nesta variavel
	char* element;

	// preenchendo matriz com o valor do arquivo
	for(i = 0;i < row; i++){
		line = getNextLine(fileMatriz);
		element = strtok(line, ":");
		for(j = 0; j < column; j++){
			if(element)	
				matriz[i][j] = atof(element);
			element = strtok('\0', ":");

		}
	}

	// preenchendo o vetor com o valor do arquivo
	line = getNextLine(fileVetor);
	element = strtok(line, ":");

	for(i = 0; i < vetLen; i++){
		if(element)
			vetor[i] = atof(element);
		element = strtok('\0', ":");
	}	

	// inicializando o vetor de resultados
	for(i = 0; i < row; i++)
		vetResult[i] = 0;


	// calculo de resultado
	for(i = 0; i < row; i++){
		for(j = 0; j < column;j++){
			vetResult[i] += matriz[i][j] * vetor[j];
		}
	}


	fprintf(fileResultado, "%d\n", row);
	for(i = 0; i < row; i++){
		if(i == row-1)
			fprintf(fileResultado, "%.2f\n", vetResult[i]);
		else
			fprintf(fileResultado, "%.2f:", vetResult[i]);
	}

	desaloc();	

}
