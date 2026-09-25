#include <stdio.h>
#include "conectividade.h"
#include "planaridade.h"

int main() {
    printf("=== Testando Grafo 1: Duas componentes conectadas por ponte ===\n");
    GrafoLista* g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1);
    adicionar_aresta(g1, 1, 2);
    adicionar_aresta(g1, 2, 0); // Triângulo
    adicionar_aresta(g1, 2, 3); // Ponte
    adicionar_aresta(g1, 3, 4); 

    dfs_articulacoes(g1);
    detectar_pontes(g1);
    eh_planar_euler(g1);
    heuristica_kuratowski(g1);
    liberar_grafo(g1);

    printf("\n=== Testando Grafo 2: K5 (Nao Planar) ===\n");
    GrafoLista* g2 = criar_grafo(5);
    for(int i = 0; i < 5; i++) {
        for(int j = i+1; j < 5; j++) {
            adicionar_aresta(g2, i, j);
        }
    }
    dfs_articulacoes(g2);
    detectar_pontes(g2);
    eh_planar_euler(g2);
    heuristica_kuratowski(g2);
    liberar_grafo(g2);

    return 0;
}