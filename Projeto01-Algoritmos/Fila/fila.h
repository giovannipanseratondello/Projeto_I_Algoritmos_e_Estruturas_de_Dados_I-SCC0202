#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct fila FILA;

/* Criação e destruição */
FILA *fila_criar(void);
void fila_apagar(FILA **f);

/* Operações básicas */
bool fila_inserir(FILA *f, ITEM *item);
ITEM *fila_remover(FILA *f);
ITEM *fila_primeiro(FILA *f);

/* Informações */
bool fila_vazia(FILA *f);
int fila_tamanho(FILA *f);

/* Auxiliar */
void fila_imprimir(FILA *f);

void fila_percorrer(FILA *f, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif