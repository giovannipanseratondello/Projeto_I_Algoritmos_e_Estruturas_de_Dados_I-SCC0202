#ifndef LISTA_SEQ_H
#define LISTA_SEQ_H

#include <stdbool.h>

typedef struct lista LISTA_SEQ;

LISTA_SEQ *lista_seq_criar(void);

void lista_seq_apagar(LISTA_SEQ **lista, void (*liberar_item)(void **item));

bool lista_seq_inserir_inicio(LISTA_SEQ *lista, void *item);

void lista_seq_percorrer(LISTA_SEQ *l, void (*processar)(void *item, void *contexto), void *contexto);

#endif