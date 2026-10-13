#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "reader.h"
#include "../Pilha/pilha.h" 
#include "../Fila/fila.h"
#include "../Lista_Seq/lista_seq.h"
#include "../Lista_Enc/lista_enc.h"
#include "../Lista_Cabeca/lista_cabeca.h"
#include "../Lista_Ord/lista_ord.h"
#include "../Lista_Gen/lista_gen.h"
#include "../Lista_Cruz/lista_cruz.h"

struct item{
    // identificacao e texto
    char designation[64];            // designacao oficial (ex: "2020 BX12")
    char fullname[128];              // nome completo
    char close_approach_date[32];    // data de aproximacao formatada em texto

    // Métricas e datas
    int year;                        // ano do evento
    int month;                       // mes do evento
    double distance_au;              // distancia em unidades astronômicas
    double velocity_km_s;            // velocidade em km/s
    double absolute_magnitude;       // magnitude absoluta (H)
    long days_until_approach;        // contagem regressiva em dias

    // Flags de tempo
    bool is_past_event;            
    bool is_future_event;

    // Avaliação de risco e pânico
    double risk_score;               // nota de risco calculada
    int panic_level;                 // nivel de alerta/panico
    char threat_category[64];        // categoria de ameaca
    char panic_verdict[256];         // descricao do veredito

    // Dados opcionais do programa Sentry
    bool on_sentry_list;             // esta na lista de monitoramento?
    double sentry_impact_prob;       // probabilidade de impacto
    double sentry_torino_scale;      // escala torino
    double sentry_palermo_scale;     // escala palermo
    double sentry_diameter_km;       // diametro estimado (km)

    // Campo auxiliar para as buscas
    double calculated_similarity;    // Pontuação final calculada pelas consultas (Query)
};

bool eh_espaco(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

void parse_string(char *str_origem,char *str_destino, int tamanho){
    if(str_destino == NULL || tamanho == 0){
        return;
    }
    
    if(str_origem == NULL || str_origem[0] == '\0'){
        return;
    }

    //ignora espacos ou aspas no inicio
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
    if (str_origem == NULL|| str_origem[0] == '\0')
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

    if( strcmp(str_origem,"True") == 0){
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

void reader_pular_cabecalho(FILE *fp){
    if (fp == NULL)
        return;
    char cabecalho[1024];
    if (fgets(cabecalho, sizeof(cabecalho), fp) == NULL) {
        return;
    }
}

ITEM* reader_ler_item(FILE *fp){
    if  (fp == NULL)
        return NULL;

    char linha[1024];
    if (fgets(linha, sizeof(linha), fp) == NULL){
        return NULL;
    }

    if( linha[0] == '\0' || linha[0] == '\n' || linha[0] == '\r'){
        return reader_ler_item(fp);
    }

    ITEM *item = item_criar();
    if ( item == NULL ){
        return NULL;
    }

    char *ptr = linha; // ponteiro que aponta pro primeiro caracter da linha

    // 1. Strings
    parse_string(extrair_campo_csv(&ptr), item->designation, sizeof(item->designation));
    parse_string(extrair_campo_csv(&ptr), item->fullname, sizeof(item->fullname));
    parse_string(extrair_campo_csv(&ptr), item->close_approach_date, sizeof(item->close_approach_date));

    // 2. Inteiros (int / long)
    item->year = parse_int(extrair_campo_csv(&ptr));
    item->month = parse_int(extrair_campo_csv(&ptr));

    // 3. Flutuantes (double)
    item->distance_au = parse_double(extrair_campo_csv(&ptr));
    item->velocity_km_s = parse_double(extrair_campo_csv(&ptr));
    item->absolute_magnitude = parse_double(extrair_campo_csv(&ptr));
    
    // 4. Inteiro Longo (long)
    item->days_until_approach = parse_long(extrair_campo_csv(&ptr));

    // 5. Booleanos (bool)
    item->is_past_event = parse_bool(extrair_campo_csv(&ptr));
    item->is_future_event = parse_bool(extrair_campo_csv(&ptr));

    // 6. Métricas Adicionais (double / int / string)
    item->risk_score = parse_double(extrair_campo_csv(&ptr));
    item->panic_level = parse_int(extrair_campo_csv(&ptr));
    parse_string(extrair_campo_csv(&ptr), item->threat_category, sizeof(item->threat_category));
    parse_string(extrair_campo_csv(&ptr), item->panic_verdict, sizeof(item->panic_verdict));

    // 7. Dados Sentry (bool / double)
    item->on_sentry_list = parse_bool(extrair_campo_csv(&ptr));
    item->sentry_impact_prob = parse_double(extrair_campo_csv(&ptr));
    item->sentry_torino_scale = parse_double(extrair_campo_csv(&ptr));
    item->sentry_palermo_scale = parse_double(extrair_campo_csv(&ptr));
    item->sentry_diameter_km = parse_double(extrair_campo_csv(&ptr));

    return item;
}
        


