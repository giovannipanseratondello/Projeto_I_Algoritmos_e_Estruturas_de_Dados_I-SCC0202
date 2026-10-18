#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"

typedef struct no{
    void *item;
    struct no *prior;
} NO;

struct pilha{
    int size;
    NO *top;
};

/* Informações */
bool pilha_vazia(PILHA *p){
    if(p == NULL || p->size == 0) return true;
    else return false;
}
int pilha_tamanho(PILHA *p){
    if(p == NULL) return -1;

    return p->size;
}

PILHA *pilha_criar(){
    PILHA *p = (PILHA *) malloc(sizeof(PILHA));
    if(p == NULL) return NULL;
    
    p->top = NULL;
    p->size = 0;
    return p;
}

/* Operações da pilha */
bool pilha_empilhar(PILHA *p, void *item){
    if(p == NULL || item == NULL) return false;

    NO *novo = (NO *) malloc(sizeof(NO));
    if(novo == NULL) return false;
    
    novo->item = item;
    novo->prior = p->top;
    p->top = novo;

    p->size++;
    return true;
}

void *pilha_desempilhar(PILHA *p){
    if(p == NULL || pilha_vazia(p)) return NULL;

    NO *remove = p->top;
    void *item = remove->item;
    
    p->top = remove->prior;
    p->size--;

    free(remove);
    return item;
}

void *pilha_topo(PILHA *p){
    if(p == NULL || pilha_vazia(p)) return NULL;

    return p->top->item;
}

void pilha_apagar(PILHA **p, void (*liberar_item)(void **item)){
    if(p == NULL || *p == NULL) return;

    void *item;
    while(!pilha_vazia(*p)){
        item = pilha_desempilhar(*p);
        if (liberar_item != NULL && item != NULL) {
            liberar_item(&item);
        }
    }
    free(*p);
    *p = NULL;
}

void pilha_percorrer(PILHA *p, void (*processar)(void *item, void *contexto), void *contexto) {
    if (p == NULL || processar == NULL) return;
    NO *aux = p->top; 
    while (aux != NULL) {
        processar(aux->item, contexto);
        aux = aux->prior;
    }
}