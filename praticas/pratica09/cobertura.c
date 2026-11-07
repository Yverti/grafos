#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cobertura.h"

Grafo* criar_grafo(int n) {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->num_vertices = n;
    g->adj = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = (int*) calloc(n, sizeof(int));
    }
    return g;
}

void liberar_grafo(Grafo* g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

void adicionar_aresta(Grafo* g, int u, int v) {
    if (u >= 0 && u < g->num_vertices && v >= 0 && v < g->num_vertices) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
    }
}

// Verifica se o conjunto atual de vértices cobre todas as arestas do grafo
bool eh_cobertura(Grafo* g, const bool* solucao) {
    for (int u = 0; u < g->num_vertices; u++) {
        for (int v = u + 1; v < g->num_vertices; v++) {
            if (g->adj[u][v] == 1) {
                // Se existe uma aresta entre u e v, ao menos um deles deve estar no conjunto
                if (!solucao[u] && !solucao[v]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Função auxiliar recursiva para o Backtracking com Poda por Limite (Branch and Bound)
static void bt_helper(Grafo* g, int v, bool* sol_atual, int tamanho_atual, bool* melhor_sol, int* min_tam) {
    // Poda por limite: se o tamanho atual já for maior ou igual ao melhor encontrado, aborta este ramo
    if (tamanho_atual >= *min_tam) {
        return;
    }

    // Caso base: se avaliamos todos os vértices
    if (v == g->num_vertices) {
        if (eh_cobertura(g, sol_atual)) {
            *min_tam = tamanho_atual;
            for (int i = 0; i < g->num_vertices; i++) {
                melhor_sol[i] = sol_atual[i];
            }
        }
        return;
    }

    // Ramo 1: Inclui o vértice 'v' na cobertura
    sol_atual[v] = true;
    bt_helper(g, v + 1, sol_atual, tamanho_atual + 1, melhor_sol, min_tam);

    // Ramo 2: Não inclui o vértice 'v' na cobertura
    sol_atual[v] = false;
    bt_helper(g, v + 1, sol_atual, tamanho_atual, melhor_sol, min_tam);
}

// Força bruta com Backtracking
int cobertura_minima_bt(Grafo* g, bool* melhor_solucao) {
    bool* sol_atual = (bool*) calloc(g->num_vertices, sizeof(bool));
    int min_tam = g->num_vertices + 1;

    bt_helper(g, 0, sol_atual, 0, melhor_solucao, &min_tam);

    free(sol_atual);
    return min_tam;
}

// Heurística Gulosa: seleciona repetidamente o vértice de maior grau restante
int cobertura_gulosa(Grafo* g, bool* solucao_gulosa) {
    int n = g->num_vertices;
    for (int i = 0; i < n; i++) solucao_gulosa[i] = false;

    // Criar cópia da matriz de adjacência para manipular as arestas
    int** adj_copia = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        adj_copia[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            adj_copia[i][j] = g->adj[i][j];
        }
    }

    int tamanho_cobertura = 0;

    while (true) {
        // Encontra o vértice com maior grau restante
        int max_grau = 0;
        int melhor_v = -1;

        for (int i = 0; i < n; i++) {
            int grau = 0;
            for (int j = 0; j < n; j++) {
                if (adj_copia[i][j] == 1) grau++;
            }
            if (grau > max_grau) {
                max_grau = grau;
                melhor_v = i;
            }
        }

        // Se o maior grau for 0, todas as arestas já foram cobertas
        if (max_grau == 0 || melhor_v == -1) {
            break;
        }

        // Seleciona o vértice de maior grau
        solucao_gulosa[melhor_v] = true;
        tamanho_cobertura++;

        // Remove todas as arestas incidentes ao vértice selecionado
        for (int j = 0; j < n; j++) {
            adj_copia[melhor_v][j] = 0;
            adj_copia[j][melhor_v] = 0;
        }
    }

    // Libera a memória da matriz auxiliar
    for (int i = 0; i < n; i++) {
        free(adj_copia[i]);
    }
    free(adj_copia);

    return tamanho_cobertura;
}