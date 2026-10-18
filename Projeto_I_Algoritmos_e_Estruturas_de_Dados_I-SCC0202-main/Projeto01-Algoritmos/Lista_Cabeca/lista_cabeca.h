#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>

typedef struct lista_cabeca LISTA_CABECA;

LISTA_CABECA *lista_cabeca_criar(void);

void lista_cabeca_apagar(LISTA_CABECA **lista, void (*liberar_item)(void **item));

bool lista_vazia_cabeca(LISTA_CABECA *lista);

int lista_tamanho_cabeca(LISTA_CABECA *lista);

bool lista_cabeca_inserir_inicio(LISTA_CABECA *lista, void *item);

void lista_cabeca_percorrer(LISTA_CABECA *l, void (*processar)(void *item, void *contexto), void *contexto);

#endif