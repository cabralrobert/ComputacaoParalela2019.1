/************************************************
 * File Name : topologias.c
 * Creation Date : 15-05-2019
 * Last Modified : ter 21 mai 2019 17:18:01 -03
 * Created By : robertcabral@alu.ufc.br
 * Institution : Universidade Federal do Ceará
 ************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int rank, left, right, p;
MPI_Status status;

int printArrayForRank(int rank, int *array,  int arraySize) {
   int i;
   char *output = (char *) malloc((arraySize + 100) * 4 * sizeof(char));
   sprintf(output, "Rank %d: ", rank);
   for (int i = 0; i < arraySize; i++)
      sprintf(output, "%s %d", output, array[i]);
   sprintf(output,"%s\n", output);
   printf("%s", output);
   fflush(stdout);
   free(output);
   return i;
}

int MPI_Ring_broadcast(int *array, int arraySize, MPI_Comm comm) {
    if(rank == 0){
        MPI_Send(array, arraySize, MPI_INT, right, 0, comm);
        MPI_Send(array, arraySize, MPI_INT, left, 0, comm);
    }else if(rank >= (p/2)){
        MPI_Recv(array, arraySize, MPI_INT, right, 0, comm, &status);
        MPI_Send(array, arraySize, MPI_INT, left, 0, comm);
    }else{
        MPI_Recv(array, arraySize, MPI_INT, left, 0, comm, &status);
        MPI_Send(array, arraySize, MPI_INT, right, 0, comm);
    }
}

int MPI_Mesh_broadcast(int *array, int arraySize, MPI_Comm comm) {
    // cria o mesh
    int dims[2] = {4,4};
    int periods[2] = {0,0};
    MPI_Comm comm_aux;
    MPI_Cart_create(comm, 2, dims, periods, 0, &comm_aux);

    // recupera os vizinhos
    int *vizinhos = malloc(4 * sizeof(int));
    MPI_Cart_shift(comm_aux, 0, 1, &vizinhos[0], &vizinhos[1]);
    MPI_Cart_shift(comm_aux, 1, 1, &vizinhos[2], &vizinhos[3]);

    // recupera a coordenada no mesh
    int coord[2];
    MPI_Cart_coords(comm_aux, rank, 2, coord);

    if(coord[1] == 0){
        // Se rank não for zero recebe do vizinho de cima
        if(rank != 0)
            MPI_Recv(array, arraySize, MPI_INT, vizinhos[0], 0, MPI_COMM_WORLD, &status);
        
        // envia para os vizinhos da direita e de baixo
        MPI_Send(array, arraySize, MPI_INT, vizinhos[3], 0, MPI_COMM_WORLD);
        MPI_Send(array, arraySize, MPI_INT, vizinhos[1], 0, MPI_COMM_WORLD);
    }else{
        // se não está na primeira coluna, recebe da direita e envia para a direita
        MPI_Recv(array, arraySize, MPI_INT, vizinhos[2], 0, MPI_COMM_WORLD, &status);
        MPI_Send(array, arraySize, MPI_INT, vizinhos[3], 0, MPI_COMM_WORLD);
    }
}

int main (int argc, char *argv[]) {
   int size = 8, tag = 0;
   char send_buffer1[8], recv_buffer1[8];
   char send_buffer2[8], recv_buffer2[8];
   int *array, arraySize;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   left = ((rank-1 + p) % p);
   right = (rank + 1) % p;

   arraySize = atoi(argv[1]);
   array = (int *) malloc(arraySize * sizeof(int));
 
   // Inicializa o array com 0.
   if (rank == 0) {
     for (int i = 0; i < arraySize; i++)
         array[i] = 0;
   }

   // Imprime o array inicial de todos os processos.
   printArrayForRank(rank, array, arraySize);

   // Faz broadcast usando uma topologia anel.
   MPI_Ring_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia anel.
   printArrayForRank(rank, array, arraySize);
  
   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);
   
   // Reconfigura o array com todos os elementos iguais a 1000.
   if (rank == 0) {
      for (int i = 0; i < arraySize; i++)
         array[i] =  1000;
   }

   // Faz broadcast usando uma topologia mesh.
   MPI_Mesh_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia mesh.
   printArrayForRank(rank, array, arraySize);
     
   free(array);
   MPI_Finalize();
   return 0;
}
