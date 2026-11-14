#include "hamiltoniano.h"
#include <stdlib.h>

Grafo* criar_grafo(int n) {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    if (!g) return NULL;

    g->n = n;
    g->matriz = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->matriz[i] = (int*) calloc(n, sizeof(int));
    }
    return g;
}

void destruir_grafo(Grafo *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        free(g->matriz[i]);
    }
    free(g->matriz);
    free(g);
}

void adicionar_aresta(Grafo *g, int u, int v, int peso) {
    if (!g || u < 0 || u >= g->n || v < 0 || v >= g->n || u == v) return;
    g->matriz[u][v] = peso;
    g->matriz[v][u] = peso;
}

static bool bt_caminho(const Grafo *g, int pos, int *caminho, bool *visitado) {
    if (pos == g->n) {
        return true;
    }

    int u = caminho[pos - 1];
    for (int v = 0; v < g->n; v++) {
        if (!visitado[v] && g->matriz[u][v] != 0) {
            visitado[v] = true;
            caminho[pos] = v;

            if (bt_caminho(g, pos + 1, caminho, visitado)) {
                return true;
            }

            visitado[v] = false;
        }
    }
    return false;
}

bool encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho) {
    if (!g || g->n <= 0 || !caminho) return false;

    bool *visitado = (bool*) calloc(g->n, sizeof(bool));
    if (!visitado) return false;

    for (int inicio = 0; inicio < g->n; inicio++) {
        for (int i = 0; i < g->n; i++) visitado[i] = false;

        caminho[0] = inicio;
        visitado[inicio] = true;

        if (bt_caminho(g, 1, caminho, visitado)) {
            free(visitado);
            return true;
        }
    }

    free(visitado);
    return false;
}

static bool bt_ciclo(const Grafo *g, int pos, int *ciclo, bool *visitado) {
    if (pos == g->n) {
        int ultimo = ciclo[pos - 1];
        int primeiro = ciclo[0];
        return (g->matriz[ultimo][primeiro] != 0);
    }

    int u = ciclo[pos - 1];
    for (int v = 1; v < g->n; v++) {
        if (!visitado[v] && g->matriz[u][v] != 0) {
            visitado[v] = true;
            ciclo[pos] = v;

            if (bt_ciclo(g, pos + 1, ciclo, visitado)) {
                return true;
            }

            visitado[v] = false;
        }
    }
    return false;
}

bool encontrar_ciclo_hamiltoniano_bt(const Grafo *g, int *ciclo) {
    if (!g || g->n < 3 || !ciclo) return false;

    bool *visitado = (bool*) calloc(g->n, sizeof(bool));
    if (!visitado) return false;

    ciclo[0] = 0;
    visitado[0] = true;

    bool encontrou = bt_ciclo(g, 1, ciclo, visitado);
    free(visitado);
    return encontrou;
}