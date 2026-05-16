#ifndef CODA_H
#define CODA_H

/*
  File: coda.h
  Autore: [Emanuel Sessa]
  Data:   [15/05/2026]

  Descrizione:
  Questo file definisce le strutture dati e le operazioni necessarie
  per la gestione della coda FIFO per la lista d'attesa. La coda è
  implementata con una lista collegata dinamica, con puntatori a testa
  e fondo per garantire efficienza O(1) sia nell'inserimento che
  nell'estrazione.

  Una coda è gestita per ogni combinazione [giorno][fascia], consentendo
  ai studenti non posizionati immediatamente di attendere il loro turno
  di ingresso.

  Contesto d'uso:
  Utilizzato dai moduli di prenotazione e da operatore per gestire la
  lista d'attesa quando non sono disponibili posti liberi.

  Motivazione dell'ADT:
  La lista collegata consente inserimento e estrazione in O(1) ai
  due estremi, riducendo al minimo i costi computazionali nelle
  operazioni critiche di coda.
*/

#include "aula.h"   /* MAX_MATRICOLA, GIORNI, FASCE, POSTI   */
/* hash.h è già incluso transitivamente tramite aula.h     */

/*
  Struttura: NodoCoda

  Descrizione:
  Rappresenta un singolo nodo della lista collegata interna alla coda.
  Contiene la matricola dello studente in attesa e un puntatore al
  nodo successivo.

  Campi:
  matricola : stringa della matricola dello studente in coda.
  next      : puntatore al nodo successivo (NULL se ultimo nodo).
*/
typedef struct NodoCoda {
    char             matricola[MAX_MATRICOLA];
    struct NodoCoda *next;
} NodoCoda;

/*
  Struttura: Coda

  Descrizione:
  Rappresenta la coda FIFO mantenendo puntatori separati a testa e
  fondo della lista per garantire operazioni di inserimento/estrazione
  efficienti O(1).

  Campi:
  testa  : puntatore al primo nodo (estrazione).
  fondo  : puntatore all'ultimo nodo (inserimento).
*/
typedef struct Coda {
    NodoCoda *testa;   /* primo elemento — estrazione */
    NodoCoda *fondo;   /* ultimo elemento — inserimento */
}Coda;


/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: coda_inizializza

  Descrizione:
  Inizializza la coda impostando testa e fondo a NULL.

  Parametri:
  c : puntatore alla coda da inizializzare.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  c != NULL.

  Post-condizioni:
  La coda è vuota e pronta all'uso.

  Effetti collaterali:
  Nessuno.
*/
void coda_inizializza(Coda *c);

/*
  Funzione: coda_inserisci

  Descrizione:
  Aggiunge un nuovo nodo con la matricola specificata in fondo
  alla coda. Alloca un nuovo NodoCoda con malloc.

  Parametri:
  c         : puntatore alla coda.
  matricola : matricola dello studente da accodare.

  Valore di ritorno:
  1 se l'inserimento è avvenuto correttamente.
  -1 se si verifica un errore di allocazione memoria.

  Pre-condizioni:
  c != NULL.
  matricola != NULL e non vuota.

  Post-condizioni:
  Se allocazione riuscita: nuovo nodo inserito in fondo;
  se c->testa era NULL, anche testa punta al nuovo nodo.

  Effetti collaterali:
  Allocazione dinamica di memoria.
*/
int coda_inserisci(Coda *c, const char *matricola);

/*
  Funzione: coda_estrai

  Descrizione:
  Rimuove e restituisce la matricola del nodo in testa dalla coda.
  Dealloca il nodo con free e aggiorna testa; se la coda diventa
  vuota, imposta fondo a NULL.

  Parametri:
  c              : puntatore alla coda.
  matricola_out  : buffer per la matricola estratta
                   (almeno MAX_MATRICOLA caratteri).

  Valore di ritorno:
  1 se l'estrazione è avvenuta correttamente.
  0 se la coda è vuota.

  Pre-condizioni:
  c != NULL.
  matricola_out è un buffer di almeno MAX_MATRICOLA caratteri.

  Post-condizioni:
  Se la coda non era vuota: nodo estratto deallocato,
  testa aggiornata, fondo = NULL se coda diventa vuota.

  Effetti collaterali:
  Deallocazione dinamica di memoria.
*/
int coda_estrai(Coda *c, char *matricola_out);

/*
  Funzione: coda_vuota

  Descrizione:
  Controlla se la coda è vuota senza modificarla.

  Parametri:
  c : puntatore alla coda.

  Valore di ritorno:
  1 se la coda è vuota (testa == NULL).
  0 se la coda contiene almeno un elemento.

  Pre-condizioni:
  c != NULL.

  Post-condizioni:
  Nessuna modifica alla coda.

  Effetti collaterali:
  Nessuno.
*/
int coda_vuota(const Coda *c);

/*
  Funzione: coda_distruggi

  Descrizione:
  Dealloca interamente la coda deallocando tutti i nodi con free
  e impostando testa e fondo a NULL.

  Parametri:
  c : puntatore alla coda da distruggere.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  c != NULL.

  Post-condizioni:
  La coda è distrutta, testa e fondo impostati a NULL.

  Effetti collaterali:
  Deallocazione dinamica di memoria per tutti i nodi.
*/
void coda_distruggi(Coda *c);


#endif /* CODA_H */
