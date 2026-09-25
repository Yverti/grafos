#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include <stdbool.h>
#include "conectividade.h"

// Verifica planaridade pela corolário da fórmula de Euler (m <= 3n - 6)
bool eh_planar_euler(GrafoLista* g);

// Heurística baseada no Teorema de Kuratowski para grafos pequenos (n <= 10)
bool heuristica_kuratowski(GrafoLista* g);

#endif