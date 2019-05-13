#include "arquivo.h"

float *initMatrix(int line_size, int column_size){
    float *matrix;
    int i, j;
    matrix = malloc(line_size * column_size * sizeof(float));

    for(i = 0; i < line_size; i++)
        for(j = 0; j < column_size; j++)
            MATRIX_POS(matrix, column_size, i, j) = 0;

    return matrix;
}

float *lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas){
    int i, j;
    float *matrix;

    *linhas = atoi(getNextLine(arquivoMatriz));
    *colunas = atoi(getNextLine(arquivoMatriz));

    matrix = initMatrix(*linhas, *colunas);

	size_t lineLen = *colunas;
	char *line = malloc(lineLen);
    char *element = malloc(lineLen);

    for(i = 0;i < *linhas; i++){
		line = getNextLine(arquivoMatriz);
		element = strtok(line, ":");
		for(j = 0; j < *colunas; j++){
			if(element)
				MATRIX_POS(matrix, *colunas, i, j) = atof(element);
			element = strtok('\0', ":");
		}
	}
    
//    free(line);
  //  free(element);
    
    return matrix;

}

void imprimirMatriz(float *m, int linhas, int colunas){
    int i, j;
    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            printf(" %f |", MATRIX_POS(m, colunas, i, j));
        }
        printf("\n");
    }

}

void escreverMatriz(char *nomeDoArquivo, float *m, int linhas, int colunas){
    int i, j;
    FILE *matrixOut;
    matrixOut = fopen(nomeDoArquivo, "wt");

    if(matrixOut != NULL){
    	fprintf(matrixOut, "%d\n", linhas);
    	fprintf(matrixOut, "%d\n", colunas);
        for(i = 0; i < linhas; i++){
            for(j = 0; j < colunas; j++){
                if(j == colunas - 1)
                    fprintf(matrixOut, "%.2f\n", MATRIX_POS(m, colunas, i, j));
                else
                    fprintf(matrixOut, "%.2f:", MATRIX_POS(m, colunas, i, j));
            }
        }
    }

}

void liberarMatriz(float *m){
    if(m != NULL)
        free(m);
}

char* getNextLine(FILE* file){	
	getline(&lineVet, &tamanho, file);
	return lineVet;
}
