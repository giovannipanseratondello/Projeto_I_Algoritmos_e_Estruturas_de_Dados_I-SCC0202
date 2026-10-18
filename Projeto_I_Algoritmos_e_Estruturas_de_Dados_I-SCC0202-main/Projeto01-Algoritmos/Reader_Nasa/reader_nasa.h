#ifndef READER_NASA_H
#define READER_NASA_H

#include <stdbool.h>
#include "../Item_Nasa/item_nasa.h"

//Lê e ignora a primeira linha (cabeçalho) do arquivo CSV.
void reader_nasa_pular_cabecalho(FILE *fp);

//Lê uma linha do arquivo CSV de asteroides, faz o parse de todos os 20 campos e retorna um ponteiro para ITEM alocado dinamicamente. Retorna NULL ao atingir o Fim de Arquivo (EOF) ou em caso de erro de leitura.
ITEM_NASA* reader_nasa_ler_item(FILE *fp);

#endif