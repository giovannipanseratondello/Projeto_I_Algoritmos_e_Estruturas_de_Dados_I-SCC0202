#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "Item_Nasa/item_nasa.h"
#include "Reader_Nasa/reader_nasa.h"
#include "Consulta/consulta.h"
#include "Lista_Cabeca/lista_cabeca.h"
#include "Lista_Cruz/lista_cruz.h"
#include "Lista_Enc/lista_enc.h"
#include "Lista_Gen/lista_gen.h"
#include "Lista_Ord/lista_ord.h"
#include "Lista_Seq/lista_seq.h"
#include "Pilha/pilha.h"
#include "Fila/fila.h"

typedef struct contexto{
    QUERY *query;
    void *result_structure;
    char *result_type;
    int *matched_records;
    double *insert_ms;
    void (*extrair_valor_campo)(void *, const char *, char *, int);
    int (*comparar_item)(void *, void *, char *);
} ContextoBusca;

typedef struct {
    FILE *saida;
    int top_k;
    int registros_escritos;
    void (*imprimir_item_csv)(void*,FILE*);
} ContextoEscrita;

void gravar_item_csv(void *item, void *ctx_void){
    ContextoEscrita *ctx = (ContextoEscrita *) ctx_void;

    if(ctx->top_k > 0 && ctx->registros_escritos >= ctx->top_k){
        return;
    }

    ctx->imprimir_item_csv(item,ctx->saida);
    ctx->registros_escritos++;
}

void* criar_estrutura(char *tipo) {
    if (tipo == NULL) return NULL;
    if (strcmp(tipo,        "pilha") == 0)         return pilha_criar();
    if (strcmp(tipo,         "fila") == 0)          return fila_criar();
    if (strcmp(tipo,    "lista_seq") == 0)     return lista_seq_criar();
    if (strcmp(tipo,    "lista_enc") == 0)     return lista_enc_criar();
    if (strcmp(tipo, "lista_cabeca") == 0)  return lista_cabeca_criar();
    if (strcmp(tipo,    "lista_ord") == 0)     return lista_ord_criar();
    if (strcmp(tipo,    "lista_gen") == 0)     return lista_gen_criar();
    if (strcmp(tipo,   "lista_cruz") == 0)    return lista_cruz_criar();
    return NULL;
}

void apagar_estutura(char *tipo, void **estrutura, void liberar_item(void **item)) {
    if (tipo == NULL) return;
    if (strcmp(tipo,        "pilha") == 0){ pilha_apagar((PILHA **)estrutura,liberar_item); return;}        
    if (strcmp(tipo,         "fila") == 0){ fila_apagar((FILA **)estrutura,liberar_item); return;}          
    if (strcmp(tipo,    "lista_seq") == 0){ lista_seq_apagar((LISTA_SEQ **)estrutura,liberar_item); return;}    
    if (strcmp(tipo,    "lista_enc") == 0){ lista_enc_apagar((LISTA_ENC **)estrutura,liberar_item); return;}      
    if (strcmp(tipo, "lista_cabeca") == 0){ lista_cabeca_apagar((LISTA_CABECA **)estrutura,liberar_item); return;}  
    if (strcmp(tipo,    "lista_ord") == 0){ lista_ord_apagar((LISTA_ORD **)estrutura,liberar_item); return;}      
    if (strcmp(tipo,    "lista_gen") == 0){ lista_gen_destruir((LISTA_GEN **)estrutura,liberar_item); return;}      
    if (strcmp(tipo,   "lista_cruz") == 0){ lista_cruz_apagar((LISTA_CRUZ **)estrutura,liberar_item); return;}     
}

int carregar_base(void *base, char *tipo_base, char *input_file, int limit, 
                  void *(*criar_item)(void),
                  void *(*ler_item)(FILE *fp),
                  void (*pular_cabecalho)(FILE *fp),
                  int (*comparar_item)(void *item1, void *item2, char *campo)) {
    
    if (base == NULL || tipo_base == NULL || input_file == NULL || ler_item == NULL) return -1;

    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", input_file);
        return -1;
    }

    if (pular_cabecalho != NULL) {
        pular_cabecalho(fp);
    }
    
    int registros_lidos = 0;

    while (!feof(fp)) {
        if (limit > 0 && registros_lidos >= limit) {
            break;
        }

        // Lê o item de forma genérica usando o ponteiro de função fornecido
        void *item = ler_item(fp);
        if (item != NULL) {
            if (strcmp(tipo_base, "pilha") == 0) {
                pilha_empilhar((PILHA*)base, item);
            } 
            else if (strcmp(tipo_base, "fila") == 0) {
                fila_inserir((FILA*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_seq") == 0) {
                lista_seq_inserir_inicio((LISTA_SEQ*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_enc") == 0) {
                lista_enc_inserir_inicio((LISTA_ENC*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_cabeca") == 0) {
                lista_cabeca_inserir_inicio((LISTA_CABECA*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_ord") == 0) {
                char padrao[] = "year"; 
                lista_ord_inserir((LISTA_ORD*)base, item, padrao, comparar_item);
            } 
            else if (strcmp(tipo_base, "lista_gen") == 0) {
                char padrao1[] = "threat_category", padrao2[] = "year";
                lista_gen_inserir((LISTA_GEN*)base, item, padrao1, padrao2);
            } 
            else if (strcmp(tipo_base, "lista_cruz") == 0) {
                char padrao1[] = "year", padrao2[] = "month";
                lista_cruz_inserir((LISTA_CRUZ*)base, item, padrao1, padrao2);
            }

            registros_lidos++;
        }
    }

    fclose(fp);
    return registros_lidos;
}

// Avalia o item, ou seja, se cumprir os requisitos da query, insere na estrutura de dados
void processar_item(void *item, void *ctx_void) {

    ContextoBusca *ctx = (ContextoBusca*) ctx_void;

    // Avalia o item com a query do projeto
    if (query_avaliar_item(ctx->query, item)) {
        (*(ctx->matched_records))++;

        clock_t inicio_insercao = clock();

        // Insere na estrutura de resultado 
        if (strcmp(ctx->result_type, "pilha") == 0) {
            pilha_empilhar((PILHA*)ctx->result_structure, item);
        } 
        else if (strcmp(ctx->result_type, "fila") == 0) {
            fila_inserir((FILA*)ctx->result_structure, item);
        } 
        else if (strcmp(ctx->result_type, "lista_seq") == 0) {
            lista_seq_inserir_inicio((LISTA_SEQ*)ctx->result_structure, item);
        } 
        else if (strcmp(ctx->result_type, "lista_enc") == 0) {
            lista_enc_inserir_inicio((LISTA_ENC*)ctx->result_structure, item);
        } 
        else if (strcmp(ctx->result_type, "lista_cabeca") == 0) {
            lista_cabeca_inserir_inicio((LISTA_CABECA*)ctx->result_structure, item);
        } 
        else if (strcmp(ctx->result_type, "lista_ord") == 0) {
            char *campo_ordem = query_obter_order_campo(ctx->query);
            char padrao[] = "year";
            if (campo_ordem[0] == '\0') campo_ordem = padrao; 
            lista_ord_inserir((LISTA_ORD*)ctx->result_structure, item, campo_ordem, ctx->comparar_item);
        } 
        else if (strcmp(ctx->result_type, "lista_gen") == 0) {
            // 1. Obtém os dois campos definidos no comando GROUP da consulta
            const char *campo1 = query_obter_group_campo1(ctx->query);
            const char *campo2 = query_obter_group_campo2(ctx->query);
            char padrao1[] = "threat_category", padrao2[] = "year";

            if (campo1 == NULL || campo1[0] == '\0') campo1 = padrao1;
            if (campo2 == NULL || campo2[0] == '\0') campo2 = padrao2;

            char val1[128] = "";
            char val2[128] = "";

            // 2. Extrai os valores usando a função universal
            if (ctx->extrair_valor_campo != NULL) {
                ctx->extrair_valor_campo(item, campo1, val1, sizeof(val1));
                ctx->extrair_valor_campo(item, campo2, val2, sizeof(val2));
            }

            // 3. Insere na lista generalizada universal mantendo o TAD desacoplado
            lista_gen_inserir((LISTA_GEN*)ctx->result_structure, item, val1, val2);
        }
        else if (strcmp(ctx->result_type, "lista_cruz") == 0) {
            char *c1 = query_obter_cross_campo1(ctx->query);
            char *c2 = query_obter_cross_campo2(ctx->query);
            char padrao1[] = "year", padrao2[] = "month";

            if (c1 == NULL || c1[0] == '\0') c1 = padrao1;
            if (c2 == NULL || c2[0] == '\0') c2 = padrao2;

            char val1[128] = "";
            char val2[128] = "";
            
            if (ctx->extrair_valor_campo != NULL) {
                ctx->extrair_valor_campo(item, c1, val1, sizeof(val1));
                ctx->extrair_valor_campo(item, c2, val2, sizeof(val2));
            }

            // Insere na lista cruzada passando as chaves extraídas
            lista_cruz_inserir((LISTA_CRUZ*)ctx->result_structure, item, val1, val2);
        }

        *(ctx->insert_ms) += ((double)(clock() - inicio_insercao)) * 1000.0 / CLOCKS_PER_SEC;
    }
}

int main(int argc, char *argv[]){
    
    char *reader_name = NULL;
    char *input_file = NULL;
    char *base_type = NULL;
    char *result_type = NULL;
    char *query_file = NULL;
    char *output_file = NULL;
    int top_k = 0;
    int limit = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--reader") == 0 && i + 1 < argc) reader_name = argv[++i];
        else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) input_file = argv[++i];
        else if (strcmp(argv[i], "--base") == 0 && i + 1 < argc) base_type = argv[++i];
        else if (strcmp(argv[i], "--result") == 0 && i + 1 < argc) result_type = argv[++i];
        else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) query_file = argv[++i];
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) output_file = argv[++i];
        else if (strcmp(argv[i], "--top-k") == 0 && i + 1 < argc) top_k = atoi(argv[++i]);
        else if (strcmp(argv[i], "--limit") == 0 && i + 1 < argc) limit = atoi(argv[++i]);
    }

    if (!reader_name || !input_file || !base_type || !result_type || !query_file || !output_file) {
        return 1;
    }

    clock_t inicio_tempo;
    double load_ms = 0.0, search_ms = 0.0, insert_ms = 0.0, write_ms = 0.0;
    int input_records = 0;
    int matched_records = 0;
    int output_records = 0;

    inicio_tempo = clock();

    void *base_structure = criar_estrutura(base_type);

    if (!base_structure){
        return 1;
    }

    int (*comparar_item)(void *item1, void *item2, char *campo);
    void *(*criar_item)(void);
    void (*apagar_item)(void**);
    void (*imprimir_item_csv)(void*,FILE*);
    void (*reader_pular_cabecalho) (FILE*);
    void* (*reader_ler_item)(FILE *);
    void (*extrair_valor_campo)(void *, const char *, char *, int);

    if (strcmp(reader_name,"nasa") == 0){
        // faz o cast de funcao, muito doido mas funciona
        comparar_item = (int (*)(void *, void *, char *)) item_nasa_comparar; 
        criar_item = (void *(*)(void)) item_nasa_criar;
        apagar_item = (void (*)(void**)) item_nasa_apagar;
        imprimir_item_csv = (void (*) (void*,FILE*)) item_nasa_imprimir_csv;
        reader_pular_cabecalho = (void (*)(FILE*)) reader_nasa_pular_cabecalho;
        reader_ler_item = (void *(*) (FILE*)) reader_nasa_ler_item;
        extrair_valor_campo = (void (*) (void*,const char*,char*,int)) extrair_valor_campo_item_nasa;
    }
    // aqui a gnt vai atribuir um "item_2_comparar" em um else

    input_records = carregar_base(base_structure, base_type, input_file, limit,criar_item,reader_ler_item,reader_pular_cabecalho,comparar_item);
    
    load_ms = ((double)(clock() - inicio_tempo)) * 1000.0 / CLOCKS_PER_SEC;

    void *result_structure = criar_estrutura(result_type);

    if (!result_structure){
        return 1;
    }

    QUERY *query = query_carregar(query_file);
    if (!query) {
        return 1;
    }

    ContextoBusca ctx = {
        .query = query,
        .result_structure = result_structure,
        .result_type = result_type,
        .matched_records = &matched_records,
        .insert_ms = &insert_ms,
        .extrair_valor_campo = extrair_valor_campo,
        .comparar_item = comparar_item
    };

    inicio_tempo = clock();
    
    if(!strcmp(base_type, "pilha")){
        pilha_percorrer((PILHA *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "fila")){
        fila_percorrer((FILA *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_seq")){
        lista_seq_percorrer((LISTA_SEQ *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_enc")){
        lista_enc_percorrer((LISTA_ENC *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_cabeca")){
        lista_cabeca_percorrer((LISTA_CABECA *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_ord")){
        lista_ord_percorrer((LISTA_ORD *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_gen")){
        lista_gen_percorrer((LISTA_GEN *)base_structure, processar_item, &ctx);
    }
    else if(!strcmp(base_type, "lista_cruz")){
        lista_cruz_percorrer((LISTA_CRUZ *)base_structure, processar_item, &ctx);
    }
    
    search_ms = ((double)(clock() - inicio_tempo)) * 1000.0 / CLOCKS_PER_SEC;

    inicio_tempo = clock();

    FILE *fp_out = fopen(output_file, "w");
    
    if(fp_out != NULL){
        ContextoEscrita ctx_escrita = {
            .saida = fp_out,
            .top_k = top_k,
            .registros_escritos = 0,
            .imprimir_item_csv = imprimir_item_csv
        };

        if(!strcmp(result_type, "pilha")){
            pilha_percorrer((PILHA *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "fila")){
            fila_percorrer((FILA *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_seq")){
            lista_seq_percorrer((LISTA_SEQ *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_enc")){
            lista_enc_percorrer((LISTA_ENC *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_cabeca")){
            lista_cabeca_percorrer((LISTA_CABECA *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_ord")){
            lista_ord_percorrer((LISTA_ORD *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_gen")){
            lista_gen_percorrer((LISTA_GEN *)result_structure, gravar_item_csv, &ctx_escrita);
        }
        else if(!strcmp(result_type, "lista_cruz")){
            lista_cruz_percorrer((LISTA_CRUZ *)result_structure, gravar_item_csv, &ctx_escrita);
        }

        output_records = ctx_escrita.registros_escritos;
        fclose(fp_out);
    }

    write_ms = ((double)(clock() - inicio_tempo)) * 1000.0 / CLOCKS_PER_SEC;

    printf("STATUS=OK\n");
    printf("INPUT_RECORDS=%d\n", input_records);
    printf("MATCHED_RECORDS=%d\n", matched_records);
    printf("OUTPUT_RECORDS=%d\n", output_records);
    printf("LOAD_MS=%.3f\n", load_ms);
    printf("SEARCH_MS=%.3f\n", search_ms);
    printf("RESULT_INSERT_MS=%.3f\n", insert_ms);
    printf("WRITE_MS=%.3f\n", write_ms);

    query_destruir(&query);

    apagar_estutura(base_type, &base_structure, apagar_item);
    apagar_estutura(result_type, &result_structure, apagar_item);

    return 0;
}