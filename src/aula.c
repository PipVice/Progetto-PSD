/*
  File: aula.c
  Autore: [Emanuel Sessa]
  Data:   [13/05/2026]

  Descrizione:
  Implementazione delle funzioni per la gestione dei posti nell'aula studio.
  Consente di gestire prenotazioni, check-in, walk-in, check-out e
  annullamenti di prenotazioni su una matrice tridimensionale di posti.
*/

#include <stdio.h>
#include "../include/aula.h"
#include <string.h>

void aula_inizializza(Posto aula[GIORNI][FASCE][POSTI]) {
    for(int i = 0; i < GIORNI; i++) {
        for (int j = 0; j < FASCE; j++) {
            for (int k = 0; k < POSTI; k++) {
                aula[i][j][k].stato = LIBERO;
                strcpy(aula[i][j][k].matricola, "");
            }   
        }
    }
}

int aula_prenota(Posto aula[GIORNI][FASCE][POSTI], int g, int f, const char *matricola, int *posto_assegnato) {
    for (int k = 0; k < POSTI; k++) {
        if (aula[g][f][k].stato == LIBERO) {
            aula[g][f][k].stato = PRENOTATO; 
            strcpy(aula[g][f][k].matricola, matricola);
            *posto_assegnato = k; 
            return 1; 
        }
    }
    return 0; 
}

int aula_checkin(Posto aula[GIORNI][FASCE][POSTI], int g, int f, const char *matricola) {
    for(int i = 0; i < POSTI; i++) {
        if(strcmp(aula[g][f][i].matricola, matricola) == 0 && aula[g][f][i].stato == PRENOTATO) {
            aula[g][f][i].stato = PRESENTE;
            return 1;
        }
    }
    return 0;
}

int aula_walkin(Posto aula[GIORNI][FASCE][POSTI], int g, int f, const char *matricola, int *posto_assegnato) {
    for(int i = 0; i < POSTI; i++) {
        if(aula[g][f][i].stato == LIBERO) {
            strcpy(aula[g][f][i].matricola, matricola);
            aula[g][f][i].stato = PRESENTE;
            *posto_assegnato = i;
            return 1;
        }
    }
    return 0;
}

int aula_checkout(Posto aula[GIORNI][FASCE][POSTI], int g, int f, const char *matricola) {
    for (int i = 0; i < POSTI; i++) {
        if (aula[g][f][i].stato == PRESENTE && strcmp(aula[g][f][i].matricola, matricola) == 0) {
            aula[g][f][i].stato = LIBERO;
            strcpy(aula[g][f][i].matricola, "");
            return i;
        }
    }
    return -1;
}

int aula_annulla_prenotazione(Posto aula[GIORNI][FASCE][POSTI], int g, int f, const char *matricola) {
    for (int i = 0; i < POSTI; i++) {
        if (aula[g][f][i].stato == PRENOTATO && strcmp(aula[g][f][i].matricola, matricola) == 0) {
            aula[g][f][i].stato = LIBERO;
            strcpy(aula[g][f][i].matricola, "");
            return i;
        }
    }
    return -1;
}

int aula_posti_liberi(Posto aula[GIORNI][FASCE][POSTI], int g, int f) {
    int conteggio = 0;
    for (int k = 0; k < POSTI; k++) {
        if (aula[g][f][k].stato == LIBERO) {
            conteggio++;
        }
    }
    return conteggio;
}

void aula_stampa_mappa(Posto aula[GIORNI][FASCE][POSTI], int g, int f) {
    printf("\n--- Mappa Aula [Giorno %d - Fascia %d] ---\n\n", g, f);

    for (int k = 0; k < POSTI; k++) {
        if (aula[g][f][k].stato == LIBERO) {
            printf(" + ");
        } else if (aula[g][f][k].stato == PRENOTATO) {
            printf(" P ");
        } else if (aula[g][f][k].stato == PRESENTE) {
            printf(" * ");
        }

        /* Va a capo ogni 6 posti per creare una griglia ordinata 5x6 */
        if ((k + 1) % 6 == 0) {
            printf("\n");
        }
    }
    printf("\nLegenda: [+] Libero  [P] Prenotato  [*] Presente\n");
}
