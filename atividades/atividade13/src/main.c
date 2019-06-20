#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include <omp.h>

#define INFINITO 1000000000

void bellman_ford(double **G , int V, int E, int **edge){

    int i, u, v, y, predecessor[V], src = 0, aux = 1;
    double distancia[V];

    // inicializa as distancias e os predecessores
    for(i=0;i<V;i++){
        distancia[i] = INFINITO; 
        predecessor[i] = -1;
    }

    // inicia a distancia da raiz
    distancia[src] = 0;
    

    // utilizando as tecnicas de relaxamento, relaxa todas as arestas utilizando paralelismo
    #pragma omp parallel for shared(distancia, predecessor, edge, V, E) private(i, y, u, v) schedule(static)
    for(i = 0; i < (V-1); i++){
        for(y = 0; y < E; y++){
            u = edge[y][0];
            v = edge[y][1];

            if(distancia[u] + G[u][v] < distancia[v]){
                distancia[v] = distancia[u] + G[u][v];
                predecessor[v] = u;
            }
        }
    }

    // verifica ciclos negaticos com paralelismo
    #pragma omp parallel for shared(distancia, edge, V, E, aux) private(i, y, u, v) schedule(static)
    for(y = 0; y < E; y++){
            u = edge[y][0];
            v = edge[y][1];

            if(distancia[u] + G[u][v] < distancia[v]){
                printf("Existe ciclo negativo.");
                aux = 0;             
            }
    }

    // se não existir ciclos negavitos mostra as distancias e predecessores
    if(aux)
        for(i = 0; i < V; i++)
            printf("distancia[%d]: %.2lf, predecessor[%d] = %d\n",i , distancia[i], i, predecessor[i]);
}

int main(int argc, char* argv[]){

    int V = 0, i, j, y = 0;
    
    double aux;
    double **g;

    // ler matriz de adjacencia
    FILE* matrix;
    int linhasMatrix, colunasMatrix;
    matrix = fopen(argv[MATRIZ], "rt");
    g = lerMatriz(matrix, &linhasMatrix, &colunasMatrix);

    // cria e aloca espaço para a matriz de arestas
    int **edge = malloc(linhasMatrix * linhasMatrix * sizeof(int*));
    for(i = 0; i < (linhasMatrix * linhasMatrix); i++)
        edge[i] = malloc(2*sizeof(int));
    
    // preenche a matriz de arestas
    for(i = 0; i < linhasMatrix; i++){
        for(j = 0; j < linhasMatrix; j++){
            if(g[i][j] != 0){
                edge[y][0] = i;
                edge[y][1] = j;
                y++;
            }
        }
    }

    // chama algoritmo bellman_ford
    bellman_ford(g,linhasMatrix,y,edge);
    
    // libera espaço das matrizes
    for(i = 0; i < (linhasMatrix * linhasMatrix); i++)
        free(edge[i]);

    free(edge);

    liberarMatriz(g);

    return 0;
}
