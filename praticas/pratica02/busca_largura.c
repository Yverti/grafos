#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"

// Inicialização do Grafo
GrafoLista* criar_grafo(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->num_vertices = n;
    g->adj = malloc(n * sizeof(No*));
    for (int i = 0; i < n; i++) g->adj[i] = NULL;
    return g;
}

// Adiciona aresta não direcionada
void adicionar_aresta(GrafoLista *g, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->vertice = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
    
    novo = malloc(sizeof(No));
    novo->vertice = u;
    novo->prox = g->adj[v];
    g->adj[v] = novo;
}

void liberar_grafo(GrafoLista *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->adj[i];
        while (atual) {
            No *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(g->adj);
    free(g);
}

// Implementação da Fila
Fila* criar_fila(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;
    f->dados = malloc(capacidade * sizeof(int));
    return f;
}

bool fila_vazia(Fila *f) { return f->tamanho == 0; }

void enfileirar(Fila *f, int valor) {
    if (f->tamanho == f->capacidade) return;
    f->fim = (f->fim + 1) % f->capacidade;
    f->dados[f->fim] = valor;
    f->tamanho++;
}

int desenfileirar(Fila *f) {
    if (fila_vazia(f)) return -1;
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

// Implementação da BFS
void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }
    
    Fila *f = criar_fila(g->num_vertices);
    dist[origem] = 0;
    enfileirar(f, origem);
    
    while (!fila_vazia(f)) {
        int u = desenfileirar(f);
        No *adj = g->adj[u];
        
        while (adj) {
            int v = adj->vertice;
            if (dist[v] == -1) { // Vértice não visitado
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(f, v);
            }
            adj = adj->prox;
        }
    }
    liberar_fila(f);
}

// 2-Coloração via BFS
bool eh_bipartido(GrafoLista *g) {
    int *cor = malloc(g->num_vertices * sizeof(int));
    for (int i = 0; i < g->num_vertices; i++) cor[i] = -1;
    
    for (int i = 0; i < g->num_vertices; i++) {
        if (cor[i] == -1) {
            Fila *f = criar_fila(g->num_vertices);
            cor[i] = 0;
            enfileirar(f, i);
            
            while (!fila_vazia(f)) {
                int u = desenfileirar(f);
                No *adj = g->adj[u];
                
                while (adj) {
                    int v = adj->vertice;
                    if (cor[v] == -1) {
                        cor[v] = 1 - cor[u];
                        enfileirar(f, v);
                    } else if (cor[v] == cor[u]) {
                        liberar_fila(f);
                        free(cor);
                        return false; // Mesma cor em vértices adjacentes = ciclo ímpar
                    }
                    adj = adj->prox;
                }
            }
            liberar_fila(f);
        }
    }
    free(cor);
    return true;
}