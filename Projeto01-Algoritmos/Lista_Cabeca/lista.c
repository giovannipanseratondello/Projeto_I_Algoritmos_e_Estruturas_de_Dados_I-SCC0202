#include <stdio.h>
#include <stdlib.h>
#include "lista_cabeca.h"

typedef struct no NO;

struct no {
    ITEM *item;
    NO *proximo;
    NO *anterior;
};

struct lista {
    NO *cabeca; // Nó sentinela que não armazena dado de ITEM
    int tamanho;
};

/* Helper interno para alocar um novo nó */
static NO *no_criar(ITEM *item) {
    NO *novo = (NO *) malloc(sizeof(NO));
    if (novo != NULL) {
        novo->item = item;
        novo->proximo = NULL;
        novo->anterior = NULL;
    }
    return novo;
}

LISTA_CABECA *lista_cabeca_criar(void) {
    LISTA_CABECA *lista = (LISTA_CABECA *) malloc(sizeof(LISTA_CABECA));
    if (lista == NULL) return NULL;

    // Aloca o nó cabeça/sentinela real exigido pelo projeto
    lista->cabeca = (NO *) malloc(sizeof(NO));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    // Em uma lista duplamente encadeada circular/sentinela,
    // o nó cabeça aponta para si mesmo quando vazia.
    lista->cabeca->item = NULL;
    lista->cabeca->proximo = lista->cabeca;
    lista->cabeca->anterior = lista->cabeca;
    lista->tamanho = 0;

    return lista;
}

void lista_cabeca_apagar(LISTA_CABECA **lista) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->cabeca->proximo;
    while (atual != (*lista)->cabeca) {
        NO *temp = atual;
        atual = atual->proximo;
        
        if (temp->item != NULL) {
            item_apagar(&(temp->item));
        }
        free(temp);
    }

    // Libera o nó cabeça real e a estrutura da lista
    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;
}

bool lista_vazia(LISTA_CABECA *lista) {
    if (lista == NULL || lista->cabeca == NULL) return true;
    return (lista->tamanho == 0);
}

int lista_tamanho(LISTA_CABECA *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

bool lista_cabeca_inserir_inicio(LISTA_CABECA *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO *novo = no_criar(item);
    if (novo == NULL) return false;

    NO *primeiro = lista->cabeca->proximo;

    novo->proximo = primeiro;
    novo->anterior = lista->cabeca;
    lista->cabeca->proximo = novo;
    primeiro->anterior = novo;

    lista->tamanho++;
    return true;
}

void lista_cabeca_buscar(LISTA *l, void (*processar)(ITEM *item, void *), void *contexto){
    if (l == NULL || lista_vazia(l) || processar == NULL) {
        return;
    }

    NO *aux = l->cabeca->proximo;
    while (aux != l->cabeca) {
        
        
        processar(aux->item, contexto);
        
        aux = aux->proximo;
    }
}