# Nome do executável final
TARGET = trabalho

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
LIBS = -lm

# Arquivos fonte atualizados com os nomes corretos dos módulos
SRC = main.c \
      Item_Nasa/item_nasa.c \
      Reader_Nasa/reader_nasa.c \
      Consulta/consulta.c \
      Lista_Cabeca/lista_cabeca.c \
      Lista_Cruz/lista_cruz.c \
      Lista_Enc/lista_enc.c \
      Lista_Gen/lista_gen.c \
      Lista_Ord/lista_ord.c \
      Lista_Seq/lista_seq.c \
      Pilha/pilha.c \
      Fila/fila.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild