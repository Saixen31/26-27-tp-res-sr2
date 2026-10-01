/*************************************************************
* proto_tdd_v0 -  récepteur                                  *
* TRANSFERT DE DONNEES  v0                                   *
*                                                            *
* Protocole sans contrôle de flux, sans reprise sur erreurs  *
*                                                            *
* Université de Toulouse / FSI / Dpt d'informatique          *
**************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t pdata;                  /* paquet reçu du réseau */
    int fin = 0;                     /* condition d'arrêt */
    paquet_t pack;                   /* paquet d'acquittement à envoyer */
    
    uint8_t seq_attendu = 0;         /* Variable indispensable pour la v2 */

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    while ( !fin ) {

        de_reseau(&pdata);
        
        if (pdata.type == DATA && verifier_controle(&pdata)) {
            
            if (pdata.num_seq == seq_attendu) {
                
                for (int i=0; i<pdata.lg_info; i++) {
                    message[i] = pdata.info[i];
                }
               
                fin = vers_application(message, pdata.lg_info);
                
                pack.type = ACK;
                pack.num_seq = seq_attendu;
                pack.lg_info = 0;
                pack.somme_ctrl = 0;
                vers_reseau(&pack);
                
                seq_attendu = inc(seq_attendu, 2);
                
            } else {
                /* l'ACK précédent s'est perdu re acquitte le numéro de séquence reçu */
                pack.type = ACK;
                pack.num_seq = pdata.num_seq;
                pack.lg_info = 0;
                pack.somme_ctrl = 0;
                vers_reseau(&pack);
            }
        }   
        
    }
        
    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}