#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

GrafoLista* criar_grafo(int num_vertices) {
    GrafoLista* g = (GrafoLista*)malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->adjacencias = (No**)malloc(num_vertices * sizeof(No*));
    for (int i = 0; i < num_vertices; i++) {
        g->adjacencias[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoLista* g, int origem, int destino) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = destino;
    novo_no->prox = g->adjacencias[origem];
    g->adjacencias[origem] = novo_no;
}

void liberar_grafo(GrafoLista* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No* atual = g->adjacencias[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adjacencias);
    free(g);
}

// Algoritmo de Kahn (BFS com grau de entrada)
int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int* grau_entrada = (int*)calloc(n, sizeof(int));
    
    // Calcula o grau de entrada de todos os vértices
    for (int i = 0; i < n; i++) {
        No* temp = g->adjacencias[i];
        while (temp) {
            grau_entrada[temp->vertice]++;
            temp = temp->prox;
        }
    }

    int* fila = (int*)malloc(n * sizeof(int));
    int inicio = 0, fim = 0;

    // Enfileira vértices com grau de entrada 0
    for (int i = 0; i < n; i++) {
        if (grau_entrada[i] == 0) {
            fila[fim++] = i;
        }
    }

    int* ordem = (int*)malloc(n * sizeof(int));
    int count = 0;

    // Processa a fila
    while (inicio < fim) {
        int u = fila[inicio++];
        ordem[count++] = u;

        No* temp = g->adjacencias[u];
        while (temp) {
            int v = temp->vertice;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
            temp = temp->prox;
        }
    }

    free(grau_entrada);
    free(fila);

    // Verifica se houve ciclo
    if (count != n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = count;
    return ordem;
}

// Utilitário DFS recursivo
int dfs_visit(GrafoLista* g, int u, int* cor, int* pilha, int* topo) {
    cor[u] = 1; // 1 = Cinza (sendo visitado)
    No* temp = g->adjacencias[u];
    
    while (temp) {
        int v = temp->vertice;
        if (cor[v] == 1) return 0; // Achou aresta de retorno (Ciclo)
        if (cor[v] == 0) {
            if (!dfs_visit(g, v, cor, pilha, topo)) return 0;
        }
        temp = temp->prox;
    }
    
    cor[u] = 2; // 2 = Preto (totalmente visitado)
    pilha[(*topo)++] = u; // Adiciona na pilha/saída
    return 1;
}

// Variante DFS para ordenação topológica
int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int* cor = (int*)calloc(n, sizeof(int)); // 0 = Branco
    int* pilha = (int*)malloc(n * sizeof(int));
    int topo = 0;

    for (int i = 0; i < n; i++) {
        if (cor[i] == 0) {
            if (!dfs_visit(g, i, cor, pilha, &topo)) {
                free(cor);
                free(pilha);
                *tamanho = 0;
                return NULL; // Grafo possui ciclo
            }
        }
    }

    free(cor);

    // Inverte a pilha para obter a ordem topológica
    int* ordem = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        ordem[i] = pilha[n - 1 - i];
    }
    free(pilha);

    *tamanho = n;
    return ordem;
}

// Verifica se é DAG tentado gerar ordenação via Kahn
int eh_dag(GrafoLista *g) {
    int tamanho;
    int* ordem = ordenacao_topologica_kahn(g, &tamanho);
    if (ordem != NULL) {
        free(ordem);
        return 1; // Acíclico
    }
    return 0; // Possui ciclo
}