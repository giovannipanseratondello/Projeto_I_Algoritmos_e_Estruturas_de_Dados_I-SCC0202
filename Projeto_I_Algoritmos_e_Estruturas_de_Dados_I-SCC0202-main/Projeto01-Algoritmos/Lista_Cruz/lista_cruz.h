#ifndef LISTA_Cruz_H
#define LISTA_Cruz_H

#include <stdbool.h>

typedef struct lista_cruz LISTA_CRUZ;

LISTA_CRUZ *lista_cruz_criar(void);

void lista_cruz_apagar(LISTA_CRUZ **lc, void (*liberar_item)(void **item));

bool lista_vazia_cruz(LISTA_CRUZ *lista);

bool lista_cruz_inserir(LISTA_CRUZ *lc, void *item, char *chave1, char *chave2);

void lista_cruz_percorrer(LISTA_CRUZ *l, void (*processar)(void *item, void *contexto), void *contexto);

#endif