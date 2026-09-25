#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

// --- Funções Auxiliares do Grafo ---
GrafoLista* criar_grafo(int num_vertices) {
    GrafoLista* g = (GrafoLista*)malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->adj = (No**)malloc(num_vertices * sizeof(No*));
    for (int i = 0; i < num_vertices; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoLista* g, int u, int v) {
    No* novo_u = (No*)malloc(sizeof(No));
    novo_u->vizinho = v;
    novo_u->prox = g->adj[u];
    g->adj[u] = novo_u;

    No* novo_v = (No*)malloc(sizeof(No));
    novo_v->vizinho = u;
    novo_v->prox = g->adj[v];
    g->adj[v] = novo_v;

    g->num_arestas++;
}

void liberar_grafo(GrafoLista* g) {
    for (int i = 0; i < g->num_vertices; i++) {
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

// --- Variáveis globais para o algoritmo de Tarjan ---
int tempo;

// --- Encontrar Articulações ---
void dfs_art_recursivo(GrafoLista* g, int u, int* descoberta, int* low, int* pai, bool* articulacao) {
    int filhos = 0;
    descoberta[u] = low[u] = ++tempo;

    No* atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->vizinho;
        if (descoberta[v] == -1) { // v não foi visitado
            filhos++;
            pai[v] = u;
            dfs_art_recursivo(g, v, descoberta, low, pai, articulacao);

            // Verifica se a subárvore enraizada em v tem uma conexão de retorno
            if (low[v] < low[u]) low[u] = low[v];

            // u é articulação nos seguintes casos:
            // 1. u é raiz da DFS e tem pelo menos dois filhos independentes
            if (pai[u] == -1 && filhos > 1)
                articulacao[u] = true;
            // 2. u não é raiz e o valor low de v é >= descoberta de u
            if (pai[u] != -1 && low[v] >= descoberta[u])
                articulacao[u] = true;
        } 
        else if (v != pai[u]) {
            // Atualiza low[u] para aresta de retorno
            if (descoberta[v] < low[u]) low[u] = descoberta[v];
        }
        atual = atual->prox;
    }
}

void dfs_articulacoes(GrafoLista* g) {
    int* descoberta = (int*)malloc(g->num_vertices * sizeof(int));
    int* low = (int*)malloc(g->num_vertices * sizeof(int));
    int* pai = (int*)malloc(g->num_vertices * sizeof(int));
    bool* articulacao = (bool*)malloc(g->num_vertices * sizeof(bool));

    for (int i = 0; i < g->num_vertices; i++) {
        pai[i] = -1;
        descoberta[i] = -1;
        articulacao[i] = false;
    }
    
    tempo = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        if (descoberta[i] == -1) {
            dfs_art_recursivo(g, i, descoberta, low, pai, articulacao);
        }
    }

    printf("Vertices de Corte (Articulacoes): ");
    bool tem_art = false;
    for (int i = 0; i < g->num_vertices; i++) {
        if (articulacao[i]) {
            printf("%d ", i);
            tem_art = true;
        }
    }
    if (!tem_art) printf("Nenhum");
    printf("\n");

    free(descoberta); free(low); free(pai); free(articulacao);
}

// --- Encontrar Pontes ---
void dfs_pontes_recursivo(GrafoLista* g, int u, int* descoberta, int* low, int* pai) {
    descoberta[u] = low[u] = ++tempo;

    No* atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->vizinho;
        if (descoberta[v] == -1) {
            pai[v] = u;
            dfs_pontes_recursivo(g, v, descoberta, low, pai);

            if (low[v] < low[u]) low[u] = low[v];

            // Se o menor vértice alcançável a partir de v é posterior a u, (u-v) é ponte
            if (low[v] > descoberta[u]) {
                printf("Ponte encontrada: (%d - %d)\n", u, v);
            }
        } 
        else if (v != pai[u]) {
            if (descoberta[v] < low[u]) low[u] = descoberta[v];
        }
        atual = atual->prox;
    }
}

void detectar_pontes(GrafoLista* g) {
    int* descoberta = (int*)malloc(g->num_vertices * sizeof(int));
    int* low = (int*)malloc(g->num_vertices * sizeof(int));
    int* pai = (int*)malloc(g->num_vertices * sizeof(int));

    for (int i = 0; i < g->num_vertices; i++) {
        pai[i] = -1;
        descoberta[i] = -1;
    }

    tempo = 0;
    printf("Buscando Pontes...\n");
    for (int i = 0; i < g->num_vertices; i++) {
        if (descoberta[i] == -1) {
            dfs_pontes_recursivo(g, i, descoberta, low, pai);
        }
    }

    free(descoberta); free(low); free(pai);
}