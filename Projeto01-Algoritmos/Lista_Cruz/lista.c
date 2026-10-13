#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_cruz.h"

typedef struct no_cruz {
    ITEM *item;
    char *chave_dim1;
    char *chave_dim2;
    struct no_cruz *prox_dim1;
    struct no_cruz *prox_dim2;
} NO_CRUZ;

typedef struct cabecalho_dim {
    char *nome_categoria;
    NO_CRUZ *primeiro;
    struct cabecalho_dim *prox;
} CABECALHO_DIM;

struct lista_cruz {
    NO_CRUZ *todos_os_nos;
    CABECALHO_DIM *cabecas_dim1;
    CABECALHO_DIM *cabecas_dim2;
    int total_itens;
};

LISTA_CRUZ *lista_cruz_criar(void) {
    LISTA_CRUZ *lc = malloc(sizeof(LISTA_CRUZ));
    if (lc != NULL) {
        lc->todos_os_nos = NULL;
        lc->cabecas_dim1 = NULL;
        lc->cabecas_dim2 = NULL;
        lc->total_itens = 0;
    }
    return lc;
}

bool lista_vazia(LISTA_CRUZ *lista){
    if(lista == NULL || lista->total_itens == 0) return true;
    return false;
}

void lista_cruz_apagar(LISTA_CRUZ **lc) {
    if (lc == NULL || *lc == NULL) return;

    NO_CRUZ *atual = (*lc)->todos_os_nos;
    while (atual != NULL) {
        NO_CRUZ *prox = atual->prox_dim1;
        free(atual->chave_dim1);
        free(atual->chave_dim2);
        free(atual);
        atual = prox;
    }

    CABECALHO_DIM *c1 = (*lc)->cabecas_dim1;
    while (c1 != NULL) {
        CABECALHO_DIM *prox = c1->prox;
        free(c1->nome_categoria);
        free(c1);
        c1 = prox;
    }

    CABECALHO_DIM *c2 = (*lc)->cabecas_dim2;
    while (c2 != NULL) {
        CABECALHO_DIM *prox = c2->prox;
        free(c2->nome_categoria);
        free(c2);
        c2 = prox;
    }

    free(*lc);
    *lc = NULL;
}

static CABECALHO_DIM *obter_ou_criar_cabecalho(CABECALHO_DIM **cabeca_lista, const char *nome) {
    CABECALHO_DIM *atual = *cabeca_lista;
    while (atual != NULL) {
        if (strcmp(atual->nome_categoria, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    CABECALHO_DIM *novo = malloc(sizeof(CABECALHO_DIM));
    if (novo == NULL) return NULL;
    novo->nome_categoria = strdup(nome);
    novo->primeiro = NULL;
    novo->prox = *cabeca_lista;
    *cabeca_lista = novo;
    return novo;
}

bool lista_cruz_inserir(LISTA_CRUZ *lc, ITEM *item, char *chave1, char *chave2) {
    if (lc == NULL || item == NULL || chave1 == NULL || chave2 == NULL) return false;

    NO_CRUZ *novo = malloc(sizeof(NO_CRUZ));
    if (novo == NULL) return false;

    novo->item = item;
    novo->chave_dim1 = strdup(chave1);
    novo->chave_dim2 = strdup(chave2);

    novo->prox_dim1 = lc->todos_os_nos;
    lc->todos_os_nos = novo;

    CABECALHO_DIM *cab1 = obter_ou_criar_cabecalho(&(lc->cabecas_dim1), chave1);
    if (cab1 == NULL) {
        free(novo->chave_dim1);
        free(novo->chave_dim2);
        free(novo);
        return false;
    }
    novo->prox_dim1 = cab1->primeiro;
    cab1->primeiro = novo;

    CABECALHO_DIM *cab2 = obter_ou_criar_cabecalho(&(lc->cabecas_dim2), chave2);
    if (cab2 == NULL) {
        free(novo->chave_dim1);
        free(novo->chave_dim2);
        free(novo);
        return false;
    }
    novo->prox_dim2 = cab2->primeiro;
    cab2->primeiro = novo;

    lc->total_itens++;
    return true;
}

void lista_cruz_buscar(LISTA_CRUZ *l, void (*processar)(ITEM *item, void *), void *contexto){
    if (l == NULL || lista_vazia(l) || processar == NULL) {
        return;
    }

    CABECALHO_DIM *cab;
    NO_CRUZ *aux;
    
    for(cab = l->cabecas_dim1; cab != NULL; cab = cab->prox){
        for(aux = cab->primeiro; aux != NULL; aux = aux->prox_dim1){
            processar(aux->item, contexto);
        }
    }


}