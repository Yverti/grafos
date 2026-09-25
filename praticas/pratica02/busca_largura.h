#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include <stdbool.h>

typedef struct No {
    int vertice;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    No **adj;
} GrafoLista;

typedef struct {
    int *dados;
    int capacidade;
    int inicio;
    int fim;
    int tamanho;
} Fila;

GrafoLista *criar_grafo(int n);
void adicionar_aresta(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);

Fila *criar_fila(int capacidade);
bool fila_vazia(const Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);
void liberar_fila(Fila *f);

void bfs(GrafoLista *g, int origem, int *dist, int *pred);
bool eh_bipartido(GrafoLista *g);

#endif