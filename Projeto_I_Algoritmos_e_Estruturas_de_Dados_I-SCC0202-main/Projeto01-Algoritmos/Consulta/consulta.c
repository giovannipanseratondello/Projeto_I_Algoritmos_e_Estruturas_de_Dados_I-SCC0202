#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "consulta.h"

struct filtro {
    char campo[64];
    char operador[4]; 
    char valor_str[128]; 
    double valor_num; 
};

struct similaridade {
    char campo[64];
    double valor_alvo;
    double tolerancia;
    double peso;
};

struct query {
   FILTRO *filtros[32];
   int qnt_filtos;
   
   SIMILARIDADE *similaridades[32];
   int qtd_sim;

   double accept;

   char order_campo[64];
   char order_tipo[8];
    
   char group_campo1[64];
   char group_campo2[64];
   
   char cross_campo1[64];
   char cross_campo2[64];
};

// Função auxiliar para remover espaços e quebras de linha extras das pontas
char* trim_string(char *str) {
    if (str == NULL) return NULL;
    while (*str != '\0' && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n')) {
        str++;
    }
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }
    return str;
}

QUERY *query_carregar(char *arquivo) {
    if (arquivo == NULL) return NULL;

    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) return NULL;

    QUERY *q = (QUERY*) calloc(1, sizeof(QUERY));
    if (q == NULL) {
        fclose(fp);
        return NULL;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (linha[0] == '\0' || linha[0] == '#') continue;

        char *comando = trim_string(strtok(linha, "|"));
        if (comando == NULL) continue;

        if (strcmp(comando, "FILTER") == 0) {
            char *campo = trim_string(strtok(NULL, "|"));
            char *operador = trim_string(strtok(NULL, "|"));
            char *valor = trim_string(strtok(NULL, "|"));

            if (campo && operador && valor && q->qnt_filtos < 32) {
                FILTRO *f = (FILTRO*)malloc(sizeof(FILTRO));
                if (f == NULL) {
                    fclose(fp);
                    query_destruir(&q);
                    return NULL;
                }

                strncpy(f->campo, campo, sizeof(f->campo) - 1);
                f->campo[sizeof(f->campo) - 1] = '\0';

                strncpy(f->operador, operador, sizeof(f->operador) - 1);
                f->operador[sizeof(f->operador) - 1] = '\0';

                strncpy(f->valor_str, valor, sizeof(f->valor_str) - 1);
                f->valor_str[sizeof(f->valor_str) - 1] = '\0';

                f->valor_num = atof(valor);

                q->filtros[q->qnt_filtos++] = f;
            }

        } else if (strcmp(comando, "SIM") == 0) {
            char *campo = trim_string(strtok(NULL, "|"));
            char *alvo = trim_string(strtok(NULL, "|"));
            char *tol = trim_string(strtok(NULL, "|"));
            char *peso = trim_string(strtok(NULL, "|"));

            if (campo && alvo && tol && peso && q->qtd_sim < 32) {
                SIMILARIDADE *s = (SIMILARIDADE *)malloc(sizeof(SIMILARIDADE));
                if (s == NULL) {
                    fclose(fp);
                    query_destruir(&q);
                    return NULL;
                }

                strncpy(s->campo, campo, sizeof(s->campo) - 1);
                s->campo[sizeof(s->campo) - 1] = '\0';

                s->valor_alvo = atof(alvo);
                s->tolerancia = atof(tol);
                s->peso = atof(peso);

                q->similaridades[q->qtd_sim++] = s;
            }

        } else if (strcmp(comando, "ACCEPT") == 0) {
            char *val = trim_string(strtok(NULL, "|"));
            if (val) q->accept = atof(val);

        } else if (strcmp(comando, "ORDER") == 0) {
            char *campo = trim_string(strtok(NULL, "|"));
            char *tipo = trim_string(strtok(NULL, "|"));

            if (campo) {
                strncpy(q->order_campo, campo, sizeof(q->order_campo) - 1);
                q->order_campo[sizeof(q->order_campo) - 1] = '\0';
            }
            if (tipo) {
                strncpy(q->order_tipo, tipo, sizeof(q->order_tipo) - 1);
                q->order_tipo[sizeof(q->order_tipo) - 1] = '\0';
            }

        } else if (strcmp(comando, "GROUP") == 0) {
            char *g1 = trim_string(strtok(NULL, "|"));
            char *g2 = trim_string(strtok(NULL, "|"));

            if (g1) {
                strncpy(q->group_campo1, g1, sizeof(q->group_campo1) - 1);
                q->group_campo1[sizeof(q->group_campo1) - 1] = '\0';
            }
            if (g2) {
                strncpy(q->group_campo2, g2, sizeof(q->group_campo2) - 1);
                q->group_campo2[sizeof(q->group_campo2) - 1] = '\0';
            }

        } else if (strcmp(comando, "CROSS") == 0) {
            char *c1 = trim_string(strtok(NULL, "|"));
            char *c2 = trim_string(strtok(NULL, "|"));

            if (c1) {
                strncpy(q->cross_campo1, c1, sizeof(q->cross_campo1) - 1);
                q->cross_campo1[sizeof(q->cross_campo1) - 1] = '\0';
            }
            if (c2) {
                strncpy(q->cross_campo2, c2, sizeof(q->cross_campo2) - 1);
                q->cross_campo2[sizeof(q->cross_campo2) - 1] = '\0';
            }
        }
    }

    fclose(fp);
    return q;
}

