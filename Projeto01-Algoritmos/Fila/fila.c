#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

typedef struct no{
    ITEM *item;
    struct no *next;
} NO;

struct fila
{
    int size;
    NO *begin;
    NO *end;
};


FILA *fila_criar(void){
    FILA *f = malloc(sizeof(FILA));
    if(f == NULL) return NULL;

    f->end = NULL;
    f->begin = NULL;
    f->size = 0;

    return f;
}

void fila_apagar(FILA **f){
    if (f == NULL || *f == NULL) return;

    ITEM *item;
    while(!fila_vazia(*f)){
        item = fila_remover(*f);
        item_apagar(&item);
    }
    
    free(*f);
    *f = NULL;
}

/* Operações básicas */
bool fila_inserir(FILA *f, ITEM *item){
    if(f == NULL || item == NULL) return false;

    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    new->item = item;

    if(fila_vazia(f)){
        f->begin = new;
    }
    else{
        f->end->next = new;
    }

    f->end = new;
    new->next = NULL;
    f->size++;

    return true;
}

ITEM *fila_remover(FILA *f){
    if(f == NULL || fila_vazia(f)) return NULL;

    NO *remove = f->begin;
    ITEM *item = remove->item;

    f->begin = remove->next;

    if(f->begin == NULL) f->end = NULL;

    f->size--;
    free(remove);

    return item;
}

ITEM *fila_primeiro(FILA *f){
    if(f == NULL || fila_vazia(f)) return NULL;

    return f->begin->item;
}

/* Informações */
bool fila_vazia(FILA *f){
    if(f == NULL || f->size == 0) return true;
    else return false;
}

int fila_tamanho(FILA *f){
    if(f == NULL) return -1;
    return f->size;
}

/* Auxiliar */
void fila_imprimir(FILA *f){
    if(f == NULL || fila_vazia(f)) return;

    NO *aux = f->begin;

    printf("Inicio -> ");
    while(aux != NULL){
        printf("(%d) ",item_get_chave(aux->item));
        aux = aux->next;
    }
    printf("<- Fim\n");
}

void fila_buscar(FILA *f, void (*processar)(ITEM *item, void *), void *contexto){
    if (f == NULL || fila_vazia(f) || processar == NULL) {
        return;
    }

    NO *aux = f->begin;
    while (aux != NULL) {
        
        
        processar(aux->item, contexto);
        
        aux = aux->next;
    }
}