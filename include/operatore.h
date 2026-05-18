#ifndef OPERATORE_H
#define OPERATORE_H

/*
  File: operatore.h
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [16/05/2026]

  Descrizione:
  Questo file definisce le strutture dati e le operazioni necessarie
  per la gestione delle funzioni riservate all'operatore.

  L'accesso al menu operatore avviene inserendo la matricola speciale
  "00000" nella fase di identificazione.

  Funzionalità disponibili:
    1. Checkout di fine fascia oraria
       - libera tutti i posti della fascia corrente
       - azzera le prenotazioni scadute (assenti)
       - aggiorna la TabellaHash per ogni studente uscito
       - avanza fascia_attuale
       - assegna automaticamente i posti liberati a chi è
         in lista d'attesa per la fascia successiva

    2. Visualizzazione storico
       - stampa l'intero contenuto del file di log

  Contesto d'uso:
  Utilizzato dal modulo main per gestire le operazioni amministrative
  del sistema e l'avanzamento temporale (fascia e giorno).

  Motivazione dell'ADT:
  StatoSistema racchiude lo stato globale del sistema (fascia e giorno
  correnti), semplificando il passaggio di parametri e garantendo
  coerenza nelle operazioni di gestione temporale.
*/

#include "aula.h"     /* Posto, GIORNI, FASCE, POSTI         */
#include "coda.h"     /* Coda                                 */
#include "hash.h"     /* TabellaHash                          */
#include "storico.h"  /* storico_stampa, path file            */

/*
  Struttura: StatoSistema

  Descrizione:
  Raccoglie lo stato globale del sistema relativo alle dimensioni
  temporali. Viene passata alle funzioni che necessitano di leggere
  o aggiornare il tempo di sistema.

  Campi:
  fascia_attuale  : fascia oraria corrente (0-3).
  giorno_attuale  : giorno della settimana corrente (0=lunedì, 4=venerdì).
*/
typedef struct {
    int fascia_attuale;   /* fascia in corso: 0–3              */
    int giorno_attuale;   /* giorno in corso: 0 (lun)–4 (ven) */
} StatoSistema;

/* Etichette leggibili per fasce e giorni (uso in stampe) */
extern const char *NOMI_FASCE[FASCE];   /* "08-10", "10-12", … */
extern const char *NOMI_GIORNI[GIORNI]; /* "Lunedi", "Martedi", … */


/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: operatore_menu

  Descrizione:
  Mostra il menu operatore in loop finché l'operatore non sceglie
  di uscire. Delega alle funzioni specifiche in base alla scelta
  effettuata.

  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa (singola).
  h             : puntatore alla tabella hash degli studenti.
  stato         : puntatore allo stato di sistema.
  path_storico  : path del file di log.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  aula inizializzata; code inizializzata; h != NULL.
  stato != NULL; path_storico != NULL.

  Post-condizioni:
  Le modifiche alle strutture avvengono nelle funzioni richiamate.

  Effetti collaterali:
  Output su stdout.
*/
void operatore_menu(Posto        aula[GIORNI][FASCE][POSTI],
                    Coda        *code,
                    TabellaHash *h,
                    StatoSistema *stato,
                    const char  *path_storico);

/*
  Funzione: operatore_checkout_fascia

  Descrizione:
  Effettua il checkout di fine fascia oraria liberando tutti i posti
  della fascia corrente. Rimuove le prenotazioni scadute (studenti
  assenti), aggiorna la tabella hash, e assegna automaticamente i
  posti liberati agli studenti in lista d'attesa (singola coda).

  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa (singola).
  h             : puntatore alla tabella hash degli studenti.
  stato         : puntatore allo stato di sistema.
  path_storico  : path del file di log.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  aula inizializzata; code inizializzata; h != NULL.
  stato != NULL; path_storico != NULL.
  stato->fascia_attuale >= 0.

  Post-condizioni:
  Tutti i posti PRESENTE vengono liberati (LIBERO); i posti PRENOTATO
  scaduti vengono liberati; fascia_attuale viene incrementato se non
  è l'ultima fascia; i posti liberati vengono assegnati agli studenti
  estratti dalla coda per la fascia successiva.

  Effetti collaterali:
  Scritture su storico; aggiornamenti della tabella hash; estrazioni da coda.
*/
void operatore_checkout_fascia(Posto        aula[GIORNI][FASCE][POSTI],
                                Coda        *code,
                                TabellaHash *h,
                                StatoSistema *stato,
                                const char  *path_storico);

/*
  Funzione: operatore_visualizza_storico

  Descrizione:
  Stampa il contenuto completo del file di log riga per riga.

  Parametri:
  path_storico : path del file di log.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  path_storico != NULL; file deve esistere (può essere vuoto).

  Post-condizioni:
  Nessuna modifica al file o alle strutture.

  Effetti collaterali:
  Output su stdout.
*/
void operatore_visualizza_storico(const char *path_storico);

/*
  Funzione: operatore_assegna_coda_fascia_successiva

  Descrizione:
  Funzione interna (statica) che assegna automaticamente i posti liberati
  della fascia successiva agli studenti in coda di attesa. Estrae dalla
  coda fino a esaurimento o posti disponibili.

  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa (singola).
  path_storico  : path del file di log.
  giorno        : giorno corrente (0-4).
  fascia        : fascia corrente (0-3).

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  aula inizializzata; code inizializzata.
  fascia < FASCE-1 (esiste una fascia successiva).

  Post-condizioni:
  Gli studenti estratti dalla coda sono assegnati ai posti liberi della
  fascia successiva con stato PRESENTE. Lo storico è aggiornato con
  OP_CODA_ENTRATA per ogni assegnazione.

  Effetti collaterali:
  Estrazioni da coda; scritture su storico.
  Funzione interna (static) - non esportata.
*/
static void operatore_assegna_coda_fascia_successiva(Posto aula[GIORNI][FASCE][POSTI],
                                                     Coda *code,
                                                     const char *path_storico,
                                                     int giorno,
                                                     int fascia);
/*
  Funzione: operatore_stato_inizializza

  Descrizione:
  Inizializza lo stato di sistema impostando fascia_attuale e
  giorno_attuale a 0 (lunedì, prima fascia).

  Parametri:
  stato : puntatore allo stato di sistema.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  stato != NULL.

  Post-condizioni:
  Sistema pronto per la prima fascia del lunedì.

  Effetti collaterali:
  Nessuno.
*/
void operatore_stato_inizializza(StatoSistema *stato);

/*
  Funzione: operatore_avanza_giorno

  Descrizione:
  Avanza il giorno e resetta la fascia a 0. Se raggiunto venerdì
  (fine settimana), stampa un messaggio e non avanza ulteriormente.

  Parametri:
  stato : puntatore allo stato di sistema.

  Valore di ritorno:
  1 se il giorno è stato avanzato correttamente.
  0 se è stata raggiunta la fine della settimana (venerdì).

  Pre-condizioni:
  stato != NULL; stato->giorno_attuale >= 0.

  Post-condizioni:
  Se giorno_attuale < GIORNI-1: giorno_attuale incrementato,
  fascia_attuale impostata a 0.

  Effetti collaterali:
  Output su stdout se fine settimana.
*/
int operatore_avanza_giorno(StatoSistema *stato);


#endif /* OPERATORE_H */
