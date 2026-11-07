#ifndef COBERTURA_H
#define COBERTURA_H

#include <stdbool.h>

// Estrutura para representar o Grafo via Matriz de Adjacência
typedef struct {
    int num_vertices;
    int **adj;
} Grafo;

// Funções de manipulação do grafo
Grafo* criar_grafo(int n);
void liberar_grafo(Grafo* g);
void adicionar_aresta(Grafo* g, int u, int v);

// Funções principais da prática
bool eh_cobertura(Grafo* g, const bool* solucao);
int cobertura_minima_bt(Grafo* g, bool* melhor_solucao);
int cobertura_gulosa(Grafo* g, bool* solucao_gulosa);

#endif // COBERTURA_H