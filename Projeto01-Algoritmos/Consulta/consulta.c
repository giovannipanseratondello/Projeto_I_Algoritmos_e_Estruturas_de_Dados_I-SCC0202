#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "consulta.h"
#include "../Item/item.h"

struct item{
    // Identificação e texto
    char designation[64];            // Designação oficial (ex: "2020 BX12")
    char fullname[128];              // Nome completo
    char close_approach_date[32];    // Data de aproximação formatada em texto

    // Métricas e datas
    int year;                        // Ano do evento
    int month;                       // Mês do evento
    double distance_au;              // Distância em Unidades Astronômicas
    double velocity_km_s;            // Velocidade em km/s
    double absolute_magnitude;       // Magnitude absoluta (H)
    long days_until_approach;        // Contagem regressiva em dias

    // Flags de tempo
    bool is_past_event;              // Já passou?
    bool is_future_event;            // Ainda vai acontecer?

    // Avaliação de risco e pânico
    double risk_score;               // Nota de risco calculada
    int    panic_level;                 // Nível de alerta/pânico
    char   threat_category[64];        // Categoria de ameaça (ex: "MONITOR", "CRITICAL")
    char   panic_verdict[256];         // Descrição do veredito

    // Dados opcionais do programa Sentry
    bool   on_sentry_list;             // Está na lista de monitoramento?
    double sentry_impact_prob;       // Probabilidade de impacto
    double sentry_torino_scale;      // Escala Torino
    double sentry_palermo_scale;     // Escala Palermo
    double sentry_diameter_km;       // Diâmetro estimado (km)

    // Campo auxiliar para as buscas
    double calculated_similarity;    // Pontuação final calculada pelas consultas (Query)
};

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
    
   char group_campo[64];
   
   char cross_campo1[64];
   char cross_campo2[64];
};


QUERY *query_carregar(char *arquivo) {
    if (arquivo == NULL) return NULL;

    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) return NULL;

    QUERY *q = calloc(1, sizeof(QUERY));
    if (q == NULL) {
        fclose(fp);
        return NULL;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (linha[0] == '\0' || linha[0] == '#') continue;

        char *comando = strtok(linha, "|");
        if (comando == NULL) continue;

        if (strcmp(comando, "FILTER") == 0) {
            char *campo = strtok(NULL, "|");
            char *operador = strtok(NULL, "|");
            char *valor = strtok(NULL, "|");

            if (campo && operador && valor && q->qnt_filtos < 32) {
                FILTRO *f = malloc(sizeof(FILTRO));
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
            char *campo = strtok(NULL, "|");
            char *alvo = strtok(NULL, "|");
            char *tol = strtok(NULL, "|");
            char *peso = strtok(NULL, "|");

            if (campo && alvo && tol && peso && q->qtd_sim < 32) {
                SIMILARIDADE *s = malloc(sizeof(SIMILARIDADE));
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
            char *val = strtok(NULL, "|");
            if (val) q->accept = atof(val);

        } else if (strcmp(comando, "ORDER") == 0) {
            char *campo = strtok(NULL, "|");
            char *tipo = strtok(NULL, "|");

            if (campo) {
                strncpy(q->order_campo, campo, sizeof(q->order_campo) - 1);
                q->order_campo[sizeof(q->order_campo) - 1] = '\0';
            }
            if (tipo) {
                strncpy(q->order_tipo, tipo, sizeof(q->order_tipo) - 1);
                q->order_tipo[sizeof(q->order_tipo) - 1] = '\0';
            }

        } else if (strcmp(comando, "GROUP") == 0) {
            char *campo = strtok(NULL, "|");
            if (campo) {
                strncpy(q->group_campo, campo, sizeof(q->group_campo) - 1);
                q->group_campo[sizeof(q->group_campo) - 1] = '\0';
            }

        } else if (strcmp(comando, "CROSS") == 0) {
            char *c1 = strtok(NULL, "|");
            char *c2 = strtok(NULL, "|");

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

double obter_valor_numerico_item(ITEM *item, const char *campo) {
    if (strcmp(campo, "year") == 0) return (double) item->year;
    if (strcmp(campo, "month") == 0) return (double) item->month;
    if (strcmp(campo, "distance_au") == 0) return item->distance_au;
    if (strcmp(campo, "velocity_km_s") == 0) return item->velocity_km_s;
    if (strcmp(campo, "absolute_magnitude") == 0) return item->absolute_magnitude;
    if (strcmp(campo, "days_until_approach") == 0) return (double) item->days_until_approach;
    if (strcmp(campo, "risk_score") == 0) return item->risk_score;
    if (strcmp(campo, "panic_level") == 0) return (double) item->panic_level;
    return 0.0;
}

bool testar_filtro(FILTRO *f, ITEM *item) {
    if (strcmp(f->campo, "threat_category") == 0) {
        if (strcmp(f->operador, "==") == 0) return strcmp(item->threat_category, f->valor_str) == 0;
        if (strcmp(f->operador, "!=") == 0) return strcmp(item->threat_category, f->valor_str) != 0;
    }

    double val_item = obter_valor_numerico_item(item, f->campo);
    if (strcmp(f->operador, "==") == 0) return val_item == f->valor_num;
    if (strcmp(f->operador, "!=") == 0) return val_item != f->valor_num;
    if (strcmp(f->operador, ">") == 0)  return val_item > f->valor_num;
    if (strcmp(f->operador, "<") == 0)  return val_item < f->valor_num;
    if (strcmp(f->operador, ">=") == 0) return val_item >= f->valor_num;
    if (strcmp(f->operador, "<=") == 0) return val_item <= f->valor_num;

    return true;
}

bool query_avaliar_item(QUERY *q, ITEM *item) {
    if (q == NULL || item == NULL) return false;

    // 1. Aplica Filtros
    for (int i = 0; i < q->qnt_filtos; i++) {
        if (!testar_filtro(q->filtros[i], item)) {
            return false;
        }
    }

    // 2. Se não houver regras SIM, aprova
    if (q->qtd_sim == 0) {
        item->calculated_similarity = 1.0;
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
    item->calculated_similarity = score_final;

    return score_final >= q->accept;
}
