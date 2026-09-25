#ifndef COLORACAO_H
#define COLORACAO_H

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

// Funções da Prática 05
void coloracao_gulosa(GrafoLista *g, int *num_cores);
void coloracao_welsh_powell(GrafoLista *g, int *num_cores);
bool eh_bipartido(GrafoLista *g);

#endif