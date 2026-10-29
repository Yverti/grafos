#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#define INF 999999999

// Estrutura de uma aresta para a lista de adjacência
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta* prox;
} Aresta;

// Estrutura do Grafo Ponderado Direcionado
typedef struct {
    int num_vertices;
    Aresta** lista_adj;
} GrafoListaPonderado;

// Funções para manipulação do grafo
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado* g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado* g);

// Funções principais do algoritmo
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(int vertice_ciclo, int *pred, int num_vertices);

#endif