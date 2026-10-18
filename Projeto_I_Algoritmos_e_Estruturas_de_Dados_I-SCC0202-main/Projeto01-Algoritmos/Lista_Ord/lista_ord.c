#include "lista_ord.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no{
    void *item;
    struct no *next;
} NO;

struct lista
{
    int size;
    NO *begin;
    NO *end;
    bool ordered;
};

bool lista_vazia_ord(LISTA_ORD *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho_ord(LISTA_ORD *lista){
    if(lista == NULL) return -1;
    return lista->size;
}

LISTA_ORD *lista_ord_criar(){
    LISTA_ORD *l = (LISTA_ORD *) malloc(sizeof(LISTA_ORD));
    if(l == NULL) return NULL;

    l->begin = NULL;
    l->end = NULL;
    l->size = 0;
    return l;
}

bool lista_ord_inserir(LISTA_ORD *l, void *item, char *campo, int (*item_comparar_func)(void *, void *, char *)) {
    if (l == NULL || item == NULL || item_comparar_func == NULL) return false;

    NO *novo = (NO *) malloc(sizeof(NO));
    if (novo == NULL) return false;
    novo->item = item;
    novo->next = NULL;

    NO *aux = l->begin, *prior = NULL; 
    
    while (aux != NULL && item_comparar_func(aux->item, item, campo) < 0) {
        prior = aux;
        aux = aux->next;
    }

    if (prior == NULL) {
        novo->next = l->begin;
        l->begin = novo;
    } 

    else {
        prior->next = novo;
        novo->next = aux;
    }
    
    if (aux == NULL) {
        l->end = novo;
    }

    l->size++;
    return true;
}

void *lista_remover_inicio_ord(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia_ord(lista)) return NULL;

    NO *remove = lista->begin;
    void *item = remove->item;

    if(remove->next == NULL) lista->end = NULL;
    
    lista->begin = remove->next;

    free(remove);
    lista->size--;
    return item;
}

void *lista_ord_remover_fim(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia_ord(lista)) return NULL;

    NO *remove = lista->end;
    void *item = remove->item;

    if(lista->size == 1){
        lista->begin = NULL;
        lista->end = NULL;
    } 
    else{
        NO *aux = lista->begin;

        while(aux->next != lista->end) aux = aux->next;

        aux->next = NULL;
        lista->end = aux;
    }

    free(remove);
    lista->size--;
    return item;
}

void *lista_remover_ordenada(LISTA_ORD *lista, int posicao){
    if(lista == NULL || lista_vazia_ord(lista) || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_remover_inicio_ord(lista);
    if(posicao == lista->size - 1) return lista_ord_remover_fim(lista);

    NO *aux = lista->begin, *prior = NULL;
    for(int i = 0; i != posicao; i++){
        prior = aux;
        aux = aux->next;
    }

    prior->next = aux->next;
    void *item = aux->item;

    free(aux);
    lista->size--;
    return item;
}

void lista_ord_apagar(LISTA_ORD **lista, void (*liberar_item)(void **item)){
    if(lista == NULL || *lista == NULL) return;

    void *item;
    while(!lista_vazia_ord(*lista)){
        item = lista_remover_inicio_ord(*lista);
        if (liberar_item != NULL && item != NULL) {
            liberar_item(&item);
        }
    }
    free(*lista);
    *lista = NULL;
    return;
}

void lista_ord_percorrer(LISTA_ORD *l, void (*processar)(void *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;
    NO *aux = l->begin; 
    while (aux != NULL) {
        processar(aux->item, contexto);
        aux = aux->next;
    }
}