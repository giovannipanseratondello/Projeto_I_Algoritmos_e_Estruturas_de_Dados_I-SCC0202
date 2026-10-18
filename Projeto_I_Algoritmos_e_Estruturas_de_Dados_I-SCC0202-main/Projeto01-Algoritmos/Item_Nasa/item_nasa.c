#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "item_nasa.h"

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
    int panic_level;                 // Nível de alerta/pânico
    char threat_category[64];        // Categoria de ameaça (ex: "MONITOR", "CRITICAL")
    char panic_verdict[256];         // Descrição do veredito

    // Dados opcionais do programa Sentry
    bool on_sentry_list;             // Está na lista de monitoramento?
    double sentry_impact_prob;       // Probabilidade de impacto
    double sentry_torino_scale;      // Escala Torino
    double sentry_palermo_scale;     // Escala Palermo
    double sentry_diameter_km;       // Diâmetro estimado (km)

    // Campo auxiliar para as buscas
    double calculated_similarity;    // Pontuação final calculada pelas consultas (Query)
};

ITEM_NASA *item_nasa_criar(void) {
    ITEM_NASA *item = (ITEM_NASA*) malloc(sizeof(ITEM_NASA));
    if (item == NULL) {
        return NULL;
    }

    // Inicializa strings vazias
    item->designation[0] = '\0';
    item->fullname[0] = '\0';
    item->close_approach_date[0] = '\0';
    item->threat_category[0] = '\0';
    item->panic_verdict[0] = '\0';

    // Inicializa valores numéricos e booleanos
    item->year = 0;
    item->month = 0;
    item->distance_au = 0.0;
    item->velocity_km_s = 0.0;
    item->absolute_magnitude = 0.0;
    item->days_until_approach = 0;

    item->is_past_event = false;
    item->is_future_event = false;

    item->risk_score = 0.0;
    item->panic_level = 0;

    item->on_sentry_list = false;
    item->sentry_impact_prob = 0.0;
    item->sentry_torino_scale = 0.0;
    item->sentry_palermo_scale = 0.0;
    item->sentry_diameter_km = 0.0;

    item->calculated_similarity = 0.0;

    return item;
}

void item_nasa_apagar(ITEM_NASA **item){
    if ( item == NULL || *item == NULL ){
        return;
    }
    free(*item);
    *item = NULL;
}

void item_nasa_imprimir_csv(ITEM_NASA *item, FILE *saida) {
    if (item == NULL || saida == NULL) return;
    fprintf(saida, "%s,%s,%s,%d,%d,%.6f,%.6f,%.6f,%ld,%s,%s,%.6f,%d,%s,%s,%s,%.6f,%.6f,%.6f,%.6f\n",
        item->designation,
        item->fullname,
        item->close_approach_date,
        item->year,
        item->month,
        item->distance_au,
        item->velocity_km_s,
        item->absolute_magnitude,
        item->days_until_approach,
        item->is_past_event ? "true" : "false",
        item->is_future_event ? "true" : "false",
        item->risk_score,
        item->panic_level,
        item->threat_category,
        item->panic_verdict,
        item->on_sentry_list ? "true" : "false",
        item->sentry_impact_prob,
        item->sentry_torino_scale,
        item->sentry_palermo_scale,
        item->sentry_diameter_km
    );
}

