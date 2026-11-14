#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static void imprimir_rota(const int *rota, int n, bool ciclo) {
    for (int i = 0; i < n; i++) {
        printf("%d ", rota[i]);
        if (i < n - 1) printf("-> ");
    }
    if (ciclo && n > 0) {
        printf("-> %d", rota[0]);
    }
    printf("\n");
}

static Grafo* gerar_grafo_tsp_aleatorio(int n, unsigned int seed) {
    srand(seed);
    Grafo *g = criar_grafo(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int peso = (rand() % 90) + 10; // pesos entre 10 e 99
            adicionar_aresta(g, i, j, peso);
        }
    }
    return g;
}

static void testar_hamiltoniano_e_reducao(void) {
    printf("=== 1. Teste: Caminho/Ciclo Hamiltoniano e Reducao para TSP ===\n");
    int n = 5;
    Grafo *g = criar_grafo(n);

    // Grafo com Ciclo Hamiltoniano: 0 - 1 - 2 - 3 - 4 - 0 e cordas
    adicionar_aresta(g, 0, 1, 1);
    adicionar_aresta(g, 1, 2, 1);
    adicionar_aresta(g, 2, 3, 1);
    adicionar_aresta(g, 3, 4, 1);
    adicionar_aresta(g, 4, 0, 1);
    adicionar_aresta(g, 1, 3, 1);

    int *seq = (int*) malloc(n * sizeof(int));

    if (encontrar_caminho_hamiltoniano_bt(g, seq)) {
        printf("Caminho Hamiltoniano encontrado: ");
        imprimir_rota(seq, n, false);
    } else {
        printf("Nenhum Caminho Hamiltoniano encontrado.\n");
    }

    if (encontrar_ciclo_hamiltoniano_bt(g, seq)) {
        printf("Ciclo Hamiltoniano encontrado:   ");
        imprimir_rota(seq, n, true);
    } else {
        printf("Nenhum Ciclo Hamiltoniano encontrado.\n");
    }

    // Redução: existe ciclo hamiltoniano <=> TSP ótimo tem custo == n
    Grafo *tsp = grafo_para_tsp(g);
    int custo_reducao = tsp_forca_bruta(tsp, seq);
    printf("Custo TSP apos reducao (esperado %d se houver ciclo): %d\n\n", n, custo_reducao);

    free(seq);
    destruir_grafo(tsp);
    destruir_grafo(g);
}

static void executar_experimento(void) {
    printf("=== 2. Experimento TSP: Forca Bruta vs Heuristicas ===\n");
    int tamanhos[] = {5, 8, 10, 12};
    int qtd_testes = 4;

    printf("%-4s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s\n",
           "n", "Custo FB", "Tempo FB(ms)",
           "Custo VMP", "Tempo VMP(ms)",
           "Custo Ins", "Tempo Ins(ms)");
    printf("----------------------------------------------------------------------------------------\n");

    for (int t = 0; t < qtd_testes; t++) {
        int n = tamanhos[t];
        Grafo *g = gerar_grafo_tsp_aleatorio(n, 42 + n);
        int *rota = (int*) malloc(n * sizeof(int));

        clock_t ini, fim;

        // 1. Força Bruta
        ini = clock();
        int custo_fb = tsp_forca_bruta(g, rota);
        fim = clock();
        double tempo_fb = ((double)(fim - ini) / CLOCKS_PER_SEC) * 1000.0;

        // 2. Vizinho Mais Próximo
        ini = clock();
        int custo_vmp = tsp_vizinho_mais_proximo(g, rota);
        fim = clock();
        double tempo_vmp = ((double)(fim - ini) / CLOCKS_PER_SEC) * 1000.0;

        // 3. Inserção do Mais Próximo
        ini = clock();
        int custo_ins = tsp_insercao(g, rota);
        fim = clock();
        double tempo_ins = ((double)(fim - ini) / CLOCKS_PER_SEC) * 1000.0;

        printf("%-4d | %-12d | %-12.3f | %-12d | %-12.3f | %-12d | %-12.3f\n",
               n, custo_fb, tempo_fb, custo_vmp, tempo_vmp, custo_ins, tempo_ins);

        free(rota);
        destruir_grafo(g);
    }
}

int main(void) {
    testar_hamiltoniano_e_reducao();
    executar_experimento();
    return 0;
}