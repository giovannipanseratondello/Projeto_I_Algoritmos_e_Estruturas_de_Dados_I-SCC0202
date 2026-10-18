#include "lista_seq.h"
#include <stdlib.h>
#include <stdio.h>

struct lista
{
    void **itens;
    int capacidade;
    int end;
    int size;
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
    LISTA_SEQ *l = (LISTA_SEQ *) malloc(sizeof(LISTA_SEQ));
    if(l == NULL) return NULL;

    l->capacidade = 1000; // Capacidade inicial padrão
    l->itens = (void **) malloc(l->capacidade * sizeof(void *));
    if(l->itens == NULL) {
        free(l);
        return NULL;
    }

    for(int i = 0; i < l->capacidade; i++) l->itens[i] = NULL;

    l->size = 0;
    l->end = -1;
    return l;
}

bool lista_seq_inserir_inicio(LISTA_SEQ *lista, void *item){
    if(lista == NULL || item == NULL) return false;

    // Se atingir a capacidade máxima, expande o vetor dinamicamente
    if(lista->size >= lista->capacidade) {
        int nova_capacidade = lista->capacidade * 2;
        void **novos_itens = (void **) realloc(lista->itens, nova_capacidade * sizeof(void *));
        if(novos_itens == NULL) return false;

        lista->itens = novos_itens;
        for(int i = lista->capacidade; i < nova_capacidade; i++) {
            lista->itens[i] = NULL;
        }
        lista->capacidade = nova_capacidade;
    }

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

void *lista_remover_inicio(LISTA_SEQ *lista){
    if(lista == NULL || lista_vazia_seq(lista)) return NULL;

    void *remove = lista->itens[0];
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

void lista_seq_apagar(LISTA_SEQ **lista, void (*liberar_item)(void **item)){
    if(lista == NULL || *lista == NULL) return;

    void *item;
    while(!lista_vazia_seq(*lista)){
        item = lista_remover_inicio(*lista);
        if (liberar_item != NULL && item != NULL) {
            liberar_item(&item);
        }
    }
    free((*lista)->itens);
    free(*lista);
    *lista = NULL;
}

void lista_seq_percorrer(LISTA_SEQ *l, void (*processar)(void *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;
    for (int i = 0; i < l->size; i++) { 
        processar(l->itens[i], contexto); 
    }
}