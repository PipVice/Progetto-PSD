#ifndef AULA_H
#define AULA_H

/*
  File: aula.h
  Autore: [Emanuel Sessa]
  Data:   [13/05/2026]

  Descrizione:
  Questo file definisce le strutture dati e le operazioni necessarie
  per la gestione dei posti in un'aula studio. L’aula è modellata come
  un array tridimensionale statico indicizzato per giorno, fascia oraria
  e numero di posto.

  Ogni posto contiene uno stato (libero, prenotato, presente) e la
  matricola dello studente associato. Le funzioni permettono di
  effettuare prenotazioni, check-in, walk-in, check-out, annullamenti
  e visualizzazioni.

  Contesto d’uso:
  Utilizzato dai moduli di gestione prenotazioni, accessi e operatore.
  La gestione delle code di attesa è delegata ad altri moduli.

  Motivazione dell’ADT:
  L’array tridimensionale consente accesso diretto O(1) ai posti,
  garantendo efficienza nelle operazioni di aggiornamento.
*/

#include "hash.h"   /* Per MAX_MATRICOLA */

/*
  Costanti dimensionali dell’aula.
  GIORNI: numero di giorni gestiti (lun–ven).
  FASCE:  numero di fasce orarie giornaliere.
  POSTI:  numero di posti disponibili per fascia.
*/
#define GIORNI   5
#define FASCE    4
#define POSTI   30

/*
  Stati possibili di un posto.
  LIBERO: nessuno studente associato.
  PRENOTATO: posto riservato ma studente non ancora presente.
  PRESENTE: studente fisicamente in aula.
*/
#define LIBERO     0
#define PRENOTATO  1
#define PRESENTE   2

/*
  Struttura: Posto

  Descrizione:
  Rappresenta un singolo posto dell’aula. Contiene la matricola
  dello studente associato e lo stato corrente.

  Campi:
  matricola : stringa vuota se il posto è libero.
  stato     : uno dei valori LIBERO, PRENOTATO, PRESENTE.
*/
typedef struct {
    char matricola[MAX_MATRICOLA];
    int  stato;
} Posto;

/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: aula_init

  Descrizione:
  Inizializza l’intera struttura dell’aula impostando tutti i posti
  allo stato LIBERO e matricola vuota.

  Parametri:
  aula : matrice tridimensionale dei posti.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  L’array deve essere correttamente allocato dal chiamante.

  Post-condizioni:
  Tutti i posti risultano liberi e privi di matricola.

  Effetti collaterali:
  Nessuno.
*/
void aula_init(Posto aula[GIORNI][FASCE][POSTI]);

/*
  Funzione: aula_prenota

  Descrizione:
  Tenta di assegnare un posto libero nella fascia indicata.
  Se la prenotazione ha successo, il posto viene marcato come
  PRENOTATO e viene restituito l’indice del posto assegnato.

  Parametri:
  aula            : matrice tridimensionale dei posti.
  g               : giorno (0..GIORNI-1).
  f               : fascia oraria (0..FASCE-1).
  matricola       : matricola dello studente.
  posto_assegnato : puntatore all’indice del posto assegnato.

  Valore di ritorno:
  1 se la prenotazione è stata effettuata.
  0 se non ci sono posti liberi.

  Pre-condizioni:
  Indici g e f validi.
  matricola non NULL.
  posto_assegnato non NULL.

  Post-condizioni:
  Se esiste un posto libero, esso viene marcato PRENOTATO.
  In caso contrario, nessuna modifica all’aula.

  Effetti collaterali:
  Nessuno. La gestione della coda è responsabilità del chiamante.
*/
int aula_prenota(Posto aula[GIORNI][FASCE][POSTI],
                 int g, int f,
                 const char *matricola,
                 int *posto_assegnato);

/*
  Funzione: aula_checkin

  Descrizione:
  Convalida la presenza di uno studente che ha una prenotazione
  attiva nella fascia indicata. Se la cella corrispondente contiene
  la sua matricola ed è in stato PRENOTATO, viene aggiornata a PRESENTE.

  Parametri:
  aula      : matrice tridimensionale dei posti.
  g         : giorno (0..GIORNI-1).
  f         : fascia oraria (0..FASCE-1).
  matricola : matricola dello studente.

  Valore di ritorno:
  1 se il check-in è avvenuto correttamente.
  0 se non esiste una prenotazione valida.

  Pre-condizioni:
  Indici g e f validi.
  matricola non NULL.

  Post-condizioni:
  Se la prenotazione esiste, lo stato diventa PRESENTE.

  Effetti collaterali:
  Nessuno.
*/
int aula_checkin(Posto aula[GIORNI][FASCE][POSTI],
                 int g, int f,
                 const char *matricola);

