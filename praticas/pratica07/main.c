#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

int main() {
    printf("=== Teste 1: Grafo SEM ciclo negativo ===\n");
    GrafoListaPonderado* g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1, -1);
    adicionar_aresta(g1, 0, 2, 4);
    adicionar_aresta(g1, 1, 2, 3);
    adicionar_aresta(g1, 1, 3, 2);
    adicionar_aresta(g1, 1, 4, 2);
    adicionar_aresta(g1, 3, 2, 5);
    adicionar_aresta(g1, 3, 1, 1);
    adicionar_aresta(g1, 4, 3, -3);

    int dist1[5], pred1[5];
    if (bellman_ford(g1, 0, dist1, pred1) == 1) {
        printf("Menores distancias a partir do vertice 0:\n");
        for (int i = 0; i < 5; i++) {
            printf("Vertice %d: dist = %d, pred = %d\n", i, dist1[i], pred1[i]);
        }
    }
    liberar_grafo(g1);
    printf("\n");

    printf("=== Teste 2: Grafo COM ciclo negativo ===\n");
    GrafoListaPonderado* g2 = criar_grafo(4);
    adicionar_aresta(g2, 0, 1, 1);
    adicionar_aresta(g2, 1, 2, -1);
    adicionar_aresta(g2, 2, 3, -1);
    adicionar_aresta(g2, 3, 1, -1); // Forma um ciclo negativo: 1 -> 2 -> 3 -> 1 (Peso total: -3)

    int dist2[4], pred2[4];
    if (bellman_ford(g2, 0, dist2, pred2) == 1) {
         printf("Nenhum ciclo negativo detectado.\n");
    }
    liberar_grafo(g2);

    return 0;
}