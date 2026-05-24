/*
  File: report.c
  Autore: [Emanuel Sessa]
  Data:   [20/05/2026]

  Descrizione:
  Implementazione della funzione per la generazione del report
  statistico finale. Analizza il file di storico e le strutture
  dati correnti per produrre le statistiche richieste.

  Tutte le informazioni sulla struttura interna della coda vengono
  ottenute tramite coda_dimensione(), rispettando l'information
  hiding dell'ADT Coda.
*/

#include "../include/report.h"
#include "../include/storico.h"
#include "../include/coda.h"
#include "../include/operatore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void report_genera(Coda *code,
                   const char *path_storico)
{
    int totale_prenotazioni    = 0;
    int totale_accessi         = 0;          
    int totale_assenti         = 0;          
    int studenti_in_coda       = 0;

    int accessi_per_fascia[FASCE] = {0};

    FILE *fp = fopen(path_storico, "r");
    if (!fp) {
        printf("Storico non trovato. Nessun dato disponibile.\n");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), fp)) {

        if (strstr(linea, OP_PRENOTAZIONE)) {
            totale_prenotazioni++;
        }
        else if (strstr(linea, OP_CHECKIN) || strstr(linea, OP_WALKIN)
                                           || strstr(linea, OP_CODA_ENTRATA)) {
            totale_accessi++;

            /*
                Estrae l'indice di fascia dalla riga di log.
                Formato atteso: "[G<g> F<f>] ..."
                Si cerca il carattere 'F' seguito da una cifra dopo la '['.
             */
            char *pf = strstr(linea, " F");
            if (pf != NULL) {
                int fascia = -1;
                if (sscanf(pf, " F%d", &fascia) == 1) {
                    if (fascia >= 0 && fascia < FASCE) {
                        accessi_per_fascia[fascia]++;
                    }
                }
            }
        }
        else if (strstr(linea, OP_ANNULLAMENTO)) {
            totale_assenti++;
        }
    }

    fclose(fp);

    
     studenti_in_coda = coda_dimensione(code);

    int capacita_fascia = POSTI * GIORNI;

    printf("\n========================================\n");
    printf("           REPORT STATISTICO\n");
    printf("========================================\n\n");

    printf("1. Totale prenotazioni registrate: %d\n", totale_prenotazioni);
    printf("2. Totale accessi effettivi      : %d\n", totale_accessi);
    printf("3. Studenti assenti              : %d\n", totale_assenti);
    printf("4. Studenti rimasti in coda      : %d\n\n", studenti_in_coda);

    printf("5. Occupazione storica per fascia (include chi ha gia' fatto check-out):\n");
    for (int f = 0; f < FASCE; f++) {
        double percentuale = (accessi_per_fascia[f] * 100.0) / capacita_fascia;
        printf("   Fascia %d (%s): %d accessi — %.2f%%\n",
               f, NOMI_FASCE[f], accessi_per_fascia[f], percentuale);
    }

    printf("\n========================================\n\n");
}