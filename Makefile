/*
  File: Makefile
  Autore: [Giuseppe Pio Vicedomini]
  Data: [2024-06-01]
  
  Descrizione: Makefile per la compilazione del progetto "Gestione Aula Studio".
			   Include regole per la compilazione dell'applicazione principale,
			   della suite di test e per la pulizia dei file generati.
  
*/

CC     = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src
TST = test

CORE = $(SRC)/aula.o $(SRC)/coda.o $(SRC)/hash.o $(SRC)/operatore.o \
	$(SRC)/report.o $(SRC)/salvataggio.o $(SRC)/storico.o

.PHONY: all test run_test clean clean_data

all: gestione_aula

gestione_aula: $(CORE) $(SRC)/main.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo ">>> Compilazione completata: ./gestione_aula"

esegui_test: $(CORE) $(TST)/test_suite.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo ">>> Compilazione test completata: ./esegui_test"

test: esegui_test

run_test: esegui_test
	@echo ""
	./esegui_test

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TST)/%.o: $(TST)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TST)/*.o gestione_aula esegui_test
	@echo ">>> File oggetto ed eseguibili rimossi."

clean_data: clean
	rm -f *.dat *.log
	@echo ">>> File dati e log rimossi."