void query_destruir(QUERY **q) {
    if (q == NULL || *q == NULL) return;

    for (int i = 0; i < (*q)->qnt_filtos; i++) {
        if ((*q)->filtros[i] != NULL) {
            free((*q)->filtros[i]);
        }
    }

    for (int i = 0; i < (*q)->qtd_sim; i++) {
        if ((*q)->similaridades[i] != NULL) {
            free((*q)->similaridades[i]);
        }
    }

    free(*q);
    *q = NULL;
}

// Mapeamento completo de campos numéricos
double obter_valor_numerico_item(void *item, const char *campo) {
    if (strcmp(campo, "year") == 0) return (double) item_get_year((ITEM_NASA *)item);
    if (strcmp(campo, "month") == 0) return (double) item_get_month((ITEM_NASA *)item);
    if (strcmp(campo, "distance_au") == 0) return item_get_distance_au((ITEM_NASA *)item);
    if (strcmp(campo, "velocity_km_s") == 0) return item_get_velocity_km_s((ITEM_NASA *)item);
    if (strcmp(campo, "absolute_magnitude") == 0) return item_get_absolute_magnitude((ITEM_NASA *)item);
    if (strcmp(campo, "days_until_approach") == 0) return (double) item_get_days_until_approach((ITEM_NASA *)item);
    if (strcmp(campo, "risk_score") == 0) return item_get_risk_score((ITEM_NASA *)item);
    if (strcmp(campo, "panic_level") == 0) return (double) item_get_panic_level((ITEM_NASA *)item);
    if (strcmp(campo, "sentry_impact_prob") == 0) return item_get_sentry_impact_prob((ITEM_NASA *)item);
    if (strcmp(campo, "sentry_torino_scale") == 0) return item_get_sentry_torino_scale((ITEM_NASA *)item);
    if (strcmp(campo, "sentry_palermo_scale") == 0) return item_get_sentry_palermo_scale((ITEM_NASA *)item);
    if (strcmp(campo, "sentry_diameter_km") == 0) return item_get_sentry_diameter_km((ITEM_NASA *)item);
    return 0.0;
}

// Mapeamento de campos textuais
const char* obter_valor_texto_item(void *item, const char *campo) {
    if (strcmp(campo, "asteroid_designation") == 0 || strcmp(campo, "designation") == 0) return item_get_designation((ITEM_NASA *)item);
    if (strcmp(campo, "asteroid_fullname") == 0 || strcmp(campo, "fullname") == 0) return item_get_fullname((ITEM_NASA *)item);
    if (strcmp(campo, "close_approach_date") == 0) return item_get_close_approach_date((ITEM_NASA *)item);
    if (strcmp(campo, "threat_category") == 0) return item_get_threat_category((ITEM_NASA *)item);
    if (strcmp(campo, "panic_verdict") == 0) return item_get_panic_verdict((ITEM_NASA *)item);
    return NULL;
}

