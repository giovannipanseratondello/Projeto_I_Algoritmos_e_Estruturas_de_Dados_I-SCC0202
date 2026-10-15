#include "lista_enc.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no
{
   ITEM *item;
   struct no *next;
   struct no *prior;
} NO;

struct lista
{
    int size;
    NO *begin;
    NO *end;
};


bool lista_enc_vazia(LISTA_ENC *lista){
    if(lista == NULL || lista->size == 0) return true;
    return false;
}

int lista_enc_tamanho(LISTA_ENC *lista){
    if(lista == NULL) return -1;
    return lista->size;
}

LISTA_ENC *lista_enc_criar(void){
   LISTA_ENC *l = malloc(sizeof(LISTA_ENC));
    if(l == NULL) return NULL;

    l->begin = NULL;
    l->end = NULL;
    l->size = 0;

    return l;
}


bool lista_enc_inserir_inicio(LISTA_ENC *lista, ITEM *item){
   if(lista == NULL || item == NULL) return false;

   NO *new = malloc(sizeof(NO));
   if(new == NULL) return false;
   new->item = item;

   if(lista->begin == NULL) lista->end = new;
   else lista->begin->prior = new;

   new->next = lista->begin;
   new->prior = NULL;
   lista->begin = new;

   lista->size++;
   return true;
}


ITEM *lista_enc_remover_inicio(LISTA_ENC *lista){
   if(lista == NULL || lista_enc_vazia(lista)) return NULL;

   NO *remove = lista->begin;
   ITEM *item = remove->item;

   lista->begin = remove->next;

   if(lista->begin == NULL){
      lista->end = NULL;
   } 
   else lista->begin->prior = NULL;
   
   lista->size--;
   free(remove);
   return item;
}

ITEM *lista_enc_remover_fim(LISTA_ENC *lista){
   if(lista == NULL || lista_enc_vazia(lista)) return NULL;

   NO *remove = lista->end;
   ITEM *item = remove->item;

   lista->end = remove->prior;

   if(lista->end == NULL) lista->begin = NULL;
   else lista->end->next = NULL;

   lista->size--;
   free(remove);
   return item;
}

ITEM *lista_remover(LISTA_ENC *lista, int posicao){
   if(lista == NULL || lista_enc_vazia(lista) || posicao < 0 || posicao >= lista->size) return NULL;

   if(posicao == 0) return lista_enc_remover_inicio(lista);
   if(posicao == lista->size - 1) return lista_enc_remover_fim(lista);

   NO *aux;
   if(posicao >= lista->size / 2){
      aux = lista->end;
      for(int i = lista->size - 1; i != posicao; i--) aux = aux->prior;
   }
   else{
      aux = lista->begin;
      for(int i = 0; i != posicao; i++) aux = aux->next;
   }


   aux->prior->next = aux->next;
   aux->next->prior = aux->prior;

   ITEM *item = aux->item;
   free(aux);
   lista->size--;
   return item;
}

void lista_enc_apagar(LISTA_ENC **lista){
   if(lista == NULL || *lista == NULL) return;

    ITEM *item;
    while(!lista_enc_vazia(*lista)){
        item = lista_enc_remover_inicio(*lista);
        item_apagar(&item);
    }
    free(*lista);
    *lista = NULL;
    return;
}

void lista_enc_percorrer(LISTA_ENC *l, void (*processar)(ITEM *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;
    NO *aux = l->begin; // ajuste para o primeiro nó da lista encadeada
    while (aux != NULL) {
        processar(aux->item, contexto);
        aux = aux->next;
    }
}
