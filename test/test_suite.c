/*
  File: test_suite.c
  Descrizione: Suite di test automatici per il sistema "Gestione Aula Studio".
               Copre i casi di test richiesti dalla traccia 2:
                 1. Verifica della registrazione degli studenti
                 2. Test dell'inserimento delle prenotazioni
                 3. Verifica della disponibilita' dei posti
                 4. Test del check-in e check-out
                 5. Verifica dell'ingresso senza prenotazione
                 6. Test della lista di attesa
                 7. Verifica dell'annullamento e aggiornamento disponibilita'
                 8. Test dello storico e dei report
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "aula.h"
#include "hash.h"
#include "coda.h"
#include "storico.h"
#include "report.h"

#define TEST_LOG "test_storico.log"

TabellaHash hash;
Posto aula[GIORNI][FASCE][POSTI];
Coda coda;


void setup() {
    hash_inizializza(&hash);
    aula_inizializza(aula);
    coda_inizializza(&coda);
    remove(TEST_LOG); 
}

void test_registrazione_studenti() {
    printf("Esecuzione: Verifica della registrazione degli studenti...\n");
    
    int esito = hash_inserisci(&hash, "11111", "Mario Rossi", "Informatica");
    assert(esito == 1);
    
    esito = hash_inserisci(&hash, "11111", "Mario Clone", "Fisica");
    assert(esito == 0);

    Studente *s = hash_cerca(&hash, "11111");
    assert(s != NULL);
    assert(strcmp(s->nome, "Mario Rossi") == 0);
    assert(s->in_aula == 0); 
    
    printf("  -> SUPERATO\n");
}

void test_prenotazioni_e_disponibilita() {
    printf("Esecuzione: Test inserimento prenotazioni e disponibilita' posti...\n");
    
    int g = 0, f = 0, posto;
    
    assert(aula_posti_liberi(aula, g, f) == POSTI);
    
    int esito = aula_prenota(aula, g, f, "11111", &posto);
    assert(esito == 1);
    assert(posto == 0);
    
    assert(aula_posti_liberi(aula, g, f) == POSTI - 1);
    
    assert(aula_trova_posto(aula, g, f, "11111") == posto);
    
    printf("  -> SUPERATO\n");
}

void test_checkin_checkout() {
    printf("Esecuzione: Test del check-in e check-out...\n");
    
    int g = 0, f = 0;
    
    int esito = aula_checkin(aula, g, f, "11111");
    assert(esito == 1);
    
    assert(aula_checkin(aula, g, f, "99999") == 0);
    
    int posto_liberato = aula_checkout(aula, g, f, "11111");
    assert(posto_liberato == 0); 

    assert(aula_posti_liberi(aula, g, f) == POSTI);
    
    printf("  -> SUPERATO\n");
}

void test_walkin() {
    printf("Esecuzione: Verifica dell'ingresso senza prenotazione...\n");
    
    int g = 1, f = 1, posto;
    
    int esito = aula_walkin(aula, g, f, "22222", &posto);
    assert(esito == 1);
    
    assert(aula_trova_posto(aula, g, f, "22222") == posto);
    assert(aula_posti_liberi(aula, g, f) == POSTI - 1);
    
    aula_checkout(aula, g, f, "22222");
    
    printf("  -> SUPERATO\n");
}

void test_lista_attesa() {
    printf("Esecuzione: Test della lista di attesa...\n");

    int g = 3, f = 3, posto;

    assert(coda_vuota(&coda) == 1);
    assert(coda_dimensione(&coda) == 0);

    for (int i = 0; i < POSTI; i++) {
        char mat[MAX_MATRICOLA];
        sprintf(mat, "%05d", i + 1);
        int esito = aula_walkin(aula, g, f, mat, &posto);
        assert(esito == 1);
    }
    assert(aula_posti_liberi(aula, g, f) == 0);

    int esito = aula_walkin(aula, g, f, "33333", &posto);
    assert(esito == 0);

    coda_inserisci(&coda, "33333");
    coda_inserisci(&coda, "44444");
    assert(coda_vuota(&coda) == 0);
    assert(coda_dimensione(&coda) == 2);

    int p_liberato = aula_checkout(aula, g, f, "00001");
    assert(p_liberato >= 0);
    assert(aula_posti_liberi(aula, g, f) == 1);

    char estratto[MAX_MATRICOLA];
    esito = coda_estrai(&coda, estratto);
    assert(esito == 1);
    assert(strcmp(estratto, "33333") == 0);   

    esito = aula_walkin(aula, g, f, estratto, &posto);
    assert(esito == 1);                       
    assert(aula_posti_liberi(aula, g, f) == 0);

    assert(coda_dimensione(&coda) == 1);

    for (int i = 2; i <= POSTI; i++) {
        char mat[MAX_MATRICOLA];
        sprintf(mat, "%05d", i);
        aula_checkout(aula, g, f, mat);
    }
    aula_checkout(aula, g, f, "33333");

    coda_distruggi(&coda);
    coda_inizializza(&coda);
    assert(coda_vuota(&coda) == 1);

    printf("  -> SUPERATO\n");
}

void test_annullamento() {
    printf("Esecuzione: Verifica dell'annullamento e aggiornamento...\n");
    
    int g = 2, f = 2, posto;
    
    aula_prenota(aula, g, f, "55555", &posto);
    assert(aula_posti_liberi(aula, g, f) == POSTI - 1);
    
    int posto_liberato = aula_annulla_prenotazione(aula, g, f, "55555");
    assert(posto_liberato == posto);

    assert(aula_posti_liberi(aula, g, f) == POSTI);
    
    assert(aula_annulla_prenotazione(aula, g, f, "99999") == -1);

    int p2;
    aula_prenota(aula, g, f, "55555", &p2);
    assert(aula_ha_prenotazione_giorno(aula, g, "55555") == 1);
    assert(aula_ha_prenotazione_giorno(aula, g, "99999") == 0);
    aula_annulla_prenotazione(aula, g, f, "55555");
    assert(aula_ha_prenotazione_giorno(aula, g, "55555") == 0);

    printf("  -> SUPERATO\n");
}

void test_storico_report() {
    printf("Esecuzione: Test dello storico e dei report...\n");
    
    storico_scrivi(TEST_LOG, "11111", OP_PRENOTAZIONE, 0, 0, 0);
    storico_scrivi(TEST_LOG, "11111", OP_CHECKIN, 0, 0, 0);
    
    printf("\n--- Inizio output report simulato ---\n");
    report_genera(&coda, TEST_LOG);
    printf("--- Fine output report simulato ---\n");
    
    printf("  -> SUPERATO (Nessun crash durante I/O)\n");
}

int main() {
    printf("=== AVVIO SUITE DI TEST ===\n\n");
    
    setup();
    
    test_registrazione_studenti();
    test_prenotazioni_e_disponibilita();
    test_checkin_checkout();
    test_walkin();
    test_lista_attesa();
    test_annullamento();
    test_storico_report();
    
    printf("\n=== TUTTI I TEST SONO STATI SUPERATI CON SUCCESSO! ===\n");
    
    hash_distruggi(&hash);
    coda_distruggi(&coda);
    
    return 0;
}