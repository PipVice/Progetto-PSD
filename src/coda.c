/*
  File: coda.c
  Autore: [Emanuel Sessa]
  Data:   [15/05/2026]

  Descrizione:
  Implementazione di una coda FIFO usando lista collegata dinamica.
  Fornisce operazioni di inserimento in fondo, estrazione dalla testa,
  controllo se vuota, conteggio degli elementi e distruzione completa
  della coda. I dettagli interni della struttura (NodoCoda, puntatori
  testa/fondo) non vengono mai esposti ai moduli chiamanti.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coda.h"
#include "aula.h"

void coda_inizializza(Coda *c) {
    if (c != NULL) {
        c->testa = NULL;
        c->fondo = NULL;
    }
}

int coda_inserisci(Coda *c, const char *matricola) {
    NodoCoda *nuovo = (NodoCoda *)malloc(sizeof(NodoCoda));
    if (nuovo == NULL) {
        printf("Errore: memoria insufficiente per la coda.\n");
        return -1;
    }

    strcpy(nuovo->matricola, matricola);
    nuovo->next = NULL;

    if (c->fondo == NULL) {
        c->testa = nuovo;
        c->fondo = nuovo;
    } else {
        c->fondo->next = nuovo;
        c->fondo = nuovo;
    }

    return 1;
}

int coda_estrai(Coda *c, char *matricola_out) {
    if (c->testa == NULL) {
        return 0;
    }

    NodoCoda *nodo_estratto = c->testa;
    strcpy(matricola_out, nodo_estratto->matricola);
    c->testa = nodo_estratto->next;

    if (c->testa == NULL) {
        c->fondo = NULL;
    }

    free(nodo_estratto);
    return 1;
}

int coda_vuota(const Coda *c) {
    return (c->testa == NULL) ? 1 : 0;
}

int coda_dimensione(const Coda *c) {
    int n = 0;
    NodoCoda *corrente = c->testa;
    while (corrente != NULL) {
        n++;
        corrente = corrente->next;
    }
    return n;
}

void coda_distruggi(Coda *c) {
    if (c == NULL) {
        return;
    }

    NodoCoda *corrente = c->testa;
    while (corrente != NULL) {
        NodoCoda *prossimo = corrente->next;
        free(corrente);
        corrente = prossimo;
    }

    c->testa = NULL;
    c->fondo = NULL;
}
