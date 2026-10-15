#include <stdbool.h>
#include <stdio.h>

#ifndef ITEM_H
#define ITEM_H

typedef struct item ITEM;


ITEM *item_criar(); 
void item_apagar(ITEM **item); 
void item_imprimir_csv(ITEM *item, FILE *saida);  
int item_comparar(ITEM *item_A, ITEM *item_B, char *campo);

void extrair_valor_campo_item(ITEM *item, const char *nome_campo, char *dest, int tamanho_max);


// (GET)
const char* item_get_designation(ITEM *item);
const char* item_get_fullname(ITEM *item);
const char* item_get_close_approach_date(ITEM *item);
int item_get_year(ITEM *item);
int item_get_month(ITEM *item);
double item_get_distance_au(ITEM *item);
double item_get_velocity_km_s(ITEM *item);
double item_get_absolute_magnitude(ITEM *item);
long item_get_days_until_approach(ITEM *item);
bool item_get_is_past_event(ITEM *item);
bool item_get_is_future_event(ITEM *item);
double item_get_risk_score(ITEM *item);
int item_get_panic_level(ITEM *item);
const char* item_get_threat_category(ITEM *item);
const char* item_get_panic_verdict(ITEM *item);
bool item_get_on_sentry_list(ITEM *item);
double item_get_sentry_impact_prob(ITEM *item);
double item_get_sentry_torino_scale(ITEM *item);
double item_get_sentry_palermo_scale(ITEM *item);
double item_get_sentry_diameter_km(ITEM *item);
double item_get_calculated_similarity(ITEM *item);

// (Set) 
void item_set_designation(ITEM *item, const char *val);
void item_set_fullname(ITEM *item, const char *val);
void item_set_close_approach_date(ITEM *item, const char *val);
void item_set_year(ITEM *item, int val);
void item_set_month(ITEM *item, int val);
void item_set_distance_au(ITEM *item, double val);
void item_set_velocity_km_s(ITEM *item, double val);
void item_set_absolute_magnitude(ITEM *item, double val);
void item_set_days_until_approach(ITEM *item, long val);
void item_set_is_past_event(ITEM *item, bool val);
void item_set_is_future_event(ITEM *item, bool val);
void item_set_risk_score(ITEM *item, double val);
void item_set_panic_level(ITEM *item, int val);
void item_set_threat_category(ITEM *item, const char *val);
void item_set_panic_verdict(ITEM *item, const char *val);
void item_set_on_sentry_list(ITEM *item, bool val);
void item_set_sentry_impact_prob(ITEM *item, double val);
void item_set_sentry_torino_scale(ITEM *item, double val);
void item_set_sentry_palermo_scale(ITEM *item, double val);
void item_set_sentry_diameter_km(ITEM *item, double val);
void item_set_calculated_similarity(ITEM *item, double val);

#endif