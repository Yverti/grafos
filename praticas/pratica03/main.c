#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

void imprimir_array(int* arr, int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n = 6;
    GrafoLista* g = criar_grafo(n);
    
    // Grafo exemplo (DAG):
    // 5 -> 2, 5 -> 0
    // 4 -> 0, 4 -> 1
    // 2 -> 3, 3 -> 1
    adicionar_aresta(g, 5, 2);
    adicionar_aresta(g, 5, 0);
    adicionar_aresta(g, 4, 0);
    adicionar_aresta(g, 4, 1);
    adicionar_aresta(g, 2, 3);
    adicionar_aresta(g, 3, 1);

    printf("--- Teste de DAG e Ordenacao Topologica ---\n\n");
    
    if (eh_dag(g)) {
        printf("eh_dag: O grafo e um DAG! (Aciclico)\n");
    } else {
        printf("eh_dag: O grafo possui um ciclo.\n");
    }

    int tamanho;
    
    int* kahn = ordenacao_topologica_kahn(g, &tamanho);
    if (kahn) {
        printf("Ordenacao Topologica (Kahn): ");
        imprimir_array(kahn, tamanho);
        free(kahn);
    }

    int* dfs = ordenacao_topologica_dfs(g, &tamanho);
    if (dfs) {
        printf("Ordenacao Topologica (DFS):  ");
        imprimir_array(dfs, tamanho);
        free(dfs);
    }

    liberar_grafo(g);
    return 0;
}