int item_nasa_comparar(ITEM_NASA *item_A, ITEM_NASA *item_B, char *campo){
    if (item_A == NULL || item_B == NULL || campo == NULL){
        return 0;
    }
    if (strcmp(campo, "calculated_similarity") == 0 || strcmp(campo, "SIM") == 0) {
        if (item_A->calculated_similarity < item_B->calculated_similarity) return -1;
        if (item_A->calculated_similarity > item_B->calculated_similarity) return 1;
        return 0;
    }
    if (strcmp(campo, "risk_score") == 0) {
        if (item_A->risk_score < item_B->risk_score) return -1;
        if (item_A->risk_score > item_B->risk_score) return 1;
        return 0;
    }
    if (strcmp(campo, "distance_au") == 0) {
        if (item_A->distance_au < item_B->distance_au) return -1;
        if (item_A->distance_au > item_B->distance_au) return 1;
        return 0;
    }
    if (strcmp(campo, "velocity_km_s") == 0) {
        if (item_A->velocity_km_s < item_B->velocity_km_s) return -1;
        if (item_A->velocity_km_s > item_B->velocity_km_s) return 1;
        return 0;
    }
    if (strcmp(campo, "absolute_magnitude") == 0) {
        if (item_A->absolute_magnitude < item_B->absolute_magnitude) return -1;
        if (item_A->absolute_magnitude > item_B->absolute_magnitude) return 1;
        return 0;
    }

    // --- Comparação por Campos Inteiros e Long ---
    if (strcmp(campo, "year") == 0) {
        return item_A->year - item_B->year;
    }
    if (strcmp(campo, "month") == 0) {
        return item_A->month - item_B->month;
    }
    if (strcmp(campo, "panic_level") == 0) {
        return item_A->panic_level - item_B->panic_level;
    }
    if (strcmp(campo, "days_until_approach") == 0) {
        if (item_A->days_until_approach < item_B->days_until_approach) return -1;
        if (item_A->days_until_approach > item_B->days_until_approach) return 1;
        return 0;
    }

    // --- Comparação por Campos de Texto (String) ---
    if (strcmp(campo, "designation") == 0 || strcmp(campo, "asteroid_designation") == 0) {
        return strcmp(item_A->designation, item_B->designation);
    }
    if (strcmp(campo, "fullname") == 0 || strcmp(campo, "asteroid_fullname") == 0) {
        return strcmp(item_A->fullname, item_B->fullname);
    }
    if (strcmp(campo, "close_approach_date") == 0) {
        return strcmp(item_A->close_approach_date, item_B->close_approach_date);
    }
    if (strcmp(campo, "threat_category") == 0) {
        return strcmp(item_A->threat_category, item_B->threat_category);
    }
    if (strcmp(campo, "panic_verdict") == 0) {
        return strcmp(item_A->panic_verdict, item_B->panic_verdict);
    }

    return 0;
}

// --- GETTERS ---
const char* item_get_designation(ITEM_NASA *item){ 
    return item ? item->designation : NULL; 
}

const char* item_get_fullname(ITEM_NASA *item){
     return item ? item->fullname : NULL; 
}

const char* item_get_close_approach_date(ITEM_NASA *item) {
     return item ? item->close_approach_date : NULL; 
}

int item_get_year(ITEM_NASA *item) {
     return item ? item->year : 0; 
}

int item_get_month(ITEM_NASA *item) {
     return item ? item->month : 0; 
}

double item_get_distance_au(ITEM_NASA *item) { 
    return item ? item->distance_au : 0.0; 
}

double item_get_velocity_km_s(ITEM_NASA *item) { 
    return item ? item->velocity_km_s : 0.0; 
}

double item_get_absolute_magnitude(ITEM_NASA *item) { 
    return item ? item->absolute_magnitude : 0.0; 
}

long item_get_days_until_approach(ITEM_NASA *item) { 
    return item ? item->days_until_approach : 0; 
}

bool item_get_is_past_event(ITEM_NASA *item) { 
    return item ? item->is_past_event : false; 
}

bool item_get_is_future_event(ITEM_NASA *item) { 
    return item ? item->is_future_event : false; 
}

double item_get_risk_score(ITEM_NASA *item) {
    return item ? item->risk_score : 0.0; 
}

int item_get_panic_level(ITEM_NASA *item) {
     return item ? item->panic_level : 0; 
}

const char* item_get_threat_category(ITEM_NASA *item) {
     return item ? item->threat_category : NULL; 
}

const char* item_get_panic_verdict(ITEM_NASA *item) {
     return item ? item->panic_verdict : NULL; 
}

