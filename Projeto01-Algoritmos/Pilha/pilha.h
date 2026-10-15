#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct pilha PILHA;

/* Criação e destruição */
PILHA *pilha_criar();
void pilha_apagar(PILHA **p);

/* Operações da pilha */
bool pilha_empilhar(PILHA *p, ITEM *item);

void pilha_percorrer(PILHA *p, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif