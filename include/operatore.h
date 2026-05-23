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

#include "coda.h"     
#include "storico.h"  

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
  Mostra il menu operatore in loop finche l'operatore non sceglie
  di uscire. Delega alle funzioni specifiche in base alla scelta
  effettuata.
 
  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa.
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
  Gestisce la chiusura completa della fascia oraria corrente:
 
  Libera i posti della fascia corrente:
    - per ogni posto PRESENTE: registra CHECK-OUT sullo storico,
      aggiorna in_aula=0 e giorno_accesso=-1 nella tabella hash;
    - per ogni posto PRENOTATO: registra ANNULLAMENTO sullo storico
      (prenotazione scaduta, studente mai arrivato);
    - imposta tutti i posti a LIBERO.
 
  Serve la coda di attesa per la fascia successiva:
    - estrae studenti dalla coda finche ci sono posti liberi
      nella fascia successiva;
    - per ogni studente estratto: assegna il posto con stato PRESENTE,
      aggiorna in_aula=1 e giorno_accesso nella hash,
      registra ENTRATA-DA-CODA sullo storico.
 
  Al termine avanza fascia_attuale. Se e l'ultima fascia del giorno,
  non avanza e invita l'operatore a usare "Avanza giorno".
 
  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa.
  h             : puntatore alla tabella hash degli studenti.
  stato         : puntatore allo stato di sistema.
  path_storico  : path del file di log.
 
  Valore di ritorno:
  Nessuno.
 
  Pre-condizioni:
  aula inizializzata; code inizializzata; h != NULL.
  stato != NULL; path_storico != NULL.
  stato->fascia_attuale in [0, FASCE-1].
 
  Post-condizioni:
  Tutti i posti della fascia corrente sono LIBERO; la tabella hash
  e coerente con lo stato reale; la coda e parzialmente o totalmente
  svuotata; fascia_attuale e incrementato se non era l'ultima fascia.
 
  Effetti collaterali:
  Scritture su storico; aggiornamenti della tabella hash;
  estrazioni dalla coda; output su stdout.
*/
void operatore_checkout_fascia(Posto        aula[GIORNI][FASCE][POSTI],
                                Coda        *code,
                                TabellaHash *h,
                                StatoSistema *stato,
                                const char  *path_storico);
 
/*
  Funzione: operatore_visualizza_storico
 
  Descrizione:
  Stampa il contenuto completo del file di log riga per riga
  delegando a storico_stampa().
 
  Parametri:
  path_storico : path del file di log.
 
  Valore di ritorno:
  Nessuno.
 
  Pre-condizioni:
  path_storico != NULL; file deve esistere (puo essere vuoto).
 
  Post-condizioni:
  Nessuna modifica al file o alle strutture.
 
  Effetti collaterali:
  Output su stdout.
*/
void operatore_visualizza_storico(const char *path_storico);
 
/*
  Funzione: operatore_stato_inizializza
 
  Descrizione:
  Inizializza lo stato di sistema impostando fascia_attuale e
  giorno_attuale a 0 (lunedi, prima fascia).
 
  Parametri:
  stato : puntatore allo stato di sistema.
 
  Valore di ritorno:
  Nessuno.
 
  Pre-condizioni:
  stato != NULL.
 
  Post-condizioni:
  Sistema pronto per la prima fascia del lunedi.
 
  Effetti collaterali:
  Nessuno.
*/
void operatore_stato_inizializza(StatoSistema *stato);
 
/*
  Funzione: operatore_avanza_giorno
 
  Descrizione:
  Avanza il giorno, resetta la fascia a 0 e svuota la coda di attesa
  (gli studenti in coda erano riferiti al giorno precedente e non
  devono essere riportati al giorno successivo).
  Se e gia venerdi non avanza ulteriormente.
 
  Parametri:
  stato : puntatore allo stato di sistema.
  code  : puntatore alla coda di attesa da svuotare.
  h     : puntatore alla tabella hash (per reset accesso_effettuato_oggi).
 
  Valore di ritorno:
  1 se il giorno e stato avanzato correttamente.
  0 se e stata raggiunta la fine della settimana (venerdi).
 
  Pre-condizioni:
  stato != NULL; stato->giorno_attuale in [0, GIORNI-1].
  code != NULL; h != NULL.
 
  Post-condizioni:
  Se giorno_attuale < GIORNI-1: giorno_attuale incrementato,
  fascia_attuale impostata a 0, coda svuotata.
 
  Effetti collaterali:
  Deallocazione dei nodi della coda.
*/
int operatore_avanza_giorno(StatoSistema *stato, Coda *code, TabellaHash *h);
 
 
#endif 