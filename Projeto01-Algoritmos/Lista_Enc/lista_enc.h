#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista LISTA_ENC;


LISTA_ENC *lista_enc_criar(void);

void lista_enc_apagar(LISTA_ENC **lista);

bool lista_enc_inserir_inicio(LISTA_ENC *lista, ITEM *item);

void lista_enc_percorrer(LISTA_ENC *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif