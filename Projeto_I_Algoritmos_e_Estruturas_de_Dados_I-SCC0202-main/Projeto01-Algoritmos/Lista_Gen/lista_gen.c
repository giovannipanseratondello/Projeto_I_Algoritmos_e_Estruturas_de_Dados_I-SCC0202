#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_gen.h"

typedef struct no_item {
    void *item;
    struct no_item *prox;
} NO_ITEM;

typedef struct no_subgrupo {
    char *chave;
    NO_ITEM *primeiro_item;
    NO_ITEM *ultimo_item;
    struct no_subgrupo *prox;
} NO_SUBGRUPO;

typedef struct no_grupo {
    char *chave;
    NO_SUBGRUPO *primeiro_subgrupo;
    struct no_grupo *prox;
} NO_GRUPO;

struct lista_gen {
    NO_GRUPO *primeiro_grupo;
    int tamanho;
};

LISTA_GEN *lista_gen_criar(void) {
    LISTA_GEN *l = (LISTA_GEN *) malloc(sizeof(LISTA_GEN));
    if (l != NULL) {
        l->primeiro_grupo = NULL;
        l->tamanho = 0;
    }
    return l;
}

void lista_gen_destruir(LISTA_GEN **l, void (*liberar_item)(void **item)) {
    if (l == NULL || *l == NULL) return;

    NO_GRUPO *g = (*l)->primeiro_grupo;
    while (g != NULL) {
        NO_SUBGRUPO *s = g->primeiro_subgrupo;
        while (s != NULL) {
            NO_ITEM *i = s->primeiro_item;
            while (i != NULL) {
                NO_ITEM *prox_i = i->prox;
                if (i->item != NULL && liberar_item != NULL) {
                    liberar_item(&(i->item));
                }
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

    const char *g_chave = (chave_grupo != NULL) ? chave_grupo : "";
    const char *s_chave = (chave_subgrupo != NULL) ? chave_subgrupo : "";

    NO_GRUPO *g_ant = NULL;
    NO_GRUPO *g = l->primeiro_grupo;
    while (g != NULL && g->chave != NULL && strcmp(g->chave, g_chave) != 0) {
        g_ant = g;
        g = g->prox;
    }

    if (g == NULL) {
        g = (NO_GRUPO *) malloc(sizeof(NO_GRUPO));
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

    NO_SUBGRUPO *s_ant = NULL;
    NO_SUBGRUPO *s = g->primeiro_subgrupo;
    while (s != NULL && s->chave != NULL && strcmp(s->chave, s_chave) != 0) {
        s_ant = s;
        s = s->prox;
    }

    if (s == NULL) {
        s = (NO_SUBGRUPO *) malloc(sizeof(NO_SUBGRUPO));
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

    NO_ITEM *novo_i = (NO_ITEM *) malloc(sizeof(NO_ITEM));
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

void lista_gen_percorrer(LISTA_GEN *l, void (*processar)(void *item, void *contexto), void *contexto) {
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