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

    if(rank == 0){
        // recuperar nome dos arquivos
        char* matrizFileName = argv[MATRIZ];
        char* vetorFileName  = argv[VETOR];
        char* resultFileName = argv[RESULTADO];

        // carregar matriz e vetor na memoria
        FILE* file_matriz, *file_vetor;

        int line_matriz, column_matriz;
        file_matriz = fopen(matrizFileName, "rt");
        float* matriz = lerMatriz(file_matriz, &line_matriz, &column_matriz);

        line_column_matriz[0] = line_matriz;
        line_column_matriz[1] = column_matriz;

        int line_vetor, column_vetor;
        file_vetor = fopen(vetorFileName, "rt");
        float* vetor = lerMatriz(file_vetor, &line_vetor, &column_vetor);

        line_column_vetor[0] = line_vetor;
        line_column_vetor[1] = column_vetor;

        // envia o vetor para os processos
        for(int i = 1; i < num_tasks; i++){
            MPI_Send(line_column_vetor, 2, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(vetor, (line_vetor * column_vetor), MPI_FLOAT, i, 2, MPI_COMM_WORLD);
        }

        trabalho_local = (line_matriz / num_tasks);

        // envia as partições da matriz para os processos
        for(int i = 1; i < num_tasks; i++){
            MPI_Send(line_column_matriz, 2, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send((trabalho_local * column_matriz * i) + matriz, (trabalho_local * column_matriz), MPI_FLOAT, i, 2, MPI_COMM_WORLD);
        }

        // calcula o trabalho local
        float* vetor_local = initMatrix(1, trabalho_local);
        vetor_local = calculaMultiplicacao(matriz, trabalho_local, line_column_matriz[1], vetor, line_column_vetor[1]);

        float* matriz_result = initMatrix(line_column_matriz[0], line_column_vetor[1]);

        // insere o trabalho realizado localmente na matriz de resultados
        memcpy(matriz_result, vetor_local, sizeof(vetor_local[0]) * trabalho_local);

        // recebe os calculos dos demais processos
        for(int i = 1; i < num_tasks; i++)
            MPI_Recv((trabalho_local * line_column_vetor[1] * i) + matriz_result, trabalho_local * line_column_vetor[1], MPI_FLOAT, i, 3, MPI_COMM_WORLD, &status);

        // escreve a matriz no arquivo
        //imprimirMatriz(matriz_result, line_column_matriz[0], line_column_vetor[1]);
        escreverMatriz(resultFileName, matriz_result, line_column_matriz[0], line_column_vetor[1]);
        
        // libera as matrizes
        liberarMatriz(matriz);
        liberarMatriz(vetor);
        liberarMatriz(matriz_result);

    }else{
        // recebe o vetor
        MPI_Recv(line_column_vetor, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        float* vetor = initMatrix(line_column_vetor[0], line_column_vetor[1]);
        MPI_Recv(vetor, (line_column_vetor[0] * line_column_vetor[1]), MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);
        
        // recebe a matriz e calcula o trabalho local
        MPI_Recv(line_column_matriz, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        trabalho_local = (line_column_matriz[0] / num_tasks);
        line_column_matriz[0] = trabalho_local;
        float* matriz = initMatrix(line_column_matriz[0], line_column_matriz[1]);
        MPI_Recv(matriz, (line_column_matriz[0] * line_column_matriz[1]), MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);

        // calcula o resultado local e envia para o processo 0
        float* vetor_result = calculaMultiplicacao(matriz, line_column_matriz[0], line_column_matriz[1],vetor,  line_column_vetor[1]);
        MPI_Send(vetor_result, line_column_matriz[0] * line_column_vetor[1], MPI_FLOAT, 0, 3, MPI_COMM_WORLD);

    }

    MPI_Finalize();
}
