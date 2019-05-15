#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double f(double x) {
   double return_val = 0.0;
   //return_val = x;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

#define PROCESSORS 16

int main(int argc, char *argv[]) {
   
    int num_tasks, rank, qtdIterations;
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    // Valor da integral
   double integral = 0; 
   // Limites do intervalo
   double a, b;
   // Número de trapézios
   int n;
   // Base do trapézio
   double h;
   double x;
   int i;

   int contribuicoes[num_tasks];
   double x_local_vet[num_tasks];

   a = atof(argv[1]);
   b = atof(argv[2]);
   n = atoi(argv[3]);

   h = (b - a) / n;

    if(rank == 0){
       
        x = a;

        int new_n = n / num_tasks;

        for(i = 0; i < num_tasks; i++){
            contribuicoes[i] = new_n;
            new_n += (n / num_tasks);
        }

        x_local_vet[0] = a;
        for(i = 1; i < num_tasks; i++){
            x_local_vet[i] = ((b - a) / num_tasks) * i;
        }

    }

    int n_local = 0;
    double x_local = 0.0;
    double integral_local = 0;

    MPI_Scatter(contribuicoes, 1, MPI_INT, &n_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(x_local_vet, 1, MPI_DOUBLE, &x_local, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    for(i = (n_local - (n / num_tasks)); i < n_local; i++){
        x_local += h;
        integral_local += f(x_local);
    }

    MPI_Reduce(&integral_local, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0){
        integral += (f(a) + f(b)) / 2.0;
        integral = integral / num_tasks;
        integral *= h;
        printf("%lf\n", integral);
    }

    MPI_Finalize();
   return 0;
}
