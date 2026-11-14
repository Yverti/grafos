#include "tps.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Grafo* grafo_para_tsp(const Grafo *g) {
    if (!g) return NULL;

    Grafo *tsp = criar_grafo(g->n);
    if (!tsp) return NULL;

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (i == j) {
                tsp->matriz[i][j] = 0;
            } else if (g->matriz[i][j] != 0) {
                tsp->matriz[i][j] = 1;
            } else {
                tsp->matriz[i][j] = 2;
            }
        }
    }
    return tsp;
}

static void trocar(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void gerar_permutacoes_tsp(const Grafo *g, int *perm, int idx,
                                  int custo_atual, int *melhor_custo,
                                  int *melhor_rota) {
    if (idx == g->n) {
        int custo_total = custo_atual + g->matriz[perm[g->n - 1]][perm[0]];
        if (custo_total < *melhor_custo) {
            *melhor_custo = custo_total;
            if (melhor_rota) {
                memcpy(melhor_rota, perm, g->n * sizeof(int));
            }
        }
        return;
    }

    for (int i = idx; i < g->n; i++) {
        trocar(&perm[idx], &perm[i]);
        int novo_custo = custo_atual + g->matriz[perm[idx - 1]][perm[idx]];
        gerar_permutacoes_tsp(g, perm, idx + 1, novo_custo, melhor_custo, melhor_rota);
        trocar(&perm[idx], &perm[i]);
    }
}

int tsp_forca_bruta(const Grafo *g, int *melhor_rota) {
    if (!g || g->n <= 0) return 0;
    if (g->n == 1) {
        if (melhor_rota) melhor_rota[0] = 0;
        return 0;
    }

    int *perm = (int*) malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        perm[i] = i;
    }

    int melhor_custo = INT_MAX;
    // Fixa perm[0] = 0 e permuta os (n - 1) vértices restantes
    gerar_permutacoes_tsp(g, perm, 1, 0, &melhor_custo, melhor_rota);

    free(perm);
    return melhor_custo;
}

int tsp_vizinho_mais_proximo(const Grafo *g, int *rota) {
    if (!g || g->n <= 0) return 0;
    if (g->n == 1) {
        if (rota) rota[0] = 0;
        return 0;
    }

    bool *visitado = (bool*) calloc(g->n, sizeof(bool));
    int *caminho = (int*) malloc(g->n * sizeof(int));

    caminho[0] = 0;
    visitado[0] = true;
    int custo_total = 0;

    for (int passo = 1; passo < g->n; passo++) {
        int atual = caminho[passo - 1];
        int prox = -1;
        int menor_dist = INT_MAX;

        for (int v = 0; v < g->n; v++) {
            if (!visitado[v] && g->matriz[atual][v] < menor_dist) {
                menor_dist = g->matriz[atual][v];
                prox = v;
            }
        }

        caminho[passo] = prox;
        visitado[prox] = true;
        custo_total += menor_dist;
    }

    custo_total += g->matriz[caminho[g->n - 1]][caminho[0]];

    if (rota) {
        memcpy(rota, caminho, g->n * sizeof(int));
    }

    free(caminho);
    free(visitado);
    return custo_total;
}

int tsp_insercao(const Grafo *g, int *rota) {
    if (!g || g->n <= 0) return 0;
    if (g->n == 1) {
        if (rota) rota[0] = 0;
        return 0;
    }

    bool *na_rota = (bool*) calloc(g->n, sizeof(bool));
    int *sub_rota = (int*) malloc(g->n * sizeof(int));

    // Inicia ciclo com vértice 0 e seu vizinho mais próximo
    sub_rota[0] = 0;
    na_rota[0] = true;

    int mais_prox = 1;
    int menor_d = INT_MAX;
    for (int v = 1; v < g->n; v++) {
        if (g->matriz[0][v] < menor_d) {
            menor_d = g->matriz[0][v];
            mais_prox = v;
        }
    }

    sub_rota[1] = mais_prox;
    na_rota[mais_prox] = true;
    int tam = 2;

    while (tam < g->n) {
        // 1. Seleção: vértice k fora da rota mais próximo de algum vértice na rota
        int k_escolhido = -1;
        int menor_dist_rota = INT_MAX;

        for (int v = 0; v < g->n; v++) {
            if (!na_rota[v]) {
                for (int i = 0; i < tam; i++) {
                    int u = sub_rota[i];
                    if (g->matriz[v][u] < menor_dist_rota) {
                        menor_dist_rota = g->matriz[v][u];
                        k_escolhido = v;
                    }
                }
            }
        }

        // 2. Inserção: aresta (u, w) da sub-rota que minimiza d(u,k) + d(k,w) - d(u,w)
        int melhor_pos = 0;
        int menor_aumento = INT_MAX;

        for (int i = 0; i < tam; i++) {
            int u = sub_rota[i];
            int w = sub_rota[(i + 1) % tam];
            int aumento = g->matriz[u][k_escolhido] +
                          g->matriz[k_escolhido][w] -
                          g->matriz[u][w];

            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i;
            }
        }

        // Desloca elementos para inserir k_escolhido após melhor_pos
        for (int i = tam; i > melhor_pos + 1; i--) {
            sub_rota[i] = sub_rota[i - 1];
        }
        sub_rota[melhor_pos + 1] = k_escolhido;
        na_rota[k_escolhido] = true;
        tam++;
    }

    int custo_total = 0;
    for (int i = 0; i < g->n; i++) {
        custo_total += g->matriz[sub_rota[i]][sub_rota[(i + 1) % g->n]];
    }

    if (rota) {
        memcpy(rota, sub_rota, g->n * sizeof(int));
    }

    free(sub_rota);
    free(na_rota);
    return custo_total;
}