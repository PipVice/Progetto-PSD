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

void operatore_menu(Posto        aula[GIORNI][FASCE][POSTI],
                    Coda         code[GIORNI][FASCE],
                    TabellaHash *h,
                    StatoSistema *stato,
                    const char  *path_storico)
{
    int scelta;
    do{
        printf("1. Check-out fascia\n");
        printf("2, Visualizza storico\n");
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


void operatore_checkout_fascia(Posto        aula[GIORNI][FASCE][POSTI],
                                Coda         code[GIORNI][FASCE],
                                TabellaHash *h,
                                StatoSistema *stato,
                                const char  *path_storico)
{
    int giorno = stato->giorno_attuale;
    int fascia = stato->fascia_attuale;
    for(int i=0; i<POSTI; i++){
        if(aula[giorno][fascia][i].stato == 1){
            char *matricola = aula[giorno][fascia][i].matricola;
            storico_scrivi(path_storico, matricola, OP_CHECKOUT, giorno, fascia, i);
            aula[giorno][fascia][i].stato = 0;
            aula[giorno][fascia][i].matricola[0] = '\0';
        }
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
