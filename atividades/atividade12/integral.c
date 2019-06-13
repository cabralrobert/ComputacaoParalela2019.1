#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include <omp.h>

double f(double x) {
   double return_val = 0.0;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

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
    double regiao;
    double integral_local = 0.0;

    // guarda os valores de a, b e n
    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);

    // calcula o tamanho do trapezio a deslocar pela função
    h = (b - a) / n;
    
    // cada processo calcula seu proprio x, ou seja, o local no grafico que começa a calcular a integral
    x = (((b - a) / num_tasks) * rank) + a;
    
    // cada processo calcula sua parte da integral
    double inicial = x, final = inicial + ((b - a) / num_tasks);
    
    // região paralela OpenMP com redução na variavel integral_local
    #pragma omp parallel shared(inicial,final,n,h) private(x,i,regiao) reduction(+:integral_local)
    {
        // cada thread calcula sua contribuição que irá fazer
        regiao = (((final - inicial) / omp_get_num_threads()) * omp_get_thread_num()) + inicial;
        
        // cada thread calcula a partição da integral
        for(i = 0; i < ((n/num_tasks) / omp_get_num_threads()); i++){
            regiao += h;
            integral_local += f(regiao);
        }

    }

    // redução com a soma das contribuições
    MPI_Reduce(&integral_local, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // processo 0 soma o trapezio maior a contribuição de cada processo e finaliza printando na tela
    if(rank == 0){
        integral += (f(a) + f(b)) / 2.0;
        integral *= h;
        printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);
    }

    MPI_Finalize();
   return 0;
}
