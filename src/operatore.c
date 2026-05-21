/*
File: operatore.c
  Autore: [Giuseppe Pio Vicedomini]
  Data:   [16/05/2026]
 
  Descrizione:
  Implementazione delle funzioni del menu operatore. Gestisce il
  check-out della fascia corrente, la visualizzazione dello storico
  e l'avanzamento del giorno di sistema.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/operatore.h"


void operatore_menu(Posto aula[GIORNI][FASCE][POSTI],
                    Coda *code,
                    TabellaHash *h,
                    StatoSistema *stato,
                    const char *path_storico)
{
    int scelta;

    do {
        printf("1. Check-out fascia\n");
        printf("2. Visualizza storico\n");
        printf("3. Avanza giorno\n");
        printf("0. Esci\n");
        printf("Scelta: ");

        if (scanf("%d", &scelta) != 1) {
            while (getchar() != '\n');
            printf("Input non valido.\n");
            continue;
        }

        switch (scelta) {
            case 1:
                operatore_checkout_fascia(aula, code, h, stato, path_storico);
                break;

            case 2:
                operatore_visualizza_storico(path_storico);
                break;

            case 3:
                if (operatore_avanza_giorno(stato, code)) {
                    printf("Giorno avanzato: %s\n", NOMI_GIORNI[stato->giorno_attuale]);
                } else {
                    printf("Fine settimana raggiunta. Usa reset o riavvia il sistema.\n");
                }
                break;

            case 0:
                printf("Uscita dal menu operatore.\n");
                break;

            default:
                printf("Scelta non valida.\n");
                break;
        }

    } while (scelta != 0);
}

void operatore_checkout_fascia(Posto aula[GIORNI][FASCE][POSTI],
                               Coda *code,
                               TabellaHash *h,
                               StatoSistema *stato,
                               const char *path_storico)
{
    int giorno = stato->giorno_attuale;
    int fascia = stato->fascia_attuale;

    for (int i = 0; i < POSTI; i++) {
        int stato_posto = aula[giorno][fascia][i].stato;

        if (stato_posto == PRENOTATO || stato_posto == PRESENTE) {
            char *matricola = aula[giorno][fascia][i].matricola;

            if (stato_posto == PRESENTE) {
                storico_scrivi(path_storico, matricola, OP_CHECKOUT, giorno, fascia, i);
                hash_aggiorna_presenza(h, matricola, 0);
                hash_aggiorna_giorno(h, matricola, -1);
            } else {
                storico_scrivi(path_storico, matricola, OP_ANNULLAMENTO, giorno, fascia, i);
            }

            aula[giorno][fascia][i].stato = LIBERO;
            aula[giorno][fascia][i].matricola[0] = '\0';
        }
    }

    if (fascia < FASCE - 1) {
        int fascia_succ = fascia + 1;
        char matricola[MAX_MATRICOLA];

        for (int posto = 0; posto < POSTI && !coda_vuota(code); posto++) {
            if (aula[giorno][fascia_succ][posto].stato == LIBERO) {
                if (coda_estrai(code, matricola)) {
                    strcpy(aula[giorno][fascia_succ][posto].matricola, matricola);
                    aula[giorno][fascia_succ][posto].stato = PRESENTE;
                    hash_aggiorna_presenza(h, matricola, 1);
                    hash_aggiorna_giorno(h, matricola, giorno);
                    storico_scrivi(path_storico, matricola, OP_CODA_ENTRATA,
                                   giorno, fascia_succ, posto);
                }
            }
        }

        stato->fascia_attuale++;
        printf("Fascia conclusa. Avanzata alla fascia %s.\n",
               NOMI_FASCE[stato->fascia_attuale]);

    } else {
        printf("Ultima fascia del giorno conclusa.\n");
        printf("Gli studenti ancora in coda non possono entrare oggi.\n");

        while (!coda_vuota(code)) {
            char matricola[MAX_MATRICOLA];
            if (coda_estrai(code, matricola)) {
                storico_scrivi(path_storico, matricola, OP_ANNULLAMENTO,
                               giorno, fascia, -1);
            }
        }
    }
}

void operatore_visualizza_storico(const char *path_storico)
{
    storico_stampa(path_storico);
}

void operatore_stato_inizializza(StatoSistema *stato)
{
    stato->giorno_attuale = 0;
    stato->fascia_attuale = 0;
}

int operatore_avanza_giorno(StatoSistema *stato, Coda *code)
{
    if (stato->giorno_attuale < GIORNI - 1) {
        stato->giorno_attuale++;
        stato->fascia_attuale = 0;
        coda_distruggi(code);
        coda_inizializza(code);
        return 1;
    }

    return 0;
}
