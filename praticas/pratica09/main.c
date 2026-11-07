#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "cobertura.h"

// Gera um grafo aleatório no modelo Erdős-Rényi com probabilidade p de aresta
Grafo* gerar_grafo_aleatorio(int n, double p) {
    Grafo* g = criar_grafo(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((double)rand() / RAND_MAX < p) {
                adicionar_aresta(g, i, j);
            }
        }
    }
    return g;
}

int main() {
    srand(42); // Semente fixa para reprodutibilidade

    int tamanhos[] = {5, 8, 10, 12, 15};
    int num_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=================================================================================\n");
    printf("%-5s | %-18s | %-18s | %-10s | %-10s\n", "N", "Tempo BT (s)", "Tempo Guloso (s)", "Tam BT", "Tam Guloso");
    printf("=================================================================================\n");

    for (int k = 0; k < num_testes; k++) {
        int n = tamanhos[k];
        Grafo* g = gerar_grafo_aleatorio(n, 0.4);

        bool* sol_bt = (bool*) malloc(n * sizeof(bool));
        bool* sol_gulosa = (bool*) malloc(n * sizeof(bool));

        // Medição do Backtracking
        clock_t inicio = clock();
        int tam_bt = cobertura_minima_bt(g, sol_bt);
        clock_t fim = clock();
        double tempo_bt = (double)(fim - inicio) / CLOCKS_PER_SEC;

        // Medição do Algoritmo Guloso
        inicio = clock();
        int tam_guloso = cobertura_gulosa(g, sol_gulosa);
        fim = clock();
        double tempo_guloso = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("%-5d | %-18.6f | %-18.6f | %-10d | %-10d\n", n, tempo_bt, tempo_guloso, tam_bt, tam_guloso);

        free(sol_bt);
        free(sol_gulosa);
        liberar_grafo(g);
    }
    printf("=================================================================================\n");

    return 0;
}