// Mapeamento de campos booleanos
bool obter_valor_booleano_item(void *item, const char *campo) {
    if (strcmp(campo, "is_past_event") == 0) return item_get_is_past_event((ITEM_NASA *)item);
    if (strcmp(campo, "is_future_event") == 0) return item_get_is_future_event((ITEM_NASA *)item);
    if (strcmp(campo, "on_sentry_list") == 0) return item_get_on_sentry_list((ITEM_NASA *)item);
    return false;
}

bool testar_filtro(FILTRO *f, void *item) {
    if (f == NULL || item == NULL) return false;

    // 1. Tratamento para campos Booleanos
    if (strcmp(f->campo, "is_past_event") == 0 || 
        strcmp(f->campo, "is_future_event") == 0 || 
        strcmp(f->campo, "on_sentry_list") == 0) {
        
        bool val_bool = obter_valor_booleano_item(item, f->campo);
        bool alvo_bool = (strcasecmp(f->valor_str, "true") == 0 || 
                          strcmp(f->valor_str, "1") == 0 || 
                          strcasecmp(f->valor_str, "T") == 0);

        if (strcmp(f->operador, "==") == 0) return val_bool == alvo_bool;
        if (strcmp(f->operador, "!=") == 0) return val_bool != alvo_bool;
        return true;
    }

    // 2. Tratamento para campos de Texto (String)
    const char *txt_val = obter_valor_texto_item(item, f->campo);
    if (txt_val != NULL) {
        if (strcmp(f->operador, "==") == 0) return strcasecmp(txt_val, f->valor_str) == 0;
        if (strcmp(f->operador, "!=") == 0) return strcasecmp(txt_val, f->valor_str) != 0;
        return true;
    }

    // 3. Tratamento para campos Numéricos
    double val_item = obter_valor_numerico_item(item, f->campo);
    
    if (strcmp(f->operador, "==") == 0) return val_item == f->valor_num;
    if (strcmp(f->operador, "!=") == 0) return val_item != f->valor_num;
    if (strcmp(f->operador, ">") == 0)  return val_item > f->valor_num;
    if (strcmp(f->operador, "<") == 0)  return val_item < f->valor_num;
    if (strcmp(f->operador, ">=") == 0) return val_item >= f->valor_num;
    if (strcmp(f->operador, "<=") == 0) return val_item <= f->valor_num;

    return true;
}

bool query_avaliar_item(QUERY *q, void *item) {
    if (q == NULL || item == NULL) return false;

    // 1. Aplica Filtros
    for (int i = 0; i < q->qnt_filtos; i++) {
        if (!testar_filtro(q->filtros[i], item)) {
            return false;
        }
    }

    // 2. Se não houver regras SIM, aprova
    if (q->qtd_sim == 0) {
        item_set_calculated_similarity((ITEM_NASA *)item, 1.0);
        return true;
    }

    // 3. Calcula a Similaridade Ponderada
    double soma_pesos = 0.0;
    double soma_score = 0.0;

    for (int i = 0; i < q->qtd_sim; i++) {
        SIMILARIDADE *s = q->similaridades[i];
        double val_x = obter_valor_numerico_item(item, s->campo);

        double sim_indiv = 0.0;
        if (s->tolerancia > 0) {
            sim_indiv = 1.0 - (fabs(val_x - s->valor_alvo) / s->tolerancia);
            if (sim_indiv < 0.0) sim_indiv = 0.0;
        }

        soma_score += sim_indiv * s->peso;
        soma_pesos += s->peso;
    }

    double score_final = (soma_pesos > 0) ? (soma_score / soma_pesos) : 0.0;
    item_set_calculated_similarity((ITEM_NASA *)item, score_final);

    return score_final >= q->accept;
}

char* query_obter_order_campo(QUERY *q) {
    if (q == NULL) return (char *)"";
    return q->order_campo;
}

char* query_obter_group_campo1(QUERY *q) {
    if (q == NULL) return (char *)"";
    return q->group_campo1;
}

char* query_obter_group_campo2(QUERY *q) {
    if (q == NULL) return (char *)"";
    return q->group_campo2;
}

char* query_obter_cross_campo1(QUERY *q) {
    if (q == NULL) return (char *)"";
    return q->cross_campo1;
}

char* query_obter_cross_campo2(QUERY *q) {
    if (q == NULL) return (char *)"";
    return q->cross_campo2;
}