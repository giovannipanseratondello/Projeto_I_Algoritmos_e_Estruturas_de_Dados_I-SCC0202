#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

typedef struct no{
    void *item;
    struct no *next;
} NO;

struct fila
{
    int size;
    NO *begin;
    NO *end;
};


FILA *fila_criar(void){
    FILA *f = (FILA *) malloc(sizeof(FILA));
    if(f == NULL) return NULL;

    f->end = NULL;
    f->begin = NULL;
    f->size = 0;

    return f;
}

void fila_apagar(FILA **f, void (*liberar_item)(void **item)){
    if (f == NULL || *f == NULL) return;

    void *item;
    while(!fila_vazia(*f)){
        item = fila_remover(*f);
        if (liberar_item != NULL && item != NULL) {
            liberar_item(&item);
        }
    }
    
    free(*f);
    *f = NULL;
}

bool fila_inserir(FILA *f, void *item){
    if(f == NULL || item == NULL) return false;

    NO *novo = (NO *) malloc(sizeof(NO));
    if(novo == NULL) return false;
    novo->item = item;

    if(fila_vazia(f)){
        f->begin = novo;
    }
    else{
        f->end->next = novo;
    }

    f->end = novo;
    novo->next = NULL;
    f->size++;

    return true;
}

void *fila_remover(FILA *f){
    if(f == NULL || fila_vazia(f)) return NULL;

    NO *remove = f->begin;
    void *item = remove->item;

    f->begin = remove->next;

    if(f->begin == NULL) f->end = NULL;

    f->size--;
    free(remove);

    return item;
}

void *fila_primeiro(FILA *f){
    if(f == NULL || fila_vazia(f)) return NULL;

    return f->begin->item;
}

bool fila_vazia(FILA *f){
    if(f == NULL || f->size == 0) return true;
    else return false;
}

int fila_tamanho(FILA *f){
    if(f == NULL) return -1;
    return f->size;
}

void fila_percorrer(FILA *f, void (*processar)(void *item, void *contexto), void *contexto) {
    if (f == NULL || processar == NULL) return;
    NO *aux = f->begin;
    while (aux != NULL) {
        processar(aux->item, contexto);
        aux = aux->next;
    }
}