bool item_get_on_sentry_list(ITEM_NASA *item) {
     return item ? item->on_sentry_list : false; 
}

double item_get_sentry_impact_prob(ITEM_NASA *item) { 
    return item ? item->sentry_impact_prob : 0.0; 
}

double item_get_sentry_torino_scale(ITEM_NASA *item) {
     return item ? item->sentry_torino_scale : 0.0; 
}

double item_get_sentry_palermo_scale(ITEM_NASA *item) { 
    return item ? item->sentry_palermo_scale : 0.0; 
}

double item_get_sentry_diameter_km(ITEM_NASA *item) { 
    return item ? item->sentry_diameter_km : 0.0; 
}

double item_get_calculated_similarity(ITEM_NASA *item) { 
    return item ? item->calculated_similarity : 0.0; 
}

// --- SETTERS ---
void item_set_designation(ITEM_NASA *item, const char *val) {
    if (item && val) {
        strncpy(item->designation, val, sizeof(item->designation) - 1);
        item->designation[sizeof(item->designation) - 1] = '\0';
    }
}

void item_set_fullname(ITEM_NASA *item, const char *val) {
    if (item && val) {
        strncpy(item->fullname, val, sizeof(item->fullname) - 1);
        item->fullname[sizeof(item->fullname) - 1] = '\0';
    }
}

void item_set_close_approach_date(ITEM_NASA *item, const char *val) {
    if (item && val) {
        strncpy(item->close_approach_date, val, sizeof(item->close_approach_date) - 1);
        item->close_approach_date[sizeof(item->close_approach_date) - 1] = '\0';
    }
}

void item_set_year(ITEM_NASA *item, int val) { 
    if (item) item->year = val; 
}

void item_set_month(ITEM_NASA *item, int val) { 
    if (item) item->month = val; 
}

void item_set_distance_au(ITEM_NASA *item, double val) { 
    if (item) item->distance_au = val; 
}

void item_set_velocity_km_s(ITEM_NASA *item, double val) { 
    if (item) item->velocity_km_s = val; 
}

void item_set_absolute_magnitude(ITEM_NASA *item, double val) { 
    if (item) item->absolute_magnitude = val; 
}

void item_set_days_until_approach(ITEM_NASA *item, long val) { 
    if (item) item->days_until_approach = val; 
}

void item_set_is_past_event(ITEM_NASA *item, bool val) { 
    if (item) item->is_past_event = val; 
}

void item_set_is_future_event(ITEM_NASA *item, bool val) { 
    if (item) item->is_future_event = val; 
}

void item_set_risk_score(ITEM_NASA *item, double val) { 
    if (item) item->risk_score = val; 
}

void item_set_panic_level(ITEM_NASA *item, int val) { 
    if (item) item->panic_level = val;
}

void item_set_threat_category(ITEM_NASA *item, const char *val) {
    if (item && val) {
        strncpy(item->threat_category, val, sizeof(item->threat_category) - 1);
        item->threat_category[sizeof(item->threat_category) - 1] = '\0';
    }
}

void item_set_panic_verdict(ITEM_NASA *item, const char *val) {
    if (item && val) {
        strncpy(item->panic_verdict, val, sizeof(item->panic_verdict) - 1);
        item->panic_verdict[sizeof(item->panic_verdict) - 1] = '\0';
    }
}

void item_set_on_sentry_list(ITEM_NASA *item, bool val) {
     if (item) item->on_sentry_list = val; 
}

void item_set_sentry_impact_prob(ITEM_NASA *item, double val) {
     if (item) item->sentry_impact_prob = val; 
}

void item_set_sentry_torino_scale(ITEM_NASA *item, double val) { 
    if (item) item->sentry_torino_scale = val; 
}

void item_set_sentry_palermo_scale(ITEM_NASA *item, double val) { 
    if (item) item->sentry_palermo_scale = val; 
}

void item_set_sentry_diameter_km(ITEM_NASA *item, double val) { 
    if (item) item->sentry_diameter_km = val; 
}

