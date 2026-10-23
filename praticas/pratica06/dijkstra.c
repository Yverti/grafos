#include "dijkstra.h"

// --- Funções do Grafo ---

GrafoListaPonderado* criar_grafo(int num_vertices) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = num_vertices;
    g->listas = (NoAresta**) malloc(num_vertices * sizeof(NoAresta*));
    
    for (int i = 0; i < num_vertices; i++) {
        g->listas[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    // Aresta u -> v
    NoAresta *novo = (NoAresta*) malloc(sizeof(NoAresta));
    novo->destino = v;
    novo->peso = peso;
    novo->proximo = g->listas[u];
    g->listas[u] = novo;

    // Aresta v -> u (Grafo não-direcionado)
    novo = (NoAresta*) malloc(sizeof(NoAresta));
    novo->destino = u;
    novo->peso = peso;
    novo->proximo = g->listas[v];
    g->listas[v] = novo;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        NoAresta *atual = g->listas[i];
        while (atual) {
            NoAresta *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(g->listas);
    free(g);
}

// --- Funções Auxiliares do Min-Heap ---

static void trocar_nos(MinHeap *heap, int i, int j) {
    NoHeap temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;

    heap->posicao[heap->dados[i].vertice] = i;
    heap->posicao[heap->dados[j].vertice] = j;
}

static void min_heapify(MinHeap *heap, int idx) {
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < heap->tamanho && heap->dados[esq].distancia < heap->dados[menor].distancia)
        menor = esq;

    if (dir < heap->tamanho && heap->dados[dir].distancia < heap->dados[menor].distancia)
        menor = dir;

    if (menor != idx) {
        trocar_nos(heap, idx, menor);
        min_heapify(heap, menor);
    }
}

// --- Funções Principais do Min-Heap ---

MinHeap* criar_min_heap(int capacidade) {
    MinHeap *heap = (MinHeap*) malloc(sizeof(MinHeap));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    heap->dados = (NoHeap*) malloc(capacidade * sizeof(NoHeap));
    heap->posicao = (int*) malloc(capacidade * sizeof(int));

    for (int i = 0; i < capacidade; i++) {
        heap->posicao[i] = -1;
    }
    return heap;
}

void liberar_min_heap(MinHeap *heap) {
    if (heap) {
        free(heap->dados);
        free(heap->posicao);
        free(heap);
    }
}

bool esta_no_min_heap(MinHeap *heap, int vertice) {
    return (heap->posicao[vertice] != -1 && heap->posicao[vertice] < heap->tamanho);
}

NoHeap extrair_minimo(MinHeap *heap) {
    if (heap->tamanho == 0) {
        NoHeap vazio = {-1, -1};
        return vazio;
    }

    NoHeap raiz = heap->dados[0];
    NoHeap ultimo = heap->dados[heap->tamanho - 1];

    heap->dados[0] = ultimo;
    heap->posicao[ultimo.vertice] = 0;
    heap->posicao[raiz.vertice] = -1; // Marcado fora do heap

    heap->tamanho--;
    min_heapify(heap, 0);

    return raiz;
}

void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia) {
    int i = heap->posicao[vertice];
    if (i == -1) return;

    heap->dados[i].distancia = nova_distancia;

    while (i > 0 && heap->dados[i].distancia < heap->dados[(i - 1) / 2].distancia) {
        trocar_nos(heap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

// --- Algoritmo de Dijkstra ---

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->num_vertices;
    MinHeap *heap = criar_min_heap(V);

    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        pred[v] = -1;
        heap->dados[v].vertice = v;
        heap->dados[v].distancia = INF;
        heap->posicao[v] = v;
    }

    heap->tamanho = V;
    dist[origem] = 0;
    diminuir_chave(heap, origem, 0);

    while (heap->tamanho > 0) {
        NoHeap min_no = extrair_minimo(heap);
        int u = min_no.vertice;

        if (dist[u] == INF) break; // Restante inalcançável

        NoAresta *adj = g->listas[u];
        while (adj != NULL) {
            int v = adj->destino;
            int peso = adj->peso;

            if (esta_no_min_heap(heap, v) && dist[u] != INF && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pred[v] = u;
                diminuir_chave(heap, v, dist[v]);
            }
            adj = adj->proximo;
        }
    }

    liberar_min_heap(heap);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino == origem) {
        printf("%d", origem);
        return;
    }
    if (pred[destino] == -1) {
        printf("Não existe caminho");
        return;
    }
    reconstruir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}
