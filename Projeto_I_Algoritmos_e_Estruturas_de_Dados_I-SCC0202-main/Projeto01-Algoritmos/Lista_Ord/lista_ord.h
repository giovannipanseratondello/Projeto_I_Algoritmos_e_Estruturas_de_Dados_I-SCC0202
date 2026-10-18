#ifndef LISTA_ORD_H
#define LISTA_ORD_H

#include <stdbool.h>

typedef struct lista LISTA_ORD;

LISTA_ORD *lista_ord_criar(void);

void lista_ord_apagar(LISTA_ORD **lista, void (*liberar_item)(void **item));

bool lista_ord_inserir(LISTA_ORD *l, void *item, char *campo, int (*item_comparar_func)(void *, void *, char *));

void lista_ord_percorrer(LISTA_ORD *l, void (*processar)(void *item, void *contexto), void *contexto);

#endif