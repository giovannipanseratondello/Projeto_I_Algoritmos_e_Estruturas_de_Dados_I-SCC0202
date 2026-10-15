#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA_CABECA;

LISTA_CABECA *lista_cabeca_criar(void);

void lista_cabeca_apagar(LISTA_CABECA **lista);

bool lista_cabeca_inserir_inicio(LISTA_CABECA *lista, ITEM *item);

void lista_cabeca_percorrer(LISTA_CABECA *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif 