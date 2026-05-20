#ifndef REPORT_H
#define REPORT_H

/*
  File: report.h
  Autore: [Emanuel Sessa]
  Data:   [20/05/2026]

  Descrizione:
  Questo file definisce la funzione per la generazione del report
  statistico finale. Il report legge lo stato corrente delle strutture
  dati e l'intero file di storico per produrre le statistiche richieste
  dalla traccia del progetto.

  Statistiche generate:
    - Numero totale di prenotazioni
    - Accessi effettivi (check-in + walk-in)
    - Occupazione media per fascia oraria (%)
    - Studenti assenti (prenotati ma non presentati)
    - Studenti rimasti in lista d'attesa (coda singola)

  Contesto d'uso:
  Chiamato alla fine del programma per produrre un report riassuntivo
  delle statistiche d'uso dell'aula.

  Motivazione dell'ADT:
  Una funzione unica dedicata alla generazione del report consente di
  centralizzare la logica di elaborazione e mantenere separazione delle
  responsabilità.
*/

#include "aula.h"   /* Posto, GIORNI, FASCE, POSTI */
#include "coda.h"   /* Coda (singola) */


/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: report_genera

  Descrizione:
  Genera e stampa un report statistico finale del sistema. Analizza:
    - la matrice Posto (stato attuale dell’aula)
    - la coda di attesa (singola)
    - l'intero file di storico (lettura completa)

  Parametri:
  aula          : matrice tridimensionale dei posti.
  code          : puntatore alla coda di attesa (singola).
  path_storico  : path del file di log.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  aula e coda inizializzate; path_storico != NULL.
  Il file di storico deve esistere (può essere vuoto).

  Post-condizioni:
  Nessuna modifica alle strutture dati.
  Stampa su stdout contiene:
    1. Totale prenotazioni registrate
    2. Totale accessi effettivi
    3. Occupazione media per fascia (%)
    4. Studenti assenti (prenotati non presentati)
    5. Studenti rimasti in coda a fine giornata

  Effetti collaterali:
  Output su stdout. Lettura dal file di storico.
*/
void report_genera(Posto       aula[GIORNI][FASCE][POSTI],
                   Coda       *code,
                   const char *path_storico);

#endif /* REPORT_H */
