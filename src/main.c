/*
  File: main.c
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [21/05/2026]
  Descrizione:
  Punto di ingresso del sistema di gestione dell'aula studio.

  Flusso all'avvio:
    1. Carica lo stato persistente (studenti, aula, stato, coda).
    2. Chiede la matricola:
       - "00000" → menu operatore
       - matricola non registrata → registrazione automatica, poi menu studente
       - matricola registrata → menu studente
    3. Al termine salva tutto su disco.

  Matricola operatore speciale: "00000"

  Note sull'information hiding:
  Tutte le operazioni sulle strutture Posto, Coda e TabellaHash avvengono
  esclusivamente tramite le funzioni dei rispettivi ADT. Il main non accede
  mai direttamente ai campi interni delle strutture dati.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aula.h"
#include "hash.h"
#include "coda.h"
#include "operatore.h"
#include "storico.h"
#include "salvataggio.h"
#include "report.h"

#define MATRICOLA_OPERATORE "00000"
#define PATH_STORICO        "storico.log"


const char *NOMI_FASCE[FASCE]   = { "08-10", "10-12", "12-14", "14-16" };
const char *NOMI_GIORNI[GIORNI] = { "Lunedi", "Martedi", "Mercoledi", "Giovedi", "Venerdi" };


static void registra_studente(TabellaHash *h, const char *matricola);
static void menu_studente(Posto aula[GIORNI][FASCE][POSTI],
                          Coda *coda, TabellaHash *h,
                          StatoSistema *stato, const char *matricola);


int main(void)
{
    /* --- Strutture globali --- */
    static Posto      aula[GIORNI][FASCE][POSTI];
    TabellaHash       hash;
    Coda              coda;
    StatoSistema      stato;

    /* --- Inizializzazione --- */
    aula_inizializza(aula);
    hash_inizializza(&hash);
    coda_inizializza(&coda);
    operatore_stato_inizializza(&stato);

    /* --- Caricamento persistente --- */
    salvataggio_carica_tutto(&hash, aula, &stato, &coda);

    printf("========================================\n");
    printf("   Sistema Gestione Aula Studio\n");
    printf("========================================\n\n");

    char matricola[MAX_MATRICOLA];


    int continua = 1;
    while (continua) {

        printf("Inserisci la tua matricola (0 per uscire): ");
        if (scanf("%5s", matricola) != 1) {
            break;
        }

        
        if (strcmp(matricola, "0") == 0) {
            continua = 0;
            break;
        }

        
        if (strcmp(matricola, MATRICOLA_OPERATORE) == 0) {
            printf("\nAccesso operatore. Giorno: %s, Fascia: %s\n\n",
                   NOMI_GIORNI[stato.giorno_attuale],
                   NOMI_FASCE[stato.fascia_attuale]);
            operatore_menu(aula, &coda, &hash, &stato, PATH_STORICO);

        } else {
            
            Studente *s = hash_cerca(&hash, matricola);

            if (s == NULL) {
                printf("\nMatricola %s non trovata. Avvio registrazione...\n", matricola);
                registra_studente(&hash, matricola);
                s = hash_cerca(&hash, matricola);
            }

            if (s != NULL) {
                printf("\nBenvenuto, %s! Giorno: %s, Fascia: %s\n\n",
                       s->nome,
                       NOMI_GIORNI[stato.giorno_attuale],
                       NOMI_FASCE[stato.fascia_attuale]);
                menu_studente(aula, &coda, &hash, &stato, matricola);
            }
        }

        
        salvataggio_salva_tutto(&hash, aula, &stato, &coda);

        printf("\n--- Sessione terminata ---\n\n");
    }


    printf("\nGenerazione report finale...\n");
    report_genera(aula, &coda, PATH_STORICO);

    salvataggio_salva_tutto(&hash, aula, &stato, &coda);


    hash_distruggi(&hash);
    coda_distruggi(&coda);

    printf("Arrivederci.\n");
    return 0;
}


static void registra_studente(TabellaHash *h, const char *matricola)
{
    char nome[MAX_NOME];
    char corso[MAX_CORSO];

    printf("--- Registrazione nuovo studente ---\n");
    printf("Nome e cognome: ");
    scanf(" %49[^\n]", nome);
 
    printf("Corso di laurea: ");
    scanf(" %49[^\n]", corso);

    int esito = hash_inserisci(h, matricola, nome, corso);
    if (esito == 1) {
        storico_scrivi(PATH_STORICO, matricola, OP_REGISTRAZIONE, -1, -1, -1);
        printf("Registrazione completata. Benvenuto, %s!\n\n", nome);
    } else {
        printf("Errore di memoria durante la registrazione.\n");
    }
}