void item_set_calculated_similarity(ITEM_NASA *item, double val) { 
    if (item) item->calculated_similarity = val; 
}

// Extrai dinamicamente o valor de qualquer campo do item para string (usado em agrupamentos/cruzamentos)
void extrair_valor_campo_item_nasa(ITEM_NASA *item, const char *nome_campo, char *dest, int tamanho_max) {
    if (item == NULL || nome_campo == NULL || dest == NULL) return;
    dest[0] = '\0';

    if (strcmp(nome_campo, "asteroid_designation") == 0 || strcmp(nome_campo, "designation") == 0) {
        const char *val = item_get_designation(item);
        if (val) { strncpy(dest, val, tamanho_max - 1); dest[tamanho_max - 1] = '\0'; }
    } 
    else if (strcmp(nome_campo, "asteroid_fullname") == 0 || strcmp(nome_campo, "fullname") == 0) {
        const char *val = item_get_fullname(item);
        if (val) { strncpy(dest, val, tamanho_max - 1); dest[tamanho_max - 1] = '\0'; }
    } 
    else if (strcmp(nome_campo, "close_approach_date") == 0) {
        const char *val = item_get_close_approach_date(item);
        if (val) { strncpy(dest, val, tamanho_max - 1); dest[tamanho_max - 1] = '\0'; }
    } 
    else if (strcmp(nome_campo, "threat_category") == 0) {
        const char *val = item_get_threat_category(item);
        if (val) { strncpy(dest, val, tamanho_max - 1); dest[tamanho_max - 1] = '\0'; }
    } 
    else if (strcmp(nome_campo, "panic_verdict") == 0) {
        const char *val = item_get_panic_verdict(item);
        if (val) { strncpy(dest, val, tamanho_max - 1); dest[tamanho_max - 1] = '\0'; }
    }
    else if (strcmp(nome_campo, "year") == 0) {
        snprintf(dest, tamanho_max, "%d", item_get_year(item));
    } 
    else if (strcmp(nome_campo, "month") == 0) {
        snprintf(dest, tamanho_max, "%d", item_get_month(item));
    } 
    else if (strcmp(nome_campo, "panic_level") == 0) {
        snprintf(dest, tamanho_max, "%d", item_get_panic_level(item));
    }
    else if (strcmp(nome_campo, "distance_au") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_distance_au(item));
    } 
    else if (strcmp(nome_campo, "velocity_km_s") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_velocity_km_s(item));
    } 
    else if (strcmp(nome_campo, "absolute_magnitude") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_absolute_magnitude(item));
    } 
    else if (strcmp(nome_campo, "risk_score") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_risk_score(item));
    }
    else if (strcmp(nome_campo, "days_until_approach") == 0) {
        snprintf(dest, tamanho_max, "%ld", item_get_days_until_approach(item));
    }
    else if (strcmp(nome_campo, "is_past_event") == 0) {
        snprintf(dest, tamanho_max, "%s", item_get_is_past_event(item) ? "true" : "false");
    }
    else if (strcmp(nome_campo, "is_future_event") == 0) {
        snprintf(dest, tamanho_max, "%s", item_get_is_future_event(item) ? "true" : "false");
    }
    else if (strcmp(nome_campo, "on_sentry_list") == 0) {
        snprintf(dest, tamanho_max, "%s", item_get_on_sentry_list(item) ? "true" : "false");
    }
    else if (strcmp(nome_campo, "sentry_impact_prob") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_sentry_impact_prob(item));
    }
    else if (strcmp(nome_campo, "sentry_torino_scale") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_sentry_torino_scale(item));
    }
    else if (strcmp(nome_campo, "sentry_palermo_scale") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_sentry_palermo_scale(item));
    }
    else if (strcmp(nome_campo, "sentry_diameter_km") == 0) {
        snprintf(dest, tamanho_max, "%f", item_get_sentry_diameter_km(item));
    }
}