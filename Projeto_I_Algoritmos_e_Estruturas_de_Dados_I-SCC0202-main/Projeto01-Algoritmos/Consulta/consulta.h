#ifndef CONSULTA_H
#define CONSULTA_H

#include <stdbool.h>
#include "../Item_Nasa/item_nasa.h"

typedef struct filtro FILTRO;
typedef struct similaridade SIMILARIDADE;
typedef struct query QUERY;

QUERY *query_carregar( char *arquivo);
void query_destruir( QUERY **query);
bool query_avaliar_item(QUERY *q, void *item);
char* query_obter_order_campo(QUERY *q);
char* query_obter_cross_campo1(QUERY *q);
char* query_obter_cross_campo2(QUERY *q);
char* query_obter_group_campo1(QUERY *q);
char* query_obter_group_campo2(QUERY *q);

#endif