static void menu_studente(Posto aula[GIORNI][FASCE][POSTI],
                          Coda *coda, TabellaHash *h,
                          StatoSistema *stato, const char *matricola)
{
    int scelta;
    int g = stato->giorno_attuale;
    int f = stato->fascia_attuale;

    do {
        printf("--- Menu Studente (%s) ---\n", matricola);
        printf("1. Prenota un posto (fascia corrente)\n");
        printf("2. Check-in (ho una prenotazione)\n");
        printf("3. Walk-in (entro senza prenotazione)\n");
        printf("4. Check-out (esco dall'aula)\n");
        printf("5. Annulla prenotazione\n");
        printf("6. Vedi mappa aula\n");
        printf("7. Vedi posti liberi\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

    
        g = stato->giorno_attuale;
        f = stato->fascia_attuale;

        switch (scelta) {

            case 1: {
        
                Studente *s = hash_cerca(h, matricola);
                if (s && s->in_aula) {
                    printf("Sei gia' presente in aula.\n");
                    break;
                }
                int posto_assegnato;
                int esito = aula_prenota(aula, g, f, matricola, &posto_assegnato);
                if (esito == 1) {
                    printf("Prenotazione effettuata: posto %d, fascia %s, %s.\n",
                           posto_assegnato, NOMI_FASCE[f], NOMI_GIORNI[g]);
                    storico_scrivi(PATH_STORICO, matricola, OP_PRENOTAZIONE,
                                   g, f, posto_assegnato);
                } else {
                    printf("Nessun posto libero. Vuoi metterti in lista d'attesa? (1=si / 0=no): ");
                    int risp;
                    scanf("%d", &risp);
                    if (risp == 1) {
                        coda_inserisci(coda, matricola);
                        storico_scrivi(PATH_STORICO, matricola, OP_CODA_ATTESA, g, f, -1);
                        printf("Aggiunto alla lista d'attesa.\n");
                    }
                }
                break;
            }

            case 2: {
                int esito = aula_checkin(aula, g, f, matricola);
                if (esito == 1) {
                    int posto = aula_trova_posto(aula, g, f, matricola);
                    hash_aggiorna_presenza(h, matricola, 1);
                    hash_aggiorna_giorno(h, matricola, g);
                    storico_scrivi(PATH_STORICO, matricola, OP_CHECKIN, g, f, posto);
                    printf("Check-in effettuato. Buono studio!\n");
                } else {
                    printf("Nessuna prenotazione attiva trovata per questa fascia.\n");
                }
                break;
            }

            case 3: {
                Studente *s = hash_cerca(h, matricola);
                if (s && s->in_aula) {
                    printf("Sei gia' presente in aula.\n");
                    break;
                }
                int posto_assegnato;
                int esito = aula_walkin(aula, g, f, matricola, &posto_assegnato);
                if (esito == 1) {
                    hash_aggiorna_presenza(h, matricola, 1);
                    hash_aggiorna_giorno(h, matricola, g);
                    storico_scrivi(PATH_STORICO, matricola, OP_WALKIN,
                                   g, f, posto_assegnato);
                    printf("Walk-in: posto %d assegnato. Buono studio!\n", posto_assegnato);
                } else {
                    printf("Nessun posto libero. Vuoi metterti in lista d'attesa? (1=si / 0=no): ");
                    int risp;
                    scanf("%d", &risp);
                    if (risp == 1) {
                        coda_inserisci(coda, matricola);
                        storico_scrivi(PATH_STORICO, matricola, OP_CODA_ATTESA, g, f, -1);
                        printf("Aggiunto alla lista d'attesa.\n");
                    }
                }
                break;
            }

            case 4: {
                int posto = aula_checkout(aula, g, f, matricola);
                if (posto >= 0) {
                    hash_aggiorna_presenza(h, matricola, 0);
                    hash_aggiorna_giorno(h, matricola, -1);
                    storico_scrivi(PATH_STORICO, matricola, OP_CHECKOUT, g, f, posto);
                    printf("Check-out effettuato dal posto %d. Arrivederci!\n", posto);

                } else {
                    printf("Non risulti presente in aula in questa fascia.\n");
                }
                break;
            }

            case 5: {
                int posto = aula_annulla_prenotazione(aula, g, f, matricola);
                if (posto >= 0) {
                    storico_scrivi(PATH_STORICO, matricola, OP_ANNULLAMENTO, g, f, posto);
                    printf("Prenotazione annullata (posto %d liberato).\n", posto);

                } else {
                    printf("Nessuna prenotazione attiva trovata per questa fascia.\n");
                }
                break;
            }

            case 6:
                aula_stampa_mappa(aula, g, f);
                break;

            case 7:
                printf("Posti liberi in fascia %s (%s): %d\n",
                       NOMI_FASCE[f], NOMI_GIORNI[g],
                       aula_posti_liberi(aula, g, f));
                break;

            case 0:
                printf("Uscita dal menu studente.\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }

    } while (scelta != 0);
}