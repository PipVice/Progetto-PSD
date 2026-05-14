#ifndef HASH_H
#define HASH_H

/*
  File: hash.h
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [13/05/2026]

  Descrizione:
  Questo file definisce le strutture dati e le operazioni necessarie
  per la gestione della tabella hash degli studenti registrati.
  La tabella utilizza il metodo di concatenazione (chaining) per
  risolvere le collisioni, con liste collegate in ogni bucket.

  Chiave: matricola (stringa di 5 cifre + terminatore '\0').
  Valore: struct Studente con dati anagrafici e stato di presenza.

  La tabella consente ricerca, inserimento e aggiornamento efficienti
  in tempo O(1) medio, fondamentali per verificare l'identità degli
  studenti e tracciarne lo stato.

  Contesto d'uso:
  Utilizzato da tutti i moduli che necessitano di gestire i dati
  anagrafici e lo stato di presenza degli studenti.

  Motivazione dell'ADT:
  La tabella hash con chaining offre inserimento O(1) medio e ricerca
  efficiente, essenziali per identificare gli studenti durante le
  operazioni di ingresso/uscita.
*/

#define HASH_SIZE       101   /* numero di bucket (primo) */
#define MAX_MATRICOLA     6   /* 5 cifre + '\0'           */
#define MAX_NOME         50
#define MAX_CORSO        50

/*
  Struttura: Studente

  Descrizione:
  Rappresenta un singolo studente registrato nel sistema. È il nodo
  della lista di trabocco (chaining) presente in ogni bucket della
  tabella hash.

  Campi:
  matricola      : chiave primaria, stringa di 5 cifre.
  nome           : nome anagrafico dello studente.
  corso          : corso di appartenenza.
  in_aula        : stato di presenza (0 = fuori, 1 = presente).
  giorno_accesso : indice del giorno di accesso (0-4) o -1 se non
                   ancora entrato oggi; impedisce ingressi multipli
                   nello stesso giorno.
  next           : puntatore al nodo successivo nella lista di
                   trabocco (collisioni hash).
*/
typedef struct Studente {
    char            matricola[MAX_MATRICOLA];
    char            nome[MAX_NOME];
    char            corso[MAX_CORSO];
    int             in_aula;        /* 0 = fuori, 1 = presente */
    int             giorno_accesso; /* 0-4; -1 = mai entrato oggi */
    struct Studente *next;          /* lista di trabocco */
} Studente;

/*
  Struttura: TabellaHash

  Descrizione:
  Rappresenta la tabella hash. È un array di HASH_SIZE puntatori,
  ognuno dei quali punta a una lista collegata di Studente.

  Campi:
  bucket : array di HASH_SIZE puntatori a liste di Studente.
*/
typedef struct {
    Studente *bucket[HASH_SIZE];
} TabellaHash;


/* ============================================================
   SPECIFICA DELLE FUNZIONI
   ============================================================ */

/*
  Funzione: hash_init

  Descrizione:
  Inizializza la tabella hash impostando tutti i bucket a NULL.

  Parametri:
  h : puntatore alla tabella hash da inizializzare.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  h != NULL.

  Post-condizioni:
  Tutti i bucket sono NULL; tabella pronta all'uso.

  Effetti collaterali:
  Nessuno.
*/
void hash_init(TabellaHash *h);

/*
  Funzione: hash_inserisci

  Descrizione:
  Inserisce un nuovo studente nella tabella hash. Alloca un nuovo
  nodo Studente con malloc e lo inserisce in testa al bucket
  corrispondente alla matricola. Il campo giorno_accesso viene
  inizializzato a -1.

  Parametri:
  h         : puntatore alla tabella hash.
  matricola : matricola dello studente (chiave).
  nome      : nome dello studente.
  corso     : corso di appartenenza.

  Valore di ritorno:
  1 se l'inserimento è avvenuto correttamente.
  0 se la matricola è già presente (duplicato).
  -1 se si verifica un errore di allocazione memoria.

  Pre-condizioni:
  h != NULL.
  matricola, nome, corso != NULL e non vuoti.
  matricola non già presente nella tabella.

  Post-condizioni:
  Se allocazione riuscita: nuovo Studente inserito in testa al
  bucket corrispondente; giorno_accesso = -1.

  Effetti collaterali:
  Allocazione dinamica di memoria.
*/
int hash_inserisci(TabellaHash *h,
                   const char  *matricola,
                   const char  *nome,
                   const char  *corso);

/*
  Funzione: hash_cerca

  Descrizione:
  Ricerca uno studente nella tabella hash per matricola.

  Parametri:
  h         : puntatore alla tabella hash.
  matricola : matricola dello studente da cercare.

  Valore di ritorno:
  Puntatore allo Studente se trovato.
  NULL se non trovato.

  Pre-condizioni:
  h != NULL.
  matricola != NULL.

  Post-condizioni:
  Nessuna modifica alla tabella.

  Effetti collaterali:
  Nessuno.
*/
Studente *hash_cerca(TabellaHash *h, const char *matricola);

/*
  Funzione: hash_aggiorna_presenza

  Descrizione:
  Aggiorna il campo in_aula dello studente identificato dalla
  matricola.

  Parametri:
  h         : puntatore alla tabella hash.
  matricola : matricola dello studente.
  in_aula   : nuovo valore dello stato di presenza (0 o 1).

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  h != NULL.
  matricola presente nella tabella.
  in_aula == 0 oppure 1.

  Post-condizioni:
  Campo in_aula dello studente aggiornato al valore dato.

  Effetti collaterali:
  Nessuno.
*/
void hash_aggiorna_presenza(TabellaHash *h,
                             const char  *matricola,
                             int          in_aula);

/*
  Funzione: hash_aggiorna_giorno

  Descrizione:
  Aggiorna il campo giorno_accesso dello studente. Utilizzato per
  tenere traccia del giorno di primo ingresso e impedire ingressi
  multipli nello stesso giorno.

  Parametri:
  h         : puntatore alla tabella hash.
  matricola : matricola dello studente.
  giorno    : indice del giorno (0-4) oppure -1 per reset.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  h != NULL.
  matricola presente nella tabella.
  0 <= giorno <= 4 oppure giorno == -1.

  Post-condizioni:
  Campo giorno_accesso dello studente aggiornato.

  Effetti collaterali:
  Nessuno.
*/
void hash_aggiorna_giorno(TabellaHash *h,
                           const char  *matricola,
                           int          giorno);

/*
  Funzione: hash_distruggi

  Descrizione:
  Dealloca interamente la tabella hash deallocando tutti i nodi
  Studente con free e impostando tutti i bucket a NULL.

  Parametri:
  h : puntatore alla tabella hash da distruggere.

  Valore di ritorno:
  Nessuno.

  Pre-condizioni:
  h != NULL.

  Post-condizioni:
  Tutta la memoria allocata per i nodi Studente è deallocata;
  tutti i bucket impostati a NULL.

  Effetti collaterali:
  Deallocazione dinamica di memoria per tutti i nodi.
*/
void hash_distruggi(TabellaHash *h);


#endif /* HASH_H */
