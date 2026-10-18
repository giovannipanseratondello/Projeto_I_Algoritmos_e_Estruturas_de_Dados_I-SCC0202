#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "reader_nasa.h"

bool eh_espaco(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

void parse_string(char *str_origem, char *str_destino, int tamanho){
    if(str_destino == NULL || tamanho == 0){
        return;
    }
    
    if(str_origem == NULL || str_origem[0] == '\0'){
        return;
    }

    // ignora espacos ou aspas no inicio
    while (*str_origem != '\0' && (eh_espaco(*str_origem) || *str_origem == '"')) {
        str_origem++;
    }

    int i = 0;
    while(*str_origem != '\0' && i < tamanho){
        if(*str_origem == '"' || *str_origem == '\r' || *str_origem == '\n'){
            break;
        }
        str_destino[i] = *str_origem;
        i++;
        str_origem++;
    }
    str_destino[i] = '\0';

    while(i > 0 && eh_espaco(str_destino[i-1])){
        str_destino[i-1] = '\0';
        i--;
    }
}

int parse_int(char *str_origem){
    if (str_origem == NULL || str_origem[0] == '\0')
        return 0;

    return atoi(str_origem);
}

long parse_long(char *str_origem){
    if(str_origem == NULL || str_origem[0] == '\0'){
        return 0;
    }
    return atol(str_origem);
}

double parse_double(char *str_origem){
    if(str_origem == NULL || str_origem[0] == '\0'){
        return 0;
    }
    return atof(str_origem);
}

bool parse_bool(char *str_origem){
    if(str_origem == NULL || str_origem[0] == '\0'){
        return 0;
    }

    if(strcmp(str_origem, "True") == 0){
        return 1;
    }
    return 0;
}

char* extrair_campo_csv(char **linha_ptr){
    if(linha_ptr == NULL || *linha_ptr == NULL || **linha_ptr == '\0')
        return NULL;

    char *inicio = *linha_ptr;
    char *p = inicio;
    bool aspas = false;

    while(*p != '\0') {
        if(*p == '"') 
            aspas = !aspas;
        else if(*p == ',' && !aspas){
            *p = '\0';
            *linha_ptr = p + 1;
            return inicio;
        }
        p++;
    }

    *linha_ptr = p;
    return inicio;
}

void reader_nasa_pular_cabecalho(FILE *fp){
    if (fp == NULL)
        return;
    char cabecalho[1024];
    if (fgets(cabecalho, sizeof(cabecalho), fp) == NULL) {
        return;
    }
}

ITEM_NASA* reader_nasa_ler_item(FILE *fp){
    if (fp == NULL)
        return NULL;

    char linha[1024];
    if (fgets(linha, sizeof(linha), fp) == NULL){
        return NULL;
    }

    if(linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r'){
        return reader_nasa_ler_item(fp);
    }

    ITEM_NASA *item = item_nasa_criar();
    if (item == NULL){
        return NULL;
    }

    char *ptr = linha; // ponteiro que aponta pro primeiro caracter da linha
    char designation[64];            // Designação oficial (ex: "2020 BX12")
    char fullname[128];              // Nome completo
    char close_approach_date[32]; 
    char threat_category[64];        // Categoria de ameaça (ex: "MONITOR", "CRITICAL")
    char panic_verdict[256]; 

    // 1. Strings
    parse_string(extrair_campo_csv(&ptr), designation, sizeof(designation));
    item_set_designation(item, designation);
    parse_string(extrair_campo_csv(&ptr), fullname, sizeof(fullname));
    item_set_fullname(item, fullname);
    parse_string(extrair_campo_csv(&ptr), close_approach_date, sizeof(close_approach_date));
    item_set_close_approach_date(item, close_approach_date);

    // 2. Inteiros (int / long)
    item_set_year(item, parse_int(extrair_campo_csv(&ptr)));
    item_set_month(item, parse_int(extrair_campo_csv(&ptr)));

    // 3. Flutuantes (double)
    item_set_distance_au(item, parse_double(extrair_campo_csv(&ptr)));

    item_set_velocity_km_s(item, parse_double(extrair_campo_csv(&ptr)));
    item_set_absolute_magnitude(item, parse_double(extrair_campo_csv(&ptr)));
    
    // 4. Inteiro Longo (long)
    item_set_days_until_approach(item, parse_long(extrair_campo_csv(&ptr)));

    // 5. Booleanos (bool)
    item_set_is_past_event(item, parse_bool(extrair_campo_csv(&ptr)));
    item_set_is_future_event(item, parse_bool(extrair_campo_csv(&ptr)));

    // 6. Métricas Adicionais (double / int / string)
    item_set_risk_score(item, parse_double(extrair_campo_csv(&ptr)));
    item_set_panic_level(item, parse_int(extrair_campo_csv(&ptr)));

    parse_string(extrair_campo_csv(&ptr), threat_category, sizeof(threat_category));
    item_set_threat_category(item, threat_category);
    parse_string(extrair_campo_csv(&ptr), panic_verdict, sizeof(panic_verdict));
    item_set_panic_verdict(item, panic_verdict);

    // 7. Dados Sentry (bool / double)
    item_set_on_sentry_list(item, parse_bool(extrair_campo_csv(&ptr)));
    item_set_sentry_impact_prob(item, parse_double(extrair_campo_csv(&ptr)));
    item_set_sentry_torino_scale(item, parse_double(extrair_campo_csv(&ptr)));  
    item_set_sentry_palermo_scale(item, parse_double(extrair_campo_csv(&ptr)));
    item_set_sentry_diameter_km(item, parse_double(extrair_campo_csv(&ptr)));

    return item;
}