#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

typedef struct lista LISTA_ENC;

LISTA_ENC *lista_enc_criar(void);

void lista_enc_apagar(LISTA_ENC **lista, void (*liberar_item)(void **item));

bool lista_enc_inserir_inicio(LISTA_ENC *lista, void *item);

void lista_enc_percorrer(LISTA_ENC *l, void (*processar)(void *item, void *contexto), void *contexto);

#endif