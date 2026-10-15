#include "lista_seq.h"
#include <stdlib.h>
#include <stdio.h>

struct lista
{
    ITEM *itens[MAX_LENGTH];
    int end; // Último item da lista
    int size; // Próximo item
};


bool lista_vazia_seq(LISTA_SEQ *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_tamanho_seq(LISTA_SEQ *lista){
    if(lista == NULL) return -1;
    return lista->size;
}

LISTA_SEQ *lista_seq_criar(void){
    LISTA_SEQ *l = malloc(sizeof(LISTA_SEQ));
    if(l == NULL) return NULL;

    for(int i = 0; i < MAX_LENGTH; i++) l->itens[i] = NULL;

    l->size = 0;
    l->end = -1; // Permite que ao inserir o primeiro elemento, ele referencie corretamente
    return l;
}


bool lista_seq_inserir_inicio(LISTA_SEQ *lista, ITEM *item){
    if(lista == NULL || item == NULL || lista_tamanho_seq(lista) == MAX_LENGTH) return false;

    if(!lista_vazia_seq(lista)){
        for(int i = lista->size; i > 0; i--){
            lista->itens[i] = lista->itens[i - 1];
        }
    }
    lista->itens[0] = item;
    lista->end++;
    lista->size++;
    return true;
}



ITEM *lista_remover_inicio(LISTA_SEQ *lista){
    if(lista == NULL || lista_vazia_seq(lista)) return NULL;

    ITEM *remove = lista->itens[0];
    if(lista->size != 1){
        for(int i = 0; i < lista->size; i++){
            lista->itens[i] = lista->itens[i + 1];
        }
    }
    else lista->itens[0] = NULL;
    lista->end--;
    lista->size--;
    return remove;
}


void lista_seq_apagar(LISTA_SEQ **lista){
    if(lista == NULL || *lista == NULL) return;

    ITEM *item;
    while(!lista_vazia_seq(*lista)){
        item = lista_remover_inicio(*lista);
        item_apagar(&item);
    }
    free(*lista);
    *lista = NULL;
}


void lista_seq_percorrer(LISTA_SEQ *l, void (*processar)(ITEM *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;
    for (int i = 0; i < l->size; i++) { 
        processar(l->itens[i], contexto); 
    }
}
