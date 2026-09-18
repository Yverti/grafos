#include <stdio.h>
#include <stdlib.h>
#include "busca_profundidade.h"

// DFS Recursiva calculando os tempos
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo_desc, int *tempo_fin, int *tempo) {
    visitado[u] = 1; // 1 = Cinza (Descoberto)
    tempo_desc[u] = ++(*tempo);
    
    No *adj = g->adj[u];
    while (adj) {
        int v = adj->vertice;
        if (visitado[v] == 0) { // 0 = Branco (Não visitado)
            dfs_recursiva(g, v, visitado, tempo_desc, tempo_fin, tempo);
        }
        adj = adj->prox;
    }
    
    visitado[u] = 2; // 2 = Preto (Finalizado)
    tempo_fin[u] = ++(*tempo);
}

// Detecção de componentes conexos
int contar_componentes(GrafoLista *g) {
    int *visitado = calloc(g->num_vertices, sizeof(int));
    int *desc = calloc(g->num_vertices, sizeof(int));
    int *fin = calloc(g->num_vertices, sizeof(int));
    int tempo = 0;
    int componentes = 0;
    
    for (int i = 0; i < g->num_vertices; i++) {
        if (visitado[i] == 0) {
            componentes++;
            dfs_recursiva(g, i, visitado, desc, fin, &tempo);
        }
    }
    
    free(visitado);
    free(desc);
    free(fin);
    return componentes;
}

// Auxiliar recursiva para ciclos (rastreando o "pai")
static bool dfs_ciclo(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1; // Visitando
    
    No *adj = g->adj[u];
    while (adj) {
        int v = adj->vertice;
        if (visitado[v] == 0) {
            if (dfs_ciclo(g, v, u, visitado)) return true;
        } else if (v != pai && visitado[v] == 1) {
            return true; // Encontrou vértice cinza que não é o pai -> Areta de retorno (Ciclo)
        }
        adj = adj->prox;
    }
    
    visitado[u] = 2;
    return false;
}

// Verifica ciclos no grafo todo
bool tem_ciclo(GrafoLista *g) {
    int *visitado = calloc(g->num_vertices, sizeof(int));
    
    for (int i = 0; i < g->num_vertices; i++) {
        if (visitado[i] == 0) {
            if (dfs_ciclo(g, i, -1, visitado)) {
                free(visitado);
                return true;
            }
        }
    }
    
    free(visitado);
    return false;
}