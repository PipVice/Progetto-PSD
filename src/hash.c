/*
  File: hash.c
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [13/05/2026]

  Descrizione:
  Implementazione della tabella hash con chaining per la gestione
  degli studenti registrati. Fornisce operazioni di inserimento,
  ricerca e aggiornamento in tempo O(1) medio.
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hash.h"
#include <string.h>

unsigned int converti(const char *matricola){
    unsigned int x = 0;
    int i=0;
    while(matricola[i]!='\0'){
        x = x * 31 + matricola[i];
        i++;
    }
    return x % HASH_SIZE;
}

void hash_inizializza(TabellaHash *h){
    int i=0;
    while (i<HASH_SIZE){
        h->bucket[i] = NULL;
        i++;
    }   
}

int hash_inserisci(TabellaHash *h, const char *matricola, const char *nome, const char *corso) {
    unsigned int m = converti(matricola);
    
    Studente *corrente = h->bucket[m];
    while (corrente != NULL) {
        if (strcmp(corrente->matricola, matricola) == 0) {
            return 0;
        }
        corrente = corrente->next;
    }

    Studente *s = malloc(sizeof(Studente));
    if (s == NULL) {
        return -1;
    }

    strcpy(s->matricola, matricola);
    strcpy(s->nome, nome);
    strcpy(s->corso, corso);
    s->in_aula = 0;
    s->giorno_accesso = -1;

    s->next = h->bucket[m];
    h->bucket[m] = s;

    return 1;
}

Studente *hash_cerca(TabellaHash *h, const char *matricola) {
    unsigned int m = converti(matricola);
    Studente *corrente = h->bucket[m];
    
    while (corrente != NULL) {
        if (strcmp(corrente->matricola, matricola) == 0) {
            return corrente;
        }
        corrente = corrente->next;
    }
    
    return NULL;
}

void hash_aggiorna_presenza(TabellaHash *h, const char *matricola, int in_aula) {
    Studente *s = hash_cerca(h, matricola);

    if (s != NULL) {
        s->in_aula = in_aula;
    }
}

void hash_aggiorna_giorno(TabellaHash *h, const char *matricola, int giorno) {
    Studente *s = hash_cerca(h, matricola);

    if (s != NULL) {
        s->giorno_accesso = giorno;
    }
}

void hash_distruggi(TabellaHash *h) {
    if (h == NULL) return;

    for (int i = 0; i < HASH_SIZE; i++) {
        Studente *corrente = h->bucket[i];

        while (corrente != NULL) {
            Studente *prossimo = corrente->next;
            free(corrente);
            corrente = prossimo;
        }
        
        h->bucket[i] = NULL;
    }
}


