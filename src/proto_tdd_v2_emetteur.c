#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"


/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message de l'application */
    int taille_msg;                  /* taille du message */
    paquet_t pdata;                  /* paquet utilisé par le protocole */
    paquet_t pack;
    int prochain_paquet;
    int evt;

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_msg);
    prochain_paquet = 0;

    /* tant que l'émetteur a des données à envoyer */
    while ( taille_msg != 0 ) {

        /* construction paquet */
        for (int i=0; i<taille_msg; i++) {
            pdata.info[i] = message[i];
        }
        pdata.lg_info = taille_msg;
        pdata.type = DATA;
        pdata.numseq = prochain paquet;
        pdata.somme_ctrl = generer_controle(&pdata);
        /* remise à la couche reseau */
        vers_reseau(&pdata);
        depart_temporisateur();
        evt = attendre();
        de_reseau(&pack);
        while(evt ==  ){
            vers_reseau(&pdata);
            depart_temporisateur();
            evt = attendre();
        }
        de_reseau(&pack);
        arreter_temporisateur();
        prochain_paquet = inc() 
        blabla
        /* lecture des donnees suivantes de la couche application */
        de_application(message, &taille_msg);
    }

    printf("[TR] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
