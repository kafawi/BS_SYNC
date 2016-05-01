# makrefile fuer Syncronisation and threads
#
BIN = proc

CC = /usr/bin/gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lpthread

SRH = fifo.h ccp.h errInfo.h
SRC = errInfo.c fifo.c ccp.c main.c
OBJ = $(SRC:%.c=%.o)

DEPENDFILE = .depend

####
all: $(BIN)

# Berechne Abhaengigkeiten der SRC Datei
#
dep: $(SRC) $(SRH)
	$(CC) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)

# Compilieren und linken
$(BIN) : $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# laufen und Aufraeumen
run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BIN) *.o $(DEPENDFILE)
