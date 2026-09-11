#include "grafo_matriz.h"
#include <stdlib.h>
#include <stdio.h>

GrafoMatriz* criar_grafo_matriz(int n) {
    GrafoMatriz *g = (GrafoMatriz*)malloc(sizeof(GrafoMatriz));
    g->n = n;
    g->adj = (int**)malloc(n * sizeof(int*));
    
    for (int i = 0; i < n; i++) {
        g->adj[i] = (int*)calloc(n, sizeof(int));
    }
    
    return g;
}

void inserir_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;  // Grafo não-direcionado
    }
}

void remover_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        g->adj[u][v] = 0;
        g->adj[v][u] = 0;
    }
}

int sao_adjacentes_matriz(GrafoMatriz *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        return g->adj[u][v];
    }
    return 0;
}

int grau_matriz(GrafoMatriz *g, int v) {
    if (v >= 0 && v < g->n) {
        int grau = 0;
        for (int i = 0; i < g->n; i++) {
            grau += g->adj[v][i];
        }
        return grau;
    }
    return -1;
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    if (g != NULL) {
        for (int i = 0; i < g->n; i++) {
            free(g->adj[i]);
        }
        free(g->adj);
        free(g);
    }
}

void imprimir_matriz(GrafoMatriz *g) {
    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            printf("%d ", g->adj[i][j]);
        }
        printf("\n");
    }
}
