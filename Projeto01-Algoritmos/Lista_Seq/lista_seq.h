#ifndef LISTA_SEQ_H
#define LISTA_SEQ_H

#include <stdbool.h>
#include "../Item/item.h"
#define MAX_LENGTH 90000

typedef struct lista LISTA_SEQ;

LISTA_SEQ *lista_seq_criar(void);

void lista_seq_apagar(LISTA_SEQ **lista);

bool lista_seq_inserir_inicio(LISTA_SEQ *lista, ITEM *item);

void lista_seq_percorrer(LISTA_SEQ *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif