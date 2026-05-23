#ifndef SALVATAGGIO_H
#define SALVATAGGIO_H

/*
  File: salvataggio.h
  Autore: [Emanuel Sessa]
  Data:   [19/05/2026]
 
  Descrizione:
  Implementazione delle funzioni per il salvataggio e il caricamento
  persistente dello stato del sistema tra un'esecuzione e l'altra.
  Ogni struttura dati viene serializzata su un file di testo dedicato
  in formato CSV e ricaricata all'avvio tramite le funzioni pubbliche
  dei rispettivi ADT, senza accedere direttamente ai campi interni.
 
  File gestiti:
    studenti.dat  → matricola;nome;corso;in_aula;giorno_accesso
    aula.dat      → g;f;p;matricola;stato  (solo posti non liberi)
    stato.dat     → giorno_attuale;fascia_attuale
    coda.dat      → una matricola per riga (ordine FIFO)
 
  Contesto d'uso:
  Chiamato dal main all'avvio (caricamento) e al termine di ogni
  sessione (salvataggio), garantendo la persistenza dei dati tra
  esecuzioni successive del programma.
 
  Motivazione dell'ADT:
  Centralizzare la logica di I/O su file in un modulo dedicato
  mantiene separazione delle responsabilità e semplifica
  l'eventuale sostituzione del formato di persistenza.
*/

#include "operatore.h"


#define PATH_STUDENTI  "studenti.dat"
#define PATH_AULA      "aula.dat"
#define PATH_STATO     "stato.dat"
#define PATH_CODA      "coda.dat"


/* SALVATAGGIO */

int salvataggio_salva_studenti(TabellaHash *h, const char *path);

int salvataggio_salva_aula(Posto aula[GIORNI][FASCE][POSTI], const char *path);

int salvataggio_salva_stato(StatoSistema *stato, const char *path);

int salvataggio_salva_coda(Coda *c, const char *path);

/*  CARICAMENTO */

int salvataggio_carica_studenti(TabellaHash *h, const char *path);

int salvataggio_carica_aula(Posto aula[GIORNI][FASCE][POSTI], const char *path);

int salvataggio_carica_stato(StatoSistema *stato, const char *path);

int salvataggio_carica_coda(Coda *c, const char *path);

/* OPERAZIONI GLOBALI */

int salvataggio_salva_tutto(TabellaHash *h,
                            Posto aula[GIORNI][FASCE][POSTI],
                            StatoSistema *stato,
                            Coda *coda);

int salvataggio_carica_tutto(TabellaHash *h,
                             Posto aula[GIORNI][FASCE][POSTI],
                             StatoSistema *stato,
                             Coda *coda);

#endif
