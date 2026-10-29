#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

// Inicializa o grafo
GrafoListaPonderado* criar_grafo(int num_vertices) {
    GrafoListaPonderado* g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = num_vertices;
    g->lista_adj = (Aresta**) malloc(num_vertices * sizeof(Aresta*));
    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
    }
    return g;
}

// Adiciona aresta direcionada
void adicionar_aresta(GrafoListaPonderado* g, int origem, int destino, int peso) {
    Aresta* nova_aresta = (Aresta*) malloc(sizeof(Aresta));
    nova_aresta->destino = destino;
    nova_aresta->peso = peso;
    nova_aresta->prox = g->lista_adj[origem];
    g->lista_adj[origem] = nova_aresta;
}

// Libera memória
void liberar_grafo(GrafoListaPonderado* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta* atual = g->lista_adj[i];
        while (atual != NULL) {
            Aresta* prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(g->lista_adj);
    free(g);
}

// Reconstrói e imprime o ciclo negativo
void reconstruir_ciclo_negativo(int vertice_ciclo, int *pred, int num_vertices) {
    int atual = vertice_ciclo;

    // 1. Retroceder num_vertices vezes para garantir que estamos DENTRO do ciclo
    for (int i = 0; i < num_vertices; i++) {
        atual = pred[atual];
    }

    // 2. Gravar o caminho do ciclo
    int inicio_ciclo = atual;
    int *ciclo = (int*) malloc((num_vertices + 1) * sizeof(int));
    int tam_ciclo = 0;

    do {
        ciclo[tam_ciclo++] = atual;
        atual = pred[atual];
    } while (atual != inicio_ciclo);
    ciclo[tam_ciclo++] = inicio_ciclo;

    // 3. Imprimir (como caminhamos por pred, os vértices estão de trás pra frente)
    printf("-> Ciclo negativo detectado: ");
    for (int i = tam_ciclo - 1; i >= 0; i--) {
        printf("%d", ciclo[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(ciclo);
}

// Algoritmo de Bellman-Ford
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->num_vertices;

    // Passo 1: Inicialização
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[origem] = 0;

    // Passo 2: Relaxação das arestas (V - 1 vezes)
    for (int i = 1; i <= V - 1; i++) {
        for (int u = 0; u < V; u++) {
            if (dist[u] == INF) continue;
            
            Aresta* atual = g->lista_adj[u];
            while (atual != NULL) {
                int v = atual->destino;
                int peso = atual->peso;
                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pred[v] = u;
                }
                atual = atual->prox;
            }
        }
    }

    // Passo 3: Detecção de ciclos de custo negativo (N-ésima iteração)
    for (int u = 0; u < V; u++) {
        if (dist[u] == INF) continue;
        
        Aresta* atual = g->lista_adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;
            if (dist[u] + peso < dist[v]) {
                // Atualiza o predecessor para garantir que o ciclo possa ser rastreado corretamente
                pred[v] = u; 
                reconstruir_ciclo_negativo(v, pred, V);
                return 0; // 0 significa que achou ciclo negativo
            }
            atual = atual->prox;
        }
    }

    return 1; // 1 significa sucesso (sem ciclos negativos)
}