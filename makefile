# makrefile fuer Syncronisation and threads
#
BIN = proc.x

CC = /usr/bin/gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lpthread

SRH = fifo.h pcc.h errInfo.h setting.h
SRC = fifo.c pcc.c main.c
OBJ = $(SRC:%.c=%.o)
DEP = .depend

####
all: $(BIN)

# Berechne Abhaengigkeiten der SRC Datei
#
$(DEP): $(SRC) $(SRH)
	$(CC) -MM $(SRC) > $(DEP)

-include $(DEP)
# Compilieren und linken
$(BIN) : $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)


# laufen und Aufraeumen
run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(DEP) $(BIN) $(OBJ)
