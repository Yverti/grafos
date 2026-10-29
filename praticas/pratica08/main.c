#include <stdio.h>
#include "agm.h"

int main() {
    int V = 5; // Número de vértices
    int E = 7; // Número de arestas

    // Representação para Kruskal (Lista de arestas)
    Aresta arestas[] = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9}
    };

    printf("==================================\n");
    printf("        TESTANDO KRUSKAL          \n");
    printf("==================================\n");
    kruskal(arestas, E, V);
    printf("\n");

    // Representação para Prim (Lista de adjacência)
    Grafo* g = criar_grafo(V);
    adicionar_aresta(g, 0, 1, 2);
    adicionar_aresta(g, 0, 3, 6);
    adicionar_aresta(g, 1, 2, 3);
    adicionar_aresta(g, 1, 3, 8);
    adicionar_aresta(g, 1, 4, 5);
    adicionar_aresta(g, 2, 4, 7);
    adicionar_aresta(g, 3, 4, 9);

    printf("==================================\n");
    printf("          TESTANDO PRIM           \n");
    printf("==================================\n");
    // Iniciando o algoritmo de Prim pelo vértice 0
    prim(g, 0);
    printf("\n");

    liberar_grafo(g);

    return 0;
}