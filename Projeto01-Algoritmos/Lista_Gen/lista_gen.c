#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_gen.h"

// Nó da lista de itens armazenados (átomos)
typedef struct no_item {
    void *item;
    struct no_item *prox;
} NO_ITEM;

// Nó do Subgrupo (Nível 2)
typedef struct no_subgrupo {
    char *chave;
    NO_ITEM *primeiro_item;
    NO_ITEM *ultimo_item;
    struct no_subgrupo *prox;
} NO_SUBGRUPO;

// Nó do Grupo Principal (Nível 1)
typedef struct no_grupo {
    char *chave;
    NO_SUBGRUPO *primeiro_subgrupo;
    struct no_grupo *prox;
} NO_GRUPO;

// Estrutura principal da Lista Generalizada Hierárquica
struct lista_gen {
    NO_GRUPO *primeiro_grupo;
    int tamanho;
};

LISTA_GEN *lista_gen_criar(void) {
    LISTA_GEN *l = malloc(sizeof(LISTA_GEN));
    if (l != NULL) {
        l->primeiro_grupo = NULL;
        l->tamanho = 0;
    }
    return l;
}

void lista_gen_destruir(LISTA_GEN **l) {
    if (l == NULL || *l == NULL) return;

    NO_GRUPO *g = (*l)->primeiro_grupo;
    while (g != NULL) {
        NO_SUBGRUPO *s = g->primeiro_subgrupo;
        while (s != NULL) {
            NO_ITEM *i = s->primeiro_item;
            while (i != NULL) {
                NO_ITEM *prox_i = i->prox;
                free(i);
                i = prox_i;
            }
            NO_SUBGRUPO *prox_s = s->prox;
            free(s->chave);
            free(s);
            s = prox_s;
        }
        NO_GRUPO *prox_g = g->prox;
        free(g->chave);
        free(g);
        g = prox_g;
    }

    free(*l);
    *l = NULL;
}

bool lista_gen_inserir(LISTA_GEN *l, void *item, const char *chave_grupo, const char *chave_subgrupo) {
    if (l == NULL || item == NULL) return false;

    // Se as chaves vierem nulas, tratamos como strings vazias ou coringa
    const char *g_chave = (chave_grupo != NULL) ? chave_grupo : "";
    const char *s_chave = (chave_subgrupo != NULL) ? chave_subgrupo : "";

    // 1. Localiza ou cria o Grupo (Nível 1)
    NO_GRUPO *g_ant = NULL;
    NO_GRUPO *g = l->primeiro_grupo;
    while (g != NULL && g->chave != NULL && strcmp(g->chave, g_chave) != 0) {
        g_ant = g;
        g = g->prox;
    }

    if (g == NULL) {
        g = malloc(sizeof(NO_GRUPO));
        if (g == NULL) return false;
        g->chave = strdup(g_chave);
        if (g->chave == NULL) { free(g); return false; }
        g->primeiro_subgrupo = NULL;
        g->prox = NULL;

        if (g_ant == NULL) {
            l->primeiro_grupo = g;
        } else {
            g_ant->prox = g;
        }
    }

    // 2. Localiza ou cria o Subgrupo (Nível 2)
    NO_SUBGRUPO *s_ant = NULL;
    NO_SUBGRUPO *s = g->primeiro_subgrupo;
    while (s != NULL && s->chave != NULL && strcmp(s->chave, s_chave) != 0) {
        s_ant = s;
        s = s->prox;
    }

    if (s == NULL) {
        s = malloc(sizeof(NO_SUBGRUPO));
        if (s == NULL) return false;
        s->chave = strdup(s_chave);
        s->primeiro_item = NULL;
        s->ultimo_item = NULL;
        s->prox = NULL;

        if (s_ant == NULL) {
            g->primeiro_subgrupo = s;
        } else {
            s_ant->prox = s;
        }
    }

    // 3. Insere o item na lista encadeada de itens do subgrupo
    NO_ITEM *novo_i = malloc(sizeof(NO_ITEM));
    if (novo_i == NULL) return false;
    novo_i->item = item;
    novo_i->prox = NULL;

    if (s->primeiro_item == NULL) {
        s->primeiro_item = novo_i;
    } else {
        s->ultimo_item->prox = novo_i;
    }
    s->ultimo_item = novo_i;
    l->tamanho++;

    return true;
}

void lista_gen_percorrer(LISTA_GEN *l, void (*processar)(ITEM *item, void *contexto), void *contexto) {
    if (l == NULL || processar == NULL) return;

    NO_GRUPO *g = l->primeiro_grupo;
    while (g != NULL) {
        NO_SUBGRUPO *s = g->primeiro_subgrupo;
        while (s != NULL) {
            NO_ITEM *i = s->primeiro_item;
            while (i != NULL) {
                processar(i->item, contexto);
                i = i->prox;
            }
            s = s->prox;
        }
        g = g->prox;
    }
}