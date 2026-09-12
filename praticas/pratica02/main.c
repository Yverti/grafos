#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

int main() {
    int n = 5;
    GrafoLista *g = criar_grafo(n);
    
    // Cria um grafo linear (0-1-2-3-4)
    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 2, 3);
    adicionar_aresta(g, 3, 4);
    
    printf("=== Testes Pratica 02 - Buscas BFS e DFS ===\n\n");
    
    int dist[n], pred[n];
    bfs(g, 0, dist, pred);
    
    printf("[BFS] Distancias a partir da origem (0):\n");
    for (int i = 0; i < n; i++) {
        printf("Vertice %d -> Distancia: %d, Predecessor: %d\n", i, dist[i], pred[i]);
    }
    
    printf("\n[DFS] Componentes conexos: %d\n", contar_componentes(g));
    
    printf("\n[Grafo Linear] Testando Ciclos e Biparticao:\n");
    printf("- Tem ciclo? %s\n", tem_ciclo(g) ? "SIM" : "NAO");
    printf("- Eh bipartido? %s\n", eh_bipartido(g) ? "SIM" : "NAO");
    
    // Adiciona uma aresta para criar um ciclo ímpar (0-1-2-3-4-0)
    printf("\n[Adicionando aresta (4,0)...]\n");
    adicionar_aresta(g, 4, 0);
    
    printf("- Tem ciclo? %s\n", tem_ciclo(g) ? "SIM" : "NAO");
    printf("- Eh bipartido? %s\n", eh_bipartido(g) ? "SIM" : "NAO"); // Ciclo ímpar -> Não bipartido

    liberar_grafo(g);
    return 0;
}