#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_cabeca.h"

typedef struct no NO;

struct no {
    void *item;
    NO *proximo;
    NO *anterior;
};

struct lista_cabeca {
    NO *cabeca;
    int tamanho;
};

static NO *no_criar(void *item) {
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

    lista->cabeca = (NO *) malloc(sizeof(NO));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    lista->cabeca->item = NULL;
    lista->cabeca->proximo = lista->cabeca;
    lista->cabeca->anterior = lista->cabeca;
    lista->tamanho = 0;

    return lista;
}

void lista_cabeca_apagar(LISTA_CABECA **lista, void (*liberar_item)(void **item)) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->cabeca->proximo;
    while (atual != (*lista)->cabeca) {
        NO *temp = atual;
        atual = atual->proximo;
        
        if (temp->item != NULL && liberar_item != NULL) {
            liberar_item(&(temp->item));
        }
        free(temp);
    }

    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;
}

bool lista_vazia_cabeca(LISTA_CABECA *lista) {
    if (lista == NULL || lista->cabeca == NULL) return true;
    return (lista->tamanho == 0);
}

int lista_tamanho_cabeca(LISTA_CABECA *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

bool lista_cabeca_inserir_inicio(LISTA_CABECA *lista, void *item) {
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

void lista_cabeca_percorrer(LISTA_CABECA *l, void (*processar)(void *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;
    NO *aux = l->cabeca->proximo; 
    while (aux != l->cabeca) {
        processar(aux->item, contexto);
        aux = aux->proximo;
    }
}