#include "grafo_lista.h"
#include <stdlib.h>
#include <stdio.h>

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *g = (GrafoLista*)malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**)calloc(n, sizeof(No*));
    return g;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        // Insere na lista de u
        No *novo = (No*)malloc(sizeof(No));
        novo->destino = v;
        novo->prox = g->adj[u];
        g->adj[u] = novo;
        
        // Insere na lista de v (grafo não-direcionado)
        novo = (No*)malloc(sizeof(No));
        novo->destino = u;
        novo->prox = g->adj[v];
        g->adj[v] = novo;
    }
}

void remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        // Remove de u
        No *atual = g->adj[u];
        if (atual != NULL && atual->destino == v) {
            g->adj[u] = atual->prox;
            free(atual);
        } else {
            while (atual != NULL && atual->prox != NULL) {
                if (atual->prox->destino == v) {
                    No *temp = atual->prox;
                    atual->prox = temp->prox;
                    free(temp);
                    break;
                }
                atual = atual->prox;
            }
        }
        
        // Remove de v
        atual = g->adj[v];
        if (atual != NULL && atual->destino == u) {
            g->adj[v] = atual->prox;
            free(atual);
        } else {
            while (atual != NULL && atual->prox != NULL) {
                if (atual->prox->destino == u) {
                    No *temp = atual->prox;
                    atual->prox = temp->prox;
                    free(temp);
                    break;
                }
                atual = atual->prox;
            }
        }
    }
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        No *atual = g->adj[u];
        while (atual != NULL) {
            if (atual->destino == v) {
                return 1;
            }
            atual = atual->prox;
        }
    }
    return 0;
}

int grau_lista(GrafoLista *g, int v) {
    if (v >= 0 && v < g->n) {
        int grau = 0;
        No *atual = g->adj[v];
        while (atual != NULL) {
            grau++;
            atual = atual->prox;
        }
        return grau;
    }
    return -1;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g != NULL) {
        for (int i = 0; i < g->n; i++) {
            No *atual = g->adj[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }
        free(g->adj);
        free(g);
    }
}

void imprimir_lista(GrafoLista *g) {
    printf("Lista de Adjacência:\n");
    for (int i = 0; i < g->n; i++) {
        printf("Vértice %d: ", i);
        No *atual = g->adj[i];
        while (atual != NULL) {
            printf("%d ", atual->destino);
            atual = atual->prox;
        }
        printf("\n");
    }
}
