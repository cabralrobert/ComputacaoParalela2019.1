#include "arquivo.h"

double *initMatrix(int line_size, int column_size){
    double *matrix;
    int i, j;
    matrix = malloc(line_size * column_size * sizeof(double));

    for(i = 0; i < line_size; i++)
        for(j = 0; j < column_size; j++)
            MATRIX_POS(matrix, column_size, i, j) = 0;

    return matrix;
}

double *lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas){
    int i, j;
    double *matrix;

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

void imprimirMatriz(double *m, int linhas, int colunas){
    int i, j;
    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            printf(" %lf |", MATRIX_POS(m, colunas, i, j));
        }
        printf("\n");
    }

}

void escreverMatriz(char *nomeDoArquivo, double *m, int linhas, int colunas){
    int i, j;
    FILE *matrixOut;
    matrixOut = fopen(nomeDoArquivo, "wt");

    if(matrixOut != NULL){
    	fprintf(matrixOut, "%d\n", linhas);
    	fprintf(matrixOut, "%d\n", colunas);
        for(i = 0; i < linhas; i++){
            for(j = 0; j < colunas; j++){
                if(j == colunas - 1)
                    fprintf(matrixOut, "%.4lf\n", MATRIX_POS(m, colunas, i, j));
                else
                    fprintf(matrixOut, "%.4lf:", MATRIX_POS(m, colunas, i, j));
            }
        }
    }

}

void liberarMatriz(double *m){
    if(m != NULL)
        free(m);
}

char* getNextLine(FILE* file){	
	getline(&lineVet, &tamanho, file);
	return lineVet;
}
