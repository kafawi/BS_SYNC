# makrefile fuer Syncronisation and threads
#

CC = /usr/bin/gcc
CFAGS = -Wall -ggdb
LDFLAGS = -lpthread

SRC = fifo.c errInfo.c ccp.c main.c fifo.h ccp.h errInfo.h
OBJ = $(SRC:%.c=%.o)


####
proc: $(OBJ) 
	$(CC) -o proc $(OBJ)
	
	

DEPENDFILE = .depend

# Berechne Abhaengigkeiten der SRC Datei
#
dep:  $(SRC)
	$(CC) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)

# Compilieren und linken
$(BIN) :
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Aufraeumen

clean:
	rm -rf $(BIN) $(OBJ) $(DEPENDFILE) 
	rm -rf proc
