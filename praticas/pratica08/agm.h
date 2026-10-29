#ifndef AGM_H
#define AGM_H

#include <stdbool.h>

// Estruturas Obrigatórias
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

// Estruturas para o Grafo (Lista de Adjacência para o Prim)
typedef struct No {
    int v, peso;
    struct No* prox;
} No;

typedef struct {
    No** adj;
    int V;
} Grafo;

// Funções do DSU (Union-Find)
DSU* criar_dsu(int n);
void liberar_dsu(DSU *d);
int find(DSU *d, int x);
void unir(DSU *d, int x, int y);

// Funções do Algoritmo de Kruskal
void kruskal(Aresta *arestas, int E, int V);

// Funções Auxiliares do Grafo (Prim)
Grafo* criar_grafo(int V);
void adicionar_aresta(Grafo* g, int u, int v, int peso);
void liberar_grafo(Grafo* g);

// Função do Algoritmo de Prim
void prim(Grafo *g, int inicio);

#endif // AGM_H