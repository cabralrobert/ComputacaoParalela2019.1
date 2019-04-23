/************************************************
 * File Name : pi.c
 * Creation Date : 22-04-2019
 * Last Modified : seg 22 abr 2019 23:29:38 -03
 * Created By : robertcabral@alu.ufc.br
 * Institution : Universidade Federal do Ceará
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

float monteCarloPi(int n){

        int i;
        float acertos = 0, x, y;
        for(i = 0; i < n; i++){
                x = ((float)rand()/(float)(RAND_MAX));
                y = ((float)rand()/(float)(RAND_MAX));

                if((x * x) + (y * y) < 1)
                        acertos += 1;
        }
        return (4 * acertos / n);
}

int main(int argc, char* argv[]){

    int num_tasks, rank, qtdIterations;
    float pi = 0;
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(time(NULL) + (rank * rank) % RAND_MAX);

    if(num_tasks != 2 && num_tasks != 4 && num_tasks != 8)
        goto fim;

    qtdIterations = (atoi(argv[1]) / num_tasks);

    if(rank == 0){
        pi = monteCarloPi(qtdIterations);
        float valueRes = 0;
        int i;

        for(i = 1; i < num_tasks; i++){
            MPI_Recv(&valueRes, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, NULL);
            pi += valueRes;
        }

        printf("%f\n", pi/num_tasks);
    } else {
        pi = monteCarloPi(qtdIterations);
        MPI_Send(&pi, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }


fim:
    MPI_Finalize();
    return 0;

}

