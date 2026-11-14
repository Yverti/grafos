#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

Grafo* grafo_para_tsp(const Grafo *g);
int tsp_forca_bruta(const Grafo *g, int *melhor_rota);
int tsp_vizinho_mais_proximo(const Grafo *g, int *rota);
int tsp_insercao(const Grafo *g, int *rota);

#endif