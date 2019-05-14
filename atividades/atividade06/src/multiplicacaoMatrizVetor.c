#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "arquivo.h"

float* calculaMultiplicacao(float* matriz, int line_matriz, int column_matriz, float* vetor, int column_vetor){
    // calcula a multiplicação das matrizes
    float* vetRes = initMatrix(line_matriz, column_vetor);

    int i, j, z;

    for(i = 0; i < line_matriz; i++)
		for(j = 0; j < column_vetor;j++)
            for(z = 0; z < column_matriz; z++)
    		    MATRIX_POS(vetRes, column_vetor, i, j) +=  MATRIX_POS(matriz, column_matriz, i, z) * MATRIX_POS(vetor, column_vetor, z, j);

    return vetRes;
}


int main(int argc, char* argv[]){

    int num_tasks, rank, qtdIterations;
    //float acertos = 0;
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    int line_column_vetor[2] = {0,0};
    int line_column_matriz[2] = {0,0};

    int trabalho_local = 0;
    float* matriz = NULL, *vetor = NULL, *matriz_aux = NULL, *matriz_result = NULL, *matriz_local = NULL;
    char* resultFileName;

    if(rank == 0){
        // recuperar nome dos arquivos
        char* matrizFileName = argv[MATRIZ];
        char* vetorFileName  = argv[VETOR];
        resultFileName = argv[RESULTADO];

        // carregar matriz e vetor na memoria
        FILE* file_matriz, *file_vetor;

        int line_matriz, column_matriz;
        file_matriz = fopen(matrizFileName, "rt");
        matriz = lerMatriz(file_matriz, &line_matriz, &column_matriz);

        line_column_matriz[0] = line_matriz;
        line_column_matriz[1] = column_matriz;

        int line_vetor, column_vetor;
        file_vetor = fopen(vetorFileName, "rt");
        vetor = lerMatriz(file_vetor, &line_vetor, &column_vetor);

        line_column_vetor[0] = line_vetor;
        line_column_vetor[1] = column_vetor;
    }

    // envia as informações do vetor para todos os processos
    MPI_Bcast(line_column_vetor, 2, MPI_INT, 0, MPI_COMM_WORLD);

    // inicializa o vetor se não for o processo zero (pra não acessar sem tá mapeado)
    if(rank != 0)
        vetor = initMatrix(line_column_vetor[0], line_column_vetor[1]);

    // envia para todos os processos o vetor e as informações da matriz
    MPI_Bcast(vetor, (line_column_vetor[0] * line_column_vetor[1]), MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(line_column_matriz, 2, MPI_INT, 0, MPI_COMM_WORLD);

    // calcula o trabalho local
    trabalho_local = (line_column_matriz[0] / num_tasks);

    // inicializa uma matriz auxiliar para receber as partições do processo 0
    matriz_aux = malloc(line_column_matriz[1] * trabalho_local * sizeof(float));
    // cada processo recebe a partição da matriz a ser calculada (inclisive o processo 0)
    MPI_Scatter(matriz, trabalho_local * line_column_matriz[1], MPI_FLOAT, matriz_aux, line_column_matriz[1] * trabalho_local, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // cada processo calcula a sua multiplicação
    matriz_local = calculaMultiplicacao(matriz_aux, trabalho_local, line_column_matriz[1], vetor, line_column_vetor[1]);

    // processo 0 recebe as contribuições de cada processo
    matriz_result = initMatrix(line_column_matriz[0], line_column_vetor[1]);
    MPI_Gather(matriz_local, trabalho_local * line_column_vetor[1], MPI_FLOAT, matriz_result, trabalho_local * line_column_vetor[1], MPI_FLOAT, 0, MPI_COMM_WORLD);

    // libera as matrizes auxiliares
    liberarMatriz(matriz_local);
    liberarMatriz(matriz_aux);

    // apenas o processo 0
    if(rank == 0){
        // escreve o resultado final no arquivo
        //imprimirMatriz(matriz_result, line_column_matriz[0], line_column_vetor[1]);
        escreverMatriz(resultFileName, matriz_result, line_column_matriz[0], line_column_vetor[1]);
        
        // libera as matrizes
        liberarMatriz(matriz);
        liberarMatriz(vetor);
        liberarMatriz(matriz_result);
    }

    MPI_Finalize();
}
