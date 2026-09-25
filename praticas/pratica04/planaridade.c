#include <stdio.h>
#include "planaridade.h"

bool eh_planar_euler(GrafoLista* g) {
    int n = g->num_vertices;
    int m = g->num_arestas;
    
    if (n < 3) return true; // Casos triviais
    
    // Euler exige que para grafos conexos e planares, m <= 3n - 6
    if (m > 3 * n - 6) {
        printf("Grafo nao planar! Falhou no corolario de Euler (m=%d > %d).\n", m, 3*n - 6);
        return false;
    }
    
    printf("Grafo passou no teste de Euler (m=%d <= %d). Pode ser planar.\n", m, 3*n - 6);
    return true;
}

bool heuristica_kuratowski(GrafoLista* g) {
    int n = g->num_vertices;
    if (n > 10) {
        printf("Heuristica de Kuratowski suportada apenas para n <= 10.\n");
        return true; 
    }

    int* graus = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) graus[i] = 0;

    for (int i = 0; i < n; i++) {
        No* atual = g->adj[i];
        while (atual != NULL) {
            graus[i]++;
            atual = atual->prox;
        }
    }

    // Heurística K5: Precisa de pelo menos 5 vértices de grau >= 4
    int candidatos_k5 = 0;
    for (int i = 0; i < n; i++) {
        if (graus[i] >= 4) candidatos_k5++;
    }
    if (candidatos_k5 >= 5) {
        printf("Heuristica Kuratowski: Possivel subgrafo K5 detectado!\n");
        free(graus);
        return false;
    }

    // Heurística K3,3: Precisa de pelo menos 6 vértices de grau >= 3
    int candidatos_k33 = 0;
    for (int i = 0; i < n; i++) {
        if (graus[i] >= 3) candidatos_k33++;
    }
    if (candidatos_k33 >= 6) {
        printf("Heuristica Kuratowski: Possivel subgrafo K3,3 detectado!\n");
        free(graus);
        return false;
    }

    free(graus);
    printf("Heuristica Kuratowski: Nenhuma restricao obvia (K5 ou K3,3) encontrada.\n");
    return true;
}