#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#include <stdbool.h>

// Estrutura de Nó para a Lista de Adjacência
typedef struct No {
    int vizinho;
    struct No* prox;
} No;

// Estrutura do Grafo
typedef struct GrafoLista {
    int num_vertices;
    int num_arestas;
    No** adj;
} GrafoLista;

// Funções básicas do Grafo
GrafoLista* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista* g, int u, int v);
void liberar_grafo(GrafoLista* g);

// Funções da Prática 04
void dfs_articulacoes(GrafoLista* g);
void detectar_pontes(GrafoLista* g);

#endif