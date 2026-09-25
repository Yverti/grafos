#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "coloracao.h"

// --- Funções Auxiliares do Grafo ---
GrafoLista* criar_grafo(int num_vertices) {
    GrafoLista* g = (GrafoLista*)malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->adj = (No**)malloc(num_vertices * sizeof(No*));
    for (int i = 0; i < num_vertices; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoLista* g, int u, int v) {
    No* novo_u = (No*)malloc(sizeof(No));
    novo_u->vizinho = v;
    novo_u->prox = g->adj[u];
    g->adj[u] = novo_u;

    No* novo_v = (No*)malloc(sizeof(No));
    novo_v->vizinho = u;
    novo_v->prox = g->adj[v];
    g->adj[v] = novo_v;

    g->num_arestas++;
}

void liberar_grafo(GrafoLista* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No* atual = g->adj[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}

// --- Coloração Gulosa (Ordem Arbitrária) ---
void coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    int *cor = (int*)malloc(n * sizeof(int));
    bool *disponivel = (bool*)malloc(n * sizeof(bool));
    
    for (int i = 0; i < n; i++) cor[i] = -1;

    // Atribui a primeira cor ao primeiro vértice
    cor[0] = 0;
    *num_cores = 1;

    // Colore o restante dos vértices
    for (int u = 1; u < n; u++) {
        for (int i = 0; i < n; i++) disponivel[i] = true;

        // Marca cores dos vizinhos como indisponíveis
        No* atual = g->adj[u];
        while (atual != NULL) {
            if (cor[atual->vizinho] != -1) {
                disponivel[cor[atual->vizinho]] = false;
            }
            atual = atual->prox;
        }

        // Encontra a primeira cor disponível
        int cr;
        for (cr = 0; cr < n; cr++) {
            if (disponivel[cr]) break;
        }

        cor[u] = cr;
        if (cr + 1 > *num_cores) {
            *num_cores = cr + 1;
        }
    }

    printf("Coloracao Gulosa (cores por vertice):\n");
    for (int u = 0; u < n; u++) {
        printf("Vertice %d ---> Cor %d\n", u, cor[u]);
    }
    printf("Total de cores utilizadas: %d\n", *num_cores);

    free(cor);
    free(disponivel);
}

// --- Estrutura e Comparador para Welsh-Powell ---
typedef struct {
    int vertice;
    int grau;
} VerticeGrau;

int comparar_grau_decrescente(const void* a, const void* b) {
    VerticeGrau* v1 = (VerticeGrau*)a;
    VerticeGrau* v2 = (VerticeGrau*)b;
    return v2->grau - v1->grau; // Decrescente
}

// --- Coloração Welsh-Powell ---
void coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    VerticeGrau* vg = (VerticeGrau*)malloc(n * sizeof(VerticeGrau));
    
    // Calcula o grau de cada vértice
    for (int i = 0; i < n; i++) {
        vg[i].vertice = i;
        vg[i].grau = 0;
        No* atual = g->adj[i];
        while (atual != NULL) {
            vg[i].grau++;
            atual = atual->prox;
        }
    }

    // Ordena os vértices por grau (decrescente)
    qsort(vg, n, sizeof(VerticeGrau), comparar_grau_decrescente);

    int *cor = (int*)malloc(n * sizeof(int));
    bool *disponivel = (bool*)malloc(n * sizeof(bool));
    
    for (int i = 0; i < n; i++) cor[i] = -1;
    *num_cores = 0;

    // Colore os vértices na ordem ordenada
    for (int i = 0; i < n; i++) {
        int u = vg[i].vertice;
        for (int c = 0; c < n; c++) disponivel[c] = true;

        No* atual = g->adj[u];
        while (atual != NULL) {
            if (cor[atual->vizinho] != -1) {
                disponivel[cor[atual->vizinho]] = false;
            }
            atual = atual->prox;
        }

        int cr;
        for (cr = 0; cr < n; cr++) {
            if (disponivel[cr]) break;
        }

        cor[u] = cr;
        if (cr + 1 > *num_cores) {
            *num_cores = cr + 1;
        }
    }

    printf("Coloracao Welsh-Powell (cores por vertice):\n");
    for (int i = 0; i < n; i++) {
        int u = vg[i].vertice;
        printf("Vertice %d (Grau %d) ---> Cor %d\n", u, vg[i].grau, cor[u]);
    }
    printf("Total de cores utilizadas: %d\n", *num_cores);

    free(cor);
    free(disponivel);
    free(vg);
}

// --- Verificação de Grafo Bipartido (BFS) ---
bool eh_bipartido(GrafoLista *g) {
    int n = g->num_vertices;
    int *cor = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) cor[i] = -1;

    // Fila simples para a BFS
    int *fila = (int*)malloc(n * sizeof(int));
    int inicio = 0, fim = 0;

    for (int i = 0; i < n; i++) {
        if (cor[i] == -1) {
            // Inicializa a componente conexa atual
            cor[i] = 1;
            fila[fim++] = i;

            while (inicio < fim) {
                int u = fila[inicio++];
                No* atual = g->adj[u];

                while (atual != NULL) {
                    int v = atual->vizinho;
                    
                    if (cor[v] == -1) {
                        cor[v] = 1 - cor[u];
                        fila[fim++] = v;
                    } 
                    else if (cor[v] == cor[u]) {
                        free(cor);
                        free(fila);
                        return false;
                    }
                    atual = atual->prox;
                }
            }
        }
    }

    free(cor);
    free(fila);
    return true;
}