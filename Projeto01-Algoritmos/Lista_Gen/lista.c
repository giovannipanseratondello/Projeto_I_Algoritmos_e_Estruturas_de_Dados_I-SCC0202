#include <stdio.h>
#include <stdlib.h>
#include "lista_gen.h"

typedef enum { 
    ATOMO = 0, 
    SUBLISTA = 1 
} TipoNo;

typedef struct no_gen {
    TipoNo tipo;
    union {
        ITEM *item;
        LISTA_GEN *sublista;
    } info;
    struct no_gen *prox;
}NO_GEN;

struct lista_gen {
    NO_GEN *inicio;
    NO_GEN *fim;
    int tamanho;
};

LISTA_GEN *lista_gen_criar(void) {
    LISTA_GEN *l = malloc(sizeof(LISTA_GEN));
    if (l != NULL) {
        l->inicio = NULL;
        l->fim = NULL;
        l->tamanho = 0;
    }
    return l;
}

void lista_gen_apagar(LISTA_GEN **l) {
    if (l == NULL || *l == NULL) return;

    NO_GEN *atual = (*l)->inicio;
    while (atual != NULL) {
        NO_GEN *proximo = atual->prox;
        if (atual->tipo == ATOMO) {
            
        } else {
            lista_gen_apagar(&(atual->info.sublista));
        }
        free(atual);
        atual = proximo;
    }
    free(*l);
    *l = NULL;
}

bool lista_gen_inserir_item(LISTA_GEN *l, ITEM *item) {
    if (l == NULL || item == NULL) return false;

    NO_GEN *novo = malloc(sizeof(NO_GEN));
    if (novo == NULL) return false;

    novo->tipo = ATOMO;
    novo->info.item = item;
    novo->prox = NULL;

    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        l->fim->prox = novo;
    }
    l->fim = novo;
    l->tamanho++;
    return true;
}

bool lista_gen_inserir_sublista(LISTA_GEN *l, LISTA_GEN *sublista) {
    if (l == NULL || sublista == NULL) return false;

    NO_GEN *novo = malloc(sizeof(NO_GEN));
    if (novo == NULL) return false;

    novo->tipo = SUBLISTA;
    novo->info.sublista = sublista;
    novo->prox = NULL;

    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        l->fim->prox = novo;
    }
    l->fim = novo;
    l->tamanho++;
    return true;
}

bool lista_gen_vazia(LISTA_GEN *l) {
    return (l == NULL || l->inicio == NULL);
}

void lista_gen_buscar(LISTA_GEN *l, void (*processar)(ITEM *item, void *), void *contexto){
    if (l == NULL || lista_gen_vazia(l) || processar == NULL) {
        return;
    }

    NO_GEN *aux = l->inicio;
    while (aux != NULL) {
        
        if(aux->tipo == ATOMO){
            processar(aux->info.item, contexto);
        }
        else{
            lista_buscar(aux->info.sublista, processar, contexto);
        }
        
        
        aux = aux->prox;
    }
}