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


void operatore_menu(Posto aula[GIORNI][FASCE][POSTI],Coda *code,TabellaHash *h,StatoSistema *stato,const char *path_storico){
    int scelta;
    do{
        printf("1. Check-out fascia\n");
        printf("2. Visualizza storico\n");
        printf("3. Avanza giorno\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        switch(scelta){
            case 1:
                operatore_checkout_fascia(aula, code, h, stato, path_storico);
                break;
            case 2:
                operatore_visualizza_storico(path_storico);
                break;
            case 3:
                if(operatore_avanza_giorno(stato)){
                    printf("Giorno avanzato: %s\n", NOMI_GIORNI[stato->giorno_attuale]);
                }else{
                    printf("Fine settimana raggiunta. Reset del sistema necessario.\n");
                }
                break;
            case 0:
                printf("Uscita dal menu operatore.\n");
                break;
            default:
                printf("Scelta non valida. Riprova.\n");
        }   
    }while(scelta!=0);
}

static void operatore_assegna_coda_fascia_successiva(Posto aula[GIORNI][FASCE][POSTI],
                                                     Coda *code,
                                                     const char *path_storico,
                                                     int giorno,
                                                     int fascia)
{
    int fascia_successiva = fascia + 1;
    if (fascia_successiva >= FASCE) {
        return;
    }

    char matricola[MAX_MATRICOLA];
    for (int posto = 0; posto < POSTI && !coda_vuota(code); posto++) {
        if (aula[giorno][fascia_successiva][posto].stato == LIBERO) {
            if (coda_estrai(code, matricola)) {
                strcpy(aula[giorno][fascia_successiva][posto].matricola, matricola);
                aula[giorno][fascia_successiva][posto].stato = PRESENTE;
                storico_scrivi(path_storico,
                               matricola,
                               OP_CODA_ENTRATA,
                               giorno,
                               fascia_successiva,
                               posto);
            }
        }
    }
}

void operatore_checkout_fascia(Posto aula[GIORNI][FASCE][POSTI],Coda *code,TabellaHash *h,StatoSistema *stato,const char  *path_storico){
    int giorno = stato->giorno_attuale;
    int fascia = stato->fascia_attuale;

    for(int i=0; i<POSTI; i++){
        int stato_posto = aula[giorno][fascia][i].stato;
        if(stato_posto == PRENOTATO || stato_posto == PRESENTE){
            char *matricola = aula[giorno][fascia][i].matricola;

            if(stato_posto == PRESENTE){
                storico_scrivi(path_storico, matricola, OP_CHECKOUT, giorno, fascia, i);
                hash_aggiorna_presenza(h, matricola, 0);
            } else {
                storico_scrivi(path_storico, matricola, OP_ANNULLAMENTO, giorno, fascia, i);
            }

            aula[giorno][fascia][i].stato = LIBERO;
            aula[giorno][fascia][i].matricola[0] = '\0';
        }
    }

    if (fascia < FASCE - 1) {
        operatore_assegna_coda_fascia_successiva(aula, code, path_storico, giorno, fascia);  
        stato->fascia_attuale++;
        printf("Fascia conclusa. Avanzata alla fascia %d.\n", stato->fascia_attuale);
    } else {
        printf("Fascia conclusa. Sei all'ultima fascia del giorno. Usa 'Avanza giorno' per passare al giorno successivo.\n");
    }
}

void operatore_visualizza_storico(const char *path_storico){
    FILE *file = fopen(path_storico, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file storico.\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    if(ftell(file) == 0){
        printf("Il file storico è vuoto.\n");
        fclose(file);
        return;
    }
    rewind(file);
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file)){
        printf("%s", buffer);
    }
    fclose(file);
}

void operatore_stato_inizializza(StatoSistema *stato){
    stato->giorno_attuale = 0;
    stato->fascia_attuale = 0;
}

int operatore_avanza_giorno(StatoSistema *stato){
    if(stato->giorno_attuale < GIORNI - 1){
        stato->giorno_attuale++;
        stato->fascia_attuale = 0;
        return 1; // Giorno avanzato con successo
    }
    return 0; // Fine settimana raggiunta
}
