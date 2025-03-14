#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>
#include "arquivo.h"


int main(int argc, char* argv[]){

	if(!argc == 4)
		return -1;

    FILE* matrix, *vector;

    int linhasMatrix, colunasMatrix;
    matrix = fopen(argv[MATRIZ], "rt");
    float* matrixRes = lerMatriz(matrix, &linhasMatrix, &colunasMatrix);

    int linhasVector, colunasVector;
    vector = fopen(argv[VETOR], "rt");
    float* vectorRes = lerMatriz(vector, &linhasVector, &colunasVector);

    if(matrixRes == NULL || vectorRes == NULL){
        printf("Erro ao abrir o arquivo");
        return -1;
    }

    // verifica se pode realizar a multiplicação
	if(colunasMatrix != linhasVector){
		printf("Numero de colunas diferente do tamanho do vetor.\n");
		return -1;
	}
    
    // calcula a multiplicação das matrizes
    int linhasResult = linhasMatrix, colunasResult = colunasVector;
    float* vetRes = initMatrix(linhasMatrix, colunasVector);

    int i, j, z;

    for(i = 0; i < linhasResult; i++)
		for(j = 0; j < colunasResult;j++)
            for(z = 0; z < colunasMatrix; z++)
    		    MATRIX_POS(vetRes, colunasResult, i, j) +=  MATRIX_POS(matrixRes, colunasMatrix, i, z) * MATRIX_POS(vectorRes, colunasVector, z, j);

    escreverMatriz(argv[RESULTADO], vetRes, linhasResult, colunasResult);

    liberarMatriz(matrixRes);
    liberarMatriz(vectorRes);
    liberarMatriz(vetRes);

}
