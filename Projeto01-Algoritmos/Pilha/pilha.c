#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"

typedef struct no{
    ITEM *item;
    struct no *prior;

}NO;

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
    PILHA *p = malloc(sizeof(PILHA));
    if(p == NULL) return NULL;
    
    p->top = NULL;
    p->size = 0;
    return p;
}

/* Operações da pilha */
bool pilha_empilhar(PILHA *p, ITEM *item){
    if(p == NULL || item == NULL) return false;

    NO *new = malloc(sizeof(NO));
    if(new == NULL) return false;
    
    new->item = item;
    new->prior = p->top;
    p->top = new;

    p->size++;
    return true;
}

ITEM *pilha_desempilhar(PILHA *p){
    if(p == NULL || pilha_vazia(p)) return NULL;

    NO *remove = p->top;
    ITEM *item = remove->item;
    
    p->top = remove->prior;
    p->size--;

    free(remove);
    return item;
}

ITEM *pilha_topo(PILHA *p){
    if(p == NULL || pilha_vazia(p)) return NULL;

    return p->top->item;
}

void pilha_apagar(PILHA **p){
    if(p == NULL || *p == NULL) return;

    ITEM *item;
    while(!pilha_vazia(*p)){
        item = pilha_desempilhar(*p);
        item_apagar(&item);
    }
    free(item);
    free(*p);
    *p = NULL;
}

void pilha_percorrer(PILHA *p, void (*processar)(ITEM *item, void *contexto), void *contexto) {
    if (p == NULL || processar == NULL) return;
    NO *aux = p->top; // ajuste para o nome do campo do seu nó topo
    while (aux != NULL) {
        processar(aux->item, contexto);
        aux = aux->prior;
    }
}
