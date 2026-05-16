#ifndef STORICO_H
#define STORICO_H

/*
  File: storico.h
  Autore: [INSERISCI NOME]
  Data:   [INSERISCI DATA]

  Descrizione:
  Questo file definisce le funzioni per la gestione del file di log
  (storico) delle operazioni. Ogni operazione significativa viene
  registrata su file in modalità append. Il file persiste tra un'
  esecuzione e l'altra e può essere consultato dall'operatore.

  Formato di ogni riga di log:
    [Gg Ff] MAT12  OPERAZIONE  posto P

  Esempi:
    [G0 F1] 12345  PRENOTAZIONE  posto 7
    [G0 F1] 12345  CHECK-IN      posto 7
    [G0 F1] 12345  CHECK-OUT     posto 7

  Contesto d'uso:
  Utilizzato da tutti i moduli che effettuano operazioni critiche
  (prenotazione, check-in, check-out, walk-in, annullamento) per
  tracciare le azioni effettuate nel sistema.

  Motivazione dell'ADT:
  Un file di log centrale centralizza l'audit trail del sistema,
  facilitando debug, controllo e analisi delle operazioni effettuate.
*/

#include "aula.h"   /* MAX_MATRICOLA (tramite hash.h), GIORNI, FASCE */

/* Operazioni registrabili — usate come stringa in storico_scrivi */
#define OP_REGISTRAZIONE  "REGISTRAZIONE"
#define OP_PRENOTAZIONE   "PRENOTAZIONE"
#define OP_ANNULLAMENTO   "ANNULLAMENTO"
#define OP_CHECKIN        "CHECK-IN"
#define OP_WALKIN         "WALK-IN"
#define OP_CHECKOUT       "CHECK-OUT"
#define OP_CODA_ENTRATA   "ENTRATA-DA-CODA"
#define OP_CODA_ATTESA    "MESSO-IN-CODA"


/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: storico_scrivi

  Descrizione:
  Registra un'operazione nel file di log in modalità append.
  La riga viene formattata come:
    [G<g> F<f>] <matricola>  <operazione>  posto <posto>
  Se posto == -1, la parte "posto" non viene stampata
  (utilizzato per REGISTRAZIONE e MESSO-IN-CODA).

  Parametri:
  path       : path del file di log.
  matricola  : matricola dello studente coinvolto.
  operazione : stringa dell'operazione (es. "PRENOTAZIONE").
  giorno     : indice del giorno (0-4).
  fascia     : indice della fascia oraria (0-3).
  posto      : indice del posto (-1 se non applicabile).

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  path != NULL; operazione != NULL.
  File deve essere apribile in modalità append ("a").

  Post-condizioni:
  Una riga viene aggiunta in fondo al file nel formato specificato.

  Effetti collaterali:
  Scrittura su file (append).
*/
void storico_scrivi(const char *path,
                    const char *matricola,
                    const char *operazione,
                    int         giorno,
                    int         fascia,
                    int         posto);

/*
  Funzione: storico_stampa

  Descrizione:
  Legge e stampa su stdout il contenuto completo del file di log
  riga per riga.

  Parametri:
  path : path del file di log.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  path != NULL; file deve esistere (può essere vuoto).

  Post-condizioni:
  Nessuna modifica al file.
  Contenuto del file stampato su stdout riga per riga.

  Effetti collaterali:
  Output su stdout. Lettura da file.
*/
void storico_stampa(const char *path);


#endif /* STORICO_H */
