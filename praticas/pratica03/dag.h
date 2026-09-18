#ifndef DAG_H
#define DAG_H

// Estrutura de um nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* prox;
} No;

// Estrutura do Grafo (Lista de Adjacência)
typedef struct GrafoLista {
    int num_vertices;
    No** adjacencias;
} GrafoLista;

// Funções base do grafo
GrafoLista* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista* g, int origem, int destino);
void liberar_grafo(GrafoLista* g);

// Funções requeridas pela prática
int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
int eh_dag(GrafoLista *g);

#endif