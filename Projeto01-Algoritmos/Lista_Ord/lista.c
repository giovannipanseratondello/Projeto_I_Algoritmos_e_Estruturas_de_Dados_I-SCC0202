#include "lista_ord.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no{
    ITEM *item;
    struct no *next;
} NO;

struct lista
{
    int size;
    NO *begin;
    NO *end;
    bool ordered;
};

bool lista_vazia(LISTA_ORD *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho(LISTA_ORD *lista){
    if(lista == NULL) return -1;
    return lista->size;
}

LISTA_ORD *lista_ord_criar(){
    LISTA_ORD *l = malloc(sizeof(LISTA_ORD));
    if(l == NULL) return NULL;

    l->begin = NULL;
    l->end = NULL;
    l->size = 0;
    return l;
}

bool lista_ord_inserir(LISTA_ORD *l, ITEM *item, char *campo) {
    if (l == NULL || item == NULL) return false;

    NO *new = malloc(sizeof(NO));
    if (new == NULL) return false;
    new->item = item;
    new->next = NULL;

    NO *aux = l->begin, *prior = NULL; 
    
    while (aux != NULL && item_comparar(aux->item, item, campo) < 0) {
        prior = aux;
        aux = aux->next;
    }

    // Insere no início
    if (prior == NULL) {
        new->next = l->begin;
        l->begin = new;
    } 

    else {
        prior->next = new;
        new->next = aux;
    }
    
    if (aux == NULL) {
        l->end = new;
    }

    l->size++;
    return true;
}

ITEM *lista_remover_inicio(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    NO *remove = lista->begin;
    ITEM *item = remove->item;

    if(remove->next == NULL) lista->end = NULL;
    
    lista->begin = remove->next;

    free(remove);
    lista->size--;
    return item;
}

ITEM *lista_remover_fim(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    NO *remove = lista->end;
    ITEM *item = remove->item;

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

ITEM *lista_remover(LISTA_ORD *lista, int posicao){
    if(lista == NULL || lista_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_remover_inicio(lista);
    if(posicao == lista->size - 1) return lista_remover_fim(lista);

    NO *aux = lista->begin, *prior = NULL; // Prior nunca será NULL, pois para ele ser, posição tem que ser igual a 0 e ela nunca será nessa parte do código!
    for(int i = 0; i != posicao; i++){ // Posição nunca será igual a  0 ou ao tamanho da lista pois ele já teria caído nos if´s acima. 
        prior = aux;
        aux = aux->next;
    }

    prior->next = aux->next;
    ITEM *item = aux->item;

    free(aux);
    lista->size--;
    return item;
    }

void lista_ord_apagar(LISTA_ORD **lista){
    if(lista == NULL || *lista == NULL) return;

    ITEM *item;
    while(!lista_vazia(*lista)){
        item = lista_remover_inicio(*lista);
        item_apagar(&item);
    }
    free(*lista);
    *lista = NULL;
    return;
}

ITEM *lista_primeiro(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    return lista->begin->item;
}

ITEM *lista_ultimo(LISTA_ORD *lista){
    if(lista == NULL || lista_vazia(lista)) return NULL;

    return lista->end->item;
}

ITEM *lista_obter(LISTA_ORD *lista, int posicao){
    if(lista == NULL || lista_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

    if(posicao == 0) return lista_primeiro(lista);
    if(posicao == lista->size - 1) return lista_ultimo(lista);

    NO *aux = lista->begin;
    for(int i = 0; aux != NULL && i != posicao; i++) aux = aux->next;

    if(aux == NULL) return NULL;
    
    return aux->item;
}

void lista_buscar(LISTA_ORD *l, void (*processar)(ITEM *item, void *), void *contexto){
    if (l == NULL || lista_vazia(l) || processar == NULL) {
        return;
    }

    NO *aux = l->begin;
    while (aux != NULL) {
        
        
        processar(aux->item, contexto);
        
        aux = aux->next;
    }
}