/*
  Funzione: aula_walkin

  Descrizione:
  Permette l’ingresso di uno studente senza prenotazione.
  Se esiste un posto libero, esso viene assegnato e marcato
  come PRESENTE.

  Parametri:
  aula            : matrice tridimensionale dei posti.
  g               : giorno (0..GIORNI-1).
  f               : fascia oraria (0..FASCE-1).
  matricola       : matricola dello studente.
  posto_assegnato : puntatore all’indice del posto assegnato.

  Valore di ritorno:
  1 se l’ingresso è consentito.
  0 se non ci sono posti liberi.

  Pre-condizioni:
  Indici g e f validi.
  matricola non NULL.
  posto_assegnato non NULL.

  Post-condizioni:
  Se esiste un posto libero, esso viene marcato PRESENTE.

  Effetti collaterali:
  Nessuno. La gestione della coda è esterna.
*/
int aula_walkin(Posto aula[GIORNI][FASCE][POSTI],
                int g, int f,
                const char *matricola,
                int *posto_assegnato);

/*
  Funzione: aula_checkout

  Descrizione:
  Rimuove uno studente presente in aula liberando il posto
  associato. Restituisce l’indice del posto liberato.

  Parametri:
  aula      : matrice tridimensionale dei posti.
  g         : giorno (0..GIORNI-1).
  f         : fascia oraria (0..FASCE-1).
  matricola : matricola dello studente.

  Valore di ritorno:
  Indice del posto liberato, oppure -1 se non trovato.

  Pre-condizioni:
  Indici g e f validi.
  matricola non NULL.

  Post-condizioni:
  Se lo studente era PRESENTE, il posto torna LIBERO.

  Effetti collaterali:
  Il chiamante deve gestire l’eventuale coda di attesa.
*/
int aula_checkout(Posto aula[GIORNI][FASCE][POSTI],
                  int g, int f,
                  const char *matricola);

/*
  Funzione: aula_annulla_prenotazione

  Descrizione:
  Annulla una prenotazione esistente liberando il posto
  associato. Restituisce l’indice del posto liberato.

  Parametri:
  aula      : matrice tridimensionale dei posti.
  g         : giorno (0..GIORNI-1).
  f         : fascia oraria (0..FASCE-1).
  matricola : matricola dello studente.

  Valore di ritorno:
  Indice del posto liberato, oppure -1 se non trovato.

  Pre-condizioni:
  Indici g e f validi.
  matricola non NULL.

  Post-condizioni:
  Se la prenotazione esiste, il posto torna LIBERO.

  Effetti collaterali:
  Il chiamante deve gestire la coda di attesa.
*/
int aula_annulla_prenotazione(Posto aula[GIORNI][FASCE][POSTI],
                              int g, int f,
                              const char *matricola);

/*
  Funzione: aula_posti_liberi

  Descrizione:
  Conta il numero di posti liberi nella fascia indicata.

  Parametri:
  aula : matrice tridimensionale dei posti.
  g    : giorno (0..GIORNI-1).
  f    : fascia oraria (0..FASCE-1).

  Valore di ritorno:
  Numero di posti con stato LIBERO.

  Pre-condizioni:
  Indici g e f validi.

  Post-condizioni:
  Nessuna modifica all’aula.

  Effetti collaterali:
  Nessuno.
*/
int aula_posti_liberi(Posto aula[GIORNI][FASCE][POSTI],
                      int g, int f);

/*
  Funzione: aula_stampa_mappa

  Descrizione:
  Stampa su stdout la mappa dei posti della fascia indicata.
  Simboli utilizzati:
    '+' per LIBERO
    'P' per PRENOTATO
    '*' per PRESENTE

  Parametri:
  aula : matrice tridimensionale dei posti.
  g    : giorno (0..GIORNI-1).
  f    : fascia oraria (0..FASCE-1).

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  Indici g e f validi.

  Post-condizioni:
  Nessuna modifica all’aula.

  Effetti collaterali:
  Output su stdout.
*/
void aula_stampa_mappa(Posto aula[GIORNI][FASCE][POSTI],
                       int g, int f);

#endif /* AULA_H */
