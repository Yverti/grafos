#include <stdio.h>
#include "coloracao.h"

int main() {
    // Grafo 1: Um grafo que pode se beneficiar da heurística Welsh-Powell
    printf("=== Testando Grafo 1 ===\n");
    GrafoLista* g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1);
    adicionar_aresta(g1, 0, 2);
    adicionar_aresta(g1, 1, 2);
    adicionar_aresta(g1, 1, 3);
    adicionar_aresta(g1, 2, 3);
    adicionar_aresta(g1, 3, 4);

    int num_cores_guloso = 0;
    int num_cores_welsh = 0;

    printf("\n[Algoritmo Guloso]\n");
    coloracao_gulosa(g1, &num_cores_guloso);

    printf("\n[Heuristica Welsh-Powell]\n");
    coloracao_welsh_powell(g1, &num_cores_welsh);

    if (eh_bipartido(g1)) {
        printf("\nO Grafo 1 eh Bipartido!\n");
    } else {
        printf("\nO Grafo 1 NAO eh Bipartido.\n");
    }

    liberar_grafo(g1);

    // Grafo 2: Um grafo claramente bipartido (ex: um ciclo de tamanho par - quadrado)
    printf("\n=== Testando Grafo 2 (Ciclo Par) ===\n");
    GrafoLista* g2 = criar_grafo(4);
    adicionar_aresta(g2, 0, 1);
    adicionar_aresta(g2, 1, 2);
    adicionar_aresta(g2, 2, 3);
    adicionar_aresta(g2, 3, 0);

    printf("\n[Heuristica Welsh-Powell]\n");
    coloracao_welsh_powell(g2, &num_cores_welsh);

    if (eh_bipartido(g2)) {
        printf("\nO Grafo 2 eh Bipartido! (Numero cromatico = 2)\n");
    } else {
        printf("\nO Grafo 2 NAO eh Bipartido.\n");
    }

    liberar_grafo(g2);

    return 0;
}