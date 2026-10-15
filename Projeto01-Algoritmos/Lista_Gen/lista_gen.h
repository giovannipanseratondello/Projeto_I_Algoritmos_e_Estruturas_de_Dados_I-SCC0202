#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include <stdbool.h>
#include "../Item/item.h"

typedef struct lista_gen LISTA_GEN;

// Cria a lista generalizada vazia
LISTA_GEN *lista_gen_criar(void);

// Destrói a lista generalizada e libera os nós de agrupamento (sem destruir os itens em si, se gerenciados externamente)
void lista_gen_destruir(LISTA_GEN **l);

// Insere um item de forma hierárquica usando duas chaves de texto genéricas (grupo e subgrupo)
bool lista_gen_inserir(LISTA_GEN *l, void *item, const char *chave_grupo, const char *chave_subgrupo);

// Permite percorrer todos os itens armazenados na estrutura passando um callback genérico
void lista_gen_percorrer(LISTA_GEN *l, void (*processar)(ITEM *item, void *contexto), void *contexto);

#endif