/*
  File: storico.c
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [16/05/2026]

  Descrizione:
  Implementazione delle funzioni per la gestione del file di log delle
  operazioni del sistema. Consente di registrare ogni operazione
  significativa in modalità append e di consultare lo storico completo
  delle azioni effettuate.
*/

#include "storico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_LOG "storico.log"


void storico_scrivi(const char *path,
                    const char *matricola,
                    const char *operazione,
                    int         giorno,
                    int         fascia,
                    int         posto)
{
    FILE *fp;
    
    if (path == NULL || matricola == NULL || operazione == NULL) {
        fprintf(stderr, "Errore: storico_scrivi ricevuto NULL pointer\n");
        return;
    }
    
    fp = fopen(path, "a");
    if (fp == NULL) {
        fprintf(stderr, "Errore: impossibile aprire il file %s\n", path);
        return;
    }
    
    /* Giorno e fascia non sempre applicabili (es. REGISTRAZIONE) */
    if (giorno != -1 && fascia != -1) {
        fprintf(fp, "[G%d F%d] %s  %s", giorno, fascia, matricola, operazione);
    } else {
        fprintf(fp, "%s  %s", matricola, operazione);
    }

    if (posto != -1) {
        fprintf(fp, "  posto %d", posto);
    }
    
    fprintf(fp, "\n");
    
    fclose(fp);
}


void storico_stampa(const char *path)
{
    FILE *fp;
    char linea[256];
    
    if (path == NULL) {
        fprintf(stderr, "Errore: storico_stampa ricevuto NULL pointer\n");
        return;
    }
    
    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Avviso: file %s non trovato (storico vuoto)\n", path);
        return;
    }
    
    printf("\n========== STORICO OPERAZIONI ==========\n");
    
    /* Lettura sequenziale del file, riga per riga */
    while (fgets(linea, sizeof(linea), fp) != NULL) {
        printf("%s", linea);
    }
    
    printf("========================================\n\n");
    
    fclose(fp);
}