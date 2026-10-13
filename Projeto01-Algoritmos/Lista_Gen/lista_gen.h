#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_gen LISTA_GEN;

LISTA_GEN *lista_gen_criar(void);

void lista_gen_apagar(LISTA_GEN **l);

bool lista_gen_inserir_item(LISTA_GEN *l, ITEM *item);

bool lista_gen_inserir_sublista(LISTA_GEN *l, LISTA_GEN *sublista);

bool lista_gen_vazia(LISTA_GEN *l);

void lista_gen_imprimir(LISTA_GEN *l);

#endif