#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

typedef struct fila FILA;

FILA *fila_criar(void);
void fila_apagar(FILA **f, void (*liberar_item)(void **item));

bool fila_inserir(FILA *f, void *item);
void *fila_remover(FILA *f);
void *fila_primeiro(FILA *f);

bool fila_vazia(FILA *f);
int fila_tamanho(FILA *f);

void fila_percorrer(FILA *f, void (*processar)(void *item, void *contexto), void *contexto);

#endif