#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX

// Estrutura para os nós da lista de adjacência
typedef struct NoAresta {
    int destino;
    int peso;
    struct NoAresta *proximo;
} NoAresta;

// Estrutura do Grafo representado por Lista de Adjacência Ponderada
typedef struct {
    int num_vertices;
    NoAresta **listas;
} GrafoListaPonderado;

// Estruturas do Min-Heap
typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade;
    int tamanho;
    int *posicao; // Mapeia vértice -> índice no heap
} MinHeap;

// Funções de Gerenciamento do Grafo
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

// Funções do Min-Heap
MinHeap* criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *heap);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);
bool esta_no_min_heap(MinHeap *heap, int vertice);

// Algoritmo de Dijkstra e utilitários
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif