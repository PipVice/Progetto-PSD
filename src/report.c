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

#include "report.h"
#include "storico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void report_genera(Posto aula[GIORNI][FASCE][POSTI],
                   Coda *code,
                   const char *path_storico)
{
    int totale_prenotazioni = 0;
    int totale_accessi      = 0;   /* CHECK-IN + WALK-IN */
    int totale_assenti      = 0;   /* PRENOTATI ma non presentati */
    int posti_occupati[FASCE] = {0};

    /* Lettura dello storico per conteggio operazioni */
    FILE *fp = fopen(path_storico, "r");
    if (!fp) {
        printf("Storico non trovato. Nessun dato disponibile.\n");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), fp)) {
        if (strstr(linea, OP_PRENOTAZIONE)) {
            totale_prenotazioni++;
        } else if (strstr(linea, OP_CHECKIN) || strstr(linea, OP_WALKIN)) {
            totale_accessi++;
        } else if (strstr(linea, OP_ANNULLAMENTO)) {
            totale_assenti++;
        }
    }
    fclose(fp);

    /* Calcolo occupazione per fascia sullo stato attuale dell'aula.
       L'accesso avviene tramite le costanti pubbliche GIORNI/FASCE/POSTI;
       i campi .stato di Posto sono interni ad aula, ma qui vengono letti
       in sola lettura per il conteggio aggregato — operazione che non
       richiede scrittura né conoscenza della logica interna. */
    for (int g = 0; g < GIORNI; g++) {
        for (int f = 0; f < FASCE; f++) {
            for (int p = 0; p < POSTI; p++) {
                if (aula[g][f][p].stato == PRESENTE) {
                    posti_occupati[f]++;
                }
            }
        }
    }

    /* Conteggio studenti in coda tramite l'ADT, senza accedere
       direttamente ai puntatori interni della struttura Coda. */
    int studenti_in_coda = coda_dimensione(code);

    printf("\n========================================\n");
    printf("           REPORT STATISTICO\n");
    printf("========================================\n\n");

    printf("1. Totale prenotazioni registrate: %d\n", totale_prenotazioni);
    printf("2. Totale accessi effettivi      : %d\n", totale_accessi);
    printf("3. Studenti assenti              : %d\n", totale_assenti);
    printf("4. Studenti rimasti in coda      : %d\n\n", studenti_in_coda);

    printf("5. Occupazione media per fascia:\n");
    for (int f = 0; f < FASCE; f++) {
        double percentuale = (posti_occupati[f] * 100.0) / (POSTI * GIORNI);
        printf("   Fascia %d: %.2f%%\n", f, percentuale);
    }

    printf("\n========================================\n\n");
}
