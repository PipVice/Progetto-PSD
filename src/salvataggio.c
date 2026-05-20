/*
  File: salvataggio.h
  Autore: [Emanuel Sessa]
  Data:   [19/05/2026]

  Descrizione:
  Funzioni per il salvataggio e caricamento persistente dello stato del
  sistema tra un'esecuzione e l'altra. Le strutture salvate sono:

    - Tabella hash degli studenti
    - Aula (solo posti non liberi)
    - Stato del sistema (giorno/fascia)
    - Coda unica di attesa

  File utilizzati:
    studenti.dat  → matricola;nome;corso;in_aula;giorno_accesso
    aula.dat      → g;f;p;matricola;stato
    stato.dat     → giorno_attuale;fascia_attuale
    coda.dat      → una matricola per riga (FIFO)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/salvataggio.h"

/* Salva tutti gli studenti */
int salvataggio_salva_studenti(TabellaHash *h, const char *path){
    if (!h || !path) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        Studente *corr = h->bucket[i];
        while (corr) {
            fprintf(file, "%s;%s;%s;%d;%d\n",
                    corr->matricola,
                    corr->nome,
                    corr->corso,
                    corr->in_aula,
                    corr->giorno_accesso);
            corr = corr->next;
        }
    }

    fclose(file);
    return 1;
}

/* Salva solo i posti non liberi dell'aula */
int salvataggio_salva_aula(Posto aula[GIORNI][FASCE][POSTI], const char *path){
    if (!aula || !path) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    for (int g = 0; g < GIORNI; g++) {
        for (int f = 0; f < FASCE; f++) {
            for (int p = 0; p < POSTI; p++) {
                if (aula[g][f][p].stato != LIBERO) {
                    fprintf(file, "%d;%d;%d;%s;%d\n",
                            g, f, p,
                            aula[g][f][p].matricola,
                            aula[g][f][p].stato);
                }
            }
        }
    }

    fclose(file);
    return 1;
}

/* Salva giorno e fascia attuale */
int salvataggio_salva_stato(StatoSistema *stato, const char *path){
    if (!stato || !path) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    fprintf(file, "%d;%d\n", stato->giorno_attuale, stato->fascia_attuale);

    fclose(file);
    return 1;
}

/* Salva la coda unica */
int salvataggio_salva_coda(Coda *c, const char *path){
    if (!c || !path) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    NodoCoda *curr = c->testa;
    while (curr) {
        fprintf(file, "%s\n", curr->matricola);
        curr = curr->next;
    }

    fclose(file);
    return 1;
}

/* Carica studenti */
int salvataggio_carica_studenti(TabellaHash *h, const char *path){
    if (!h || !path) {
        return 0;
    }

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            continue;
        }

        char *matricola = strtok(buffer, ";\n");
        char *nome = strtok(NULL, ";\n");
        char *corso = strtok(NULL, ";\n");
        char *in_aula_str = strtok(NULL, ";\n");
        char *giorno_str = strtok(NULL, ";\n");

        if (!matricola || !nome || !corso || !in_aula_str || !giorno_str) {
            continue;
        }

        int in_aula = atoi(in_aula_str);
        int giorno = atoi(giorno_str);

        Studente *s = hash_cerca(h, matricola);
        if (!s) {
            if (hash_inserisci(h, matricola, nome, corso) != 1) {
                continue;
            }
            s = hash_cerca(h, matricola);
        }

        hash_aggiorna_presenza(h, matricola, in_aula);
        hash_aggiorna_giorno(h, matricola, giorno);
    }

    fclose(file);
    return 1;
}

/* Carica aula (solo posti non liberi) */
int salvataggio_carica_aula(Posto aula[GIORNI][FASCE][POSTI], const char *path){
    if (!aula || !path) {
        return 0;
    }

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            continue;
        }

        char *g_str = strtok(buffer, ";\n");
        char *f_str = strtok(NULL, ";\n");
        char *p_str = strtok(NULL, ";\n");
        char *matricola = strtok(NULL, ";\n");
        char *stato_str = strtok(NULL, ";\n");

        if (!g_str || !f_str || !p_str || !matricola || !stato_str) {
            continue;
        }

        int g = atoi(g_str);
        int f = atoi(f_str);
        int p = atoi(p_str);
        int stato = atoi(stato_str);

        if (g < 0 || g >= GIORNI || f < 0 || f >= FASCE || p < 0 || p >= POSTI) {
            continue;
        }

        aula[g][f][p].stato = stato;
        strncpy(aula[g][f][p].matricola, matricola, MAX_MATRICOLA - 1);
        aula[g][f][p].matricola[MAX_MATRICOLA - 1] = '\0';
    }

    fclose(file);
    return 1;
}

/* Carica stato */
int salvataggio_carica_stato(StatoSistema *stato, const char *path){
    if (!stato || !path) {
        return 0;
    }

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    int g, f;
    int read = fscanf(file, "%d;%d", &g, &f);
    fclose(file);

    if (read != 2) {
        return 0;
    }

    if (g < 0 || g >= GIORNI || f < 0 || f >= FASCE) {
        return 0;
    }

    stato->giorno_attuale = g;
    stato->fascia_attuale = f;

    return 1;
}

/* Carica coda unica */
int salvataggio_carica_coda(Coda *c, const char *path){
    if (!c || !path) {
        return 0;
    }

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    char buffer[64];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (buffer[0] == '\0') {
            continue;
        }
        coda_inserisci(c, buffer);
    }

    fclose(file);
    return 1;
}

/* Salvataggio completo */
int salvataggio_salva_tutto(TabellaHash *h,
                            Posto aula[GIORNI][FASCE][POSTI],
                            StatoSistema *stato,
                            Coda *coda){
    if (!h || !aula || !stato || !coda) {
        return 0;
    }

    int verifica_studenti = salvataggio_salva_studenti(h, PATH_STUDENTI);
    int verifica_aula = salvataggio_salva_aula(aula, PATH_AULA);
    int verifica_stato = salvataggio_salva_stato(stato, PATH_STATO);
    int verifica_coda = salvataggio_salva_coda(coda, PATH_CODA);

    if (verifica_studenti && verifica_aula && verifica_stato && verifica_coda) {
        return 1;
    }
    return 0;
}

/* Caricamento completo */
int salvataggio_carica_tutto(TabellaHash *h,
                             Posto aula[GIORNI][FASCE][POSTI],
                             StatoSistema *stato,
                             Coda *coda){
    if (!h || !aula || !stato || !coda) {
        return 0;
    }

    int verifica_studenti = salvataggio_carica_studenti(h, PATH_STUDENTI);
    int verifica_aula = salvataggio_carica_aula(aula, PATH_AULA);
    int verifica_stato = salvataggio_carica_stato(stato, PATH_STATO);
    int verifica_coda = salvataggio_carica_coda(coda, PATH_CODA);

    if (verifica_studenti || verifica_aula || verifica_stato || verifica_coda) {
        return 1;
    }
    return 0;
}
