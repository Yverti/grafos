#include "dijkstra.h"

int main() {
    // Mapeamento dos vértices:
    // A = 0, B = 1, C = 2, D = 3, E = 4, F = 5
    int num_vertices = 6;
    GrafoListaPonderado *g = criar_grafo(num_vertices);

    // Adiciona as arestas conforme a imagem da aula
    adicionar_aresta(g, 0, 1, 4);  // A - B (4)
    adicionar_aresta(g, 0, 2, 2);  // A - C (2)
    adicionar_aresta(g, 1, 2, 1);  // B - C (1)
    adicionar_aresta(g, 1, 3, 5);  // B - D (5)
    adicionar_aresta(g, 1, 4, 8);  // B - E (8)
    adicionar_aresta(g, 2, 3, 4);  // C - D (4)
    adicionar_aresta(g, 2, 4, 10); // C - E (10)
    adicionar_aresta(g, 3, 4, 2);  // D - E (2)
    adicionar_aresta(g, 3, 5, 1);  // D - F (1)
    adicionar_aresta(g, 4, 5, 3);  // E - F (3)

    int dist[num_vertices];
    int pred[num_vertices];
    int origem = 0; // Vértice A

    dijkstra(g, origem, dist, pred);

    char nomes[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    printf("=== Resultado do Algoritmo de Dijkstra (Origem: %c) ===\n\n", nomes[origem]);
    for (int i = 0; i < num_vertices; i++) {
        printf("Distancia ate %c (%d): %d\n", nomes[i], i, dist[i]);
    }

    printf("\n=== Reconstrucao do Caminho Minimo de A (0) ate F (5) ===\n");
    printf("Caminho: ");
    reconstruir_caminho(origem, 5, pred);
    printf("\nCusto Total: %d\n", dist[5]);

    liberar_grafo(g);
    return 0;
}