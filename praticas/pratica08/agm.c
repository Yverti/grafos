#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "agm.h"

/* ============================================================
 * ESTRUTURA DISJOINT SET (DSU / UNION-FIND)
 * ============================================================ */

DSU* criar_dsu(int n) {
    DSU *d = (DSU*) malloc(sizeof(DSU));
    d->n = n;
    d->pai = (int*) malloc(n * sizeof(int));
    d->rank = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        d->pai[i] = i;
        d->rank[i] = 0;
    }
    return d;
}

void liberar_dsu(DSU *d) {
    free(d->pai);
    free(d->rank);
    free(d);
}

// Encontra a raiz com Compressão de Caminho
int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

// Une dois conjuntos por Rank
void unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x != raiz_y) {
        if (d->rank[raiz_x] < d->rank[raiz_y]) {
            d->pai[raiz_x] = raiz_y;
        } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
            d->pai[raiz_y] = raiz_x;
        } else {
            d->pai[raiz_y] = raiz_x;
            d->rank[raiz_x]++;
        }
    }
}

/* ============================================================
 * ALGORITMO DE KRUSKAL - Complexidade O(E log E)
 * ============================================================ */

int comparar_arestas(const void *a, const void *b) {
    Aresta *arestaA = (Aresta *)a;
    Aresta *arestaB = (Aresta *)b;
    return arestaA->peso - arestaB->peso;
}

void kruskal(Aresta *arestas, int E, int V) {
    // Ordenação O(E log E)
    qsort(arestas, E, sizeof(Aresta), comparar_arestas);

    DSU *d = criar_dsu(V);
    int custo_total = 0;
    
    printf("Arestas da AGM (Kruskal):\n");
    for (int i = 0; i < E; i++) {
        int u = arestas[i].u;
        int v = arestas[i].v;
        int peso = arestas[i].peso;

        // Se u e v não estão no mesmo conjunto, adiciona a aresta (não forma ciclo)
        if (find(d, u) != find(d, v)) {
            unir(d, u, v);
            custo_total += peso;
            printf("(%d, %d) - Peso: %d\n", u, v, peso);
        }
    }
    printf("Custo total da AGM (Kruskal): %d\n", custo_total);
    liberar_dsu(d);
}

/* ============================================================
 * ALGORITMO DE PRIM - Complexidade O((V + E) log V)
 * ============================================================ */

// Estruturas auxiliares para o Min-Heap usado no Prim
typedef struct { int v, chave; } HeapNode;

typedef struct {
    HeapNode *array;
    int *pos; // Mapeia o vértice para seu índice atual no heap (para atualização em O(log V))
    int tamanho;
} MinHeap;

void trocar_node(HeapNode* a, HeapNode* b) {
    HeapNode t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap* h, int idx) {
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < h->tamanho && h->array[esq].chave < h->array[menor].chave) menor = esq;
    if (dir < h->tamanho && h->array[dir].chave < h->array[menor].chave) menor = dir;

    if (menor != idx) {
        h->pos[h->array[menor].v] = idx;
        h->pos[h->array[idx].v] = menor;
        trocar_node(&h->array[menor], &h->array[idx]);
        min_heapify(h, menor);
    }
}

HeapNode extrair_min(MinHeap* h) {
    HeapNode raiz = h->array[0];
    HeapNode ultimo = h->array[h->tamanho - 1];

    h->array[0] = ultimo;
    h->pos[raiz.v] = h->tamanho - 1;
    h->pos[ultimo.v] = 0;
    
    h->tamanho--;
    min_heapify(h, 0);

    return raiz;
}

void decrementar_chave(MinHeap* h, int v, int chave) {
    int i = h->pos[v];
    h->array[i].chave = chave;
    
    while (i && h->array[i].chave < h->array[(i - 1) / 2].chave) {
        h->pos[h->array[i].v] = (i - 1) / 2;
        h->pos[h->array[(i - 1) / 2].v] = i;
        trocar_node(&h->array[i], &h->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void prim(Grafo *g, int inicio) {
    int V = g->V;
    int *pai = (int*) malloc(V * sizeof(int));
    int *chave = (int*) malloc(V * sizeof(int));
    MinHeap* h = (MinHeap*) malloc(sizeof(MinHeap));
    h->array = (HeapNode*) malloc(V * sizeof(HeapNode));
    h->pos = (int*) malloc(V * sizeof(int));
    h->tamanho = V;

    for (int i = 0; i < V; ++i) {
        pai[i] = -1;
        chave[i] = INT_MAX;
        h->array[i].v = i;
        h->array[i].chave = chave[i];
        h->pos[i] = i;
    }

    chave[inicio] = 0;
    h->array[inicio].chave = 0;
    decrementar_chave(h, inicio, chave[inicio]);

    int custo_total = 0;

    while (h->tamanho > 0) {
        HeapNode min_node = extrair_min(h);
        int u = min_node.v;

        if (pai[u] != -1) custo_total += chave[u];

        No* r = g->adj[u];
        while (r != NULL) {
            int v = r->v;
            // Se v ainda está no heap e peso da aresta é menor que a chave atual de v
            if (h->pos[v] < h->tamanho && r->peso < chave[v]) {
                chave[v] = r->peso;
                pai[v] = u;
                decrementar_chave(h, v, chave[v]); // O(log V)
            }
            r = r->prox;
        }
    }

    printf("Arestas da AGM (Prim):\n");
    for (int i = 0; i < V; ++i) {
        if (pai[i] != -1) {
            printf("(%d, %d) - Peso: %d\n", pai[i], i, chave[i]);
        }
    }
    printf("Custo total da AGM (Prim): %d\n", custo_total);

    free(pai); free(chave); free(h->pos); free(h->array); free(h);
}

/* ============================================================
 * FUNÇÕES DE CRIAÇÃO E MANIPULAÇÃO DO GRAFO (Lista Adjacência)
 * ============================================================ */

Grafo* criar_grafo(int V) {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->V = V;
    g->adj = (No**) malloc(V * sizeof(No*));
    for (int i = 0; i < V; i++) g->adj[i] = NULL;
    return g;
}

void adicionar_aresta(Grafo* g, int u, int v, int peso) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->v = v;
    novo_no->peso = peso;
    novo_no->prox = g->adj[u];
    g->adj[u] = novo_no;

    novo_no = (No*) malloc(sizeof(No));
    novo_no->v = u;
    novo_no->peso = peso;
    novo_no->prox = g->adj[v];
    g->adj[v] = novo_no;
}

void liberar_grafo(Grafo* g) {
    for (int i = 0; i < g->V; i++) {
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