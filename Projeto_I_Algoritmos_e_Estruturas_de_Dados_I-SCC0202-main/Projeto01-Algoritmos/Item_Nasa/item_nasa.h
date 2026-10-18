#include <stdbool.h>
#include <stdio.h>

#ifndef ITEM_NASA_H
#define ITEM_NASA_H

typedef struct item ITEM_NASA;


ITEM_NASA *item_nasa_criar(void);
void item_nasa_apagar(ITEM_NASA **item);
void item_nasa_imprimir_csv(ITEM_NASA *item, FILE *saida);  
int item_nasa_comparar(ITEM_NASA *item_A, ITEM_NASA *item_B, char *campo);

void extrair_valor_campo_item_nasa(ITEM_NASA *item, const char *nome_campo, char *dest, int tamanho_max);


// (GET)
const char* item_get_designation(ITEM_NASA *item);
const char* item_get_fullname(ITEM_NASA *item);
const char* item_get_close_approach_date(ITEM_NASA *item);
int item_get_year(ITEM_NASA *item);
int item_get_month(ITEM_NASA *item);
double item_get_distance_au(ITEM_NASA *item);
double item_get_velocity_km_s(ITEM_NASA *item);
double item_get_absolute_magnitude(ITEM_NASA *item);
long item_get_days_until_approach(ITEM_NASA *item);
bool item_get_is_past_event(ITEM_NASA *item);
bool item_get_is_future_event(ITEM_NASA *item);
double item_get_risk_score(ITEM_NASA *item);
int item_get_panic_level(ITEM_NASA *item);
const char* item_get_threat_category(ITEM_NASA *item);
const char* item_get_panic_verdict(ITEM_NASA *item);
bool item_get_on_sentry_list(ITEM_NASA *item);
double item_get_sentry_impact_prob(ITEM_NASA *item);
double item_get_sentry_torino_scale(ITEM_NASA *item);
double item_get_sentry_palermo_scale(ITEM_NASA *item);
double item_get_sentry_diameter_km(ITEM_NASA *item);
double item_get_calculated_similarity(ITEM_NASA *item);

// (Set) 
void item_set_designation(ITEM_NASA *item, const char *val);
void item_set_fullname(ITEM_NASA *item, const char *val);
void item_set_close_approach_date(ITEM_NASA *item, const char *val);
void item_set_year(ITEM_NASA *item, int val);
void item_set_month(ITEM_NASA *item, int val);
void item_set_distance_au(ITEM_NASA *item, double val);
void item_set_velocity_km_s(ITEM_NASA *item, double val);
void item_set_absolute_magnitude(ITEM_NASA *item, double val);
void item_set_days_until_approach(ITEM_NASA *item, long val);
void item_set_is_past_event(ITEM_NASA *item, bool val);
void item_set_is_future_event(ITEM_NASA *item, bool val);
void item_set_risk_score(ITEM_NASA *item, double val);
void item_set_panic_level(ITEM_NASA *item, int val);
void item_set_threat_category(ITEM_NASA *item, const char *val);
void item_set_panic_verdict(ITEM_NASA *item, const char *val);
void item_set_on_sentry_list(ITEM_NASA *item, bool val);
void item_set_sentry_impact_prob(ITEM_NASA *item, double val);
void item_set_sentry_torino_scale(ITEM_NASA *item, double val);
void item_set_sentry_palermo_scale(ITEM_NASA *item, double val);
void item_set_sentry_diameter_km(ITEM_NASA *item, double val);
void item_set_calculated_similarity(ITEM_NASA *item, double val);

#endif