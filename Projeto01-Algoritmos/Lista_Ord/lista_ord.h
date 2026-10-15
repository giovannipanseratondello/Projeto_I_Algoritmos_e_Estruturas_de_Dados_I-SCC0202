#ifndef LISTA_ORD_H
#define LISTA_ORD_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA_ORD;

LISTA_ORD *lista_ord_criar();

void lista_ord_apagar(LISTA_ORD **lista);

bool lista_ord_inserir(LISTA_ORD *l, ITEM *item, char *campo);

void lista_ord_percorrer(LISTA_ORD *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif