#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "Item/item.h"
#include "Reader/reader.h"
#include "Consulta/consulta.h"
#include "Lista_Cabeca/lista_cabeca.h"
#include "Lista_Cruz/lista_cruz.h"
#include "Lista_Enc/lista_enc.h"
#include "Lista_Gen/lista_gen.h"
#include "Lista_Ord/lista_ord.h"
#include "Lista_Seq/lista_seq.h"
#include "Pilha/pilha.h"
#include "Fila/fila.h"

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

int carregar_base(void *base, char *tipo_base, char *input_file, int limit) {
    if (base == NULL || tipo_base == NULL || input_file == NULL) return -1;

    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", input_file);
        return -1;
    }

    reader_pular_cabecalho(fp);
    int registros_lidos = 0;

    while (!feof(fp)) {
        if (limit > 0 && registros_lidos >= limit) {
            break;
        }

        ITEM *item = reader_ler_item(fp);
        if (item != NULL) {
            if (strcmp(tipo_base, "pilha") == 0) {
                pilha_empilhar((PILHA*)base, item);
            } 
            else if (strcmp(tipo_base, "fila") == 0) {
                fila_inserir((FILA*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_seq") == 0) {
                lista_seq_inserir((LISTA_SEQ*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_enc") == 0) {
                lista_enc_inserir((LISTA_ENC*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_cabeca") == 0) {
                lista_cabeca_inserir((LISTA_CABECA*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_ord") == 0) {
                lista_inserir_ordenada((LISTA_ORD*)base, item, "year"); // Padronizamos para ordenar a estrutura com base no Ano
            } 
            else if (strcmp(tipo_base, "lista_gen") == 0) {
                lista_gen_inserir_item((LISTA_GEN*)base, item);
            } 
            else if (strcmp(tipo_base, "lista_cruz") == 0) {
                lista_cruz_inserir((LISTA_CRUZ*)base, item, "year", "month"); // Padronizamos para ordenar a estrutura com base no Ano e Mês
            }

            registros_lidos++;
        }
    }

    fclose(fp);
    return registros_lidos;
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

    input_records = carregar_base(base_structure, base_type, input_file, limit);
    
    load_ms = ((double)(clock() - inicio_tempo)) * 1000.0 / CLOCKS_PER_SEC;

    void *result_structure = criar_estrutura(result_type);

    if (!result_structure){
        return 1;
    }

    QUERY *query = query_carregar(query_file);
    if (!query) {
        return 1;
    }

    // A partir daqui, você fará o percurso da 'base_structure', 
    // avaliará com query_avaliar_item(query, item),
    // e inserirá na 'result_structure' checando o 'result_type' via strcmp direto.

    // Exemplo de inserção direta no resultado (sem wrappers):
    /*
    if (strcmp(result_type, "pilha") == 0) {
        pilha_empilhar((PILHA*)result_structure, item);
    } else if (strcmp(result_type, "lista_ord") == 0) {
        lista_inserir_ordenada((LISTA_ORD*)result_structure, item, query->order_campo);
    } // ... e assim por diante para os outros TADs
    */

    // Liberação de memória ao fim:
    query_destruir(&query);

    return 0;
}