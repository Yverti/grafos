#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    int n = 5;
    
    printf("=== GRAFO COM MATRIZ DE ADJACÊNCIA ===\n\n");
    GrafoMatriz *gm = criar_grafo_matriz(n);
    
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 3);
    inserir_aresta_matriz(gm, 2, 4);
    inserir_aresta_matriz(gm, 3, 4);
    
    imprimir_matriz(gm);
    
    printf("\nGraus dos vértices:\n");
    for (int i = 0; i < n; i++) {
        printf("Vértice %d: grau %d\n", i, grau_matriz(gm, i));
    }
    
    printf("\nVerificando adjacências:\n");
    printf("0 e 1 adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 1) ? "Sim" : "Não");
    printf("0 e 3 adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 3) ? "Sim" : "Não");
    
    liberar_grafo_matriz(gm);
    
    printf("\n\n=== GRAFO COM LISTA DE ADJACÊNCIA ===\n\n");
    GrafoLista *gl = criar_grafo_lista(n);
    
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 3);
    inserir_aresta_lista(gl, 2, 4);
    inserir_aresta_lista(gl, 3, 4);
    
    imprimir_lista(gl);
    
    printf("\nGraus dos vértices:\n");
    for (int i = 0; i < n; i++) {
        printf("Vértice %d: grau %d\n", i, grau_lista(gl, i));
    }
    
    printf("\nVerificando adjacências:\n");
    printf("0 e 1 adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim" : "Não");
    printf("0 e 3 adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 3) ? "Sim" : "Não");
    
    printf("\nRemovendo aresta 0-1...\n");
    remover_aresta_lista(gl, 0, 1);
    
    printf("0 e 1 adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim" : "Não");
    
    liberar_grafo_lista(gl);
    
    return 0;
}
