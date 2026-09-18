#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h" // Reaproveita estruturas do grafo

// Pilha (LIFO) para DFS iterativa (opcional, foco na recursiva abaixo)
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

// Funções DFS
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo_desc, int *tempo_fin, int *tempo);
int contar_componentes(GrafoLista *g);
bool tem_ciclo(GrafoLista *g);

#endif // BUSCA_PROFUNDIDADE_H