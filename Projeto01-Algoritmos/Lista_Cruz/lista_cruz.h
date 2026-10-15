#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_cruz LISTA_CRUZ;

LISTA_CRUZ *lista_cruz_criar(void);

void lista_cruz_apagar(LISTA_CRUZ **lc);

bool lista_cruz_inserir(LISTA_CRUZ *lc, ITEM *item, char *chave1, char *chave2);

void lista_cruz_imprimir(LISTA_CRUZ *lc);

void lista_cruz_percorrer(LISTA_CRUZ *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif