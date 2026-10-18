#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>

typedef struct pilha PILHA;

PILHA *pilha_criar(void);

void pilha_apagar(PILHA **p, void (*liberar_item)(void **item));

bool pilha_empilhar(PILHA *p, void *item);

void pilha_percorrer(PILHA *p, void (*processar)(void *item, void *contexto), void *contexto);

#endif