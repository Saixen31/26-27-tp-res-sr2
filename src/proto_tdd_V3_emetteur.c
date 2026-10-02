#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"



/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */

int main(int argc, char* argv[]) {
    unsigned char message[MAX_INFO];
    int taille_msg;
    paquet_t paquet;
    paquet_t ack;
    int borne_inf; taille_fenetre; curseur; 
    uint8_t seq_a_emettre = 0; 
    int evenement;
    init_reseau(EMISSION);
    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport v2.\n");

    de_application(message, &taille_msg);

    while (taille_msg != 0) {
        
        for (int i=0; i<taille_msg; i++) {
            paquet.info[i] = message[i];
        }
        paquet.lg_info = taille_msg;
        paquet.type = DATA;
        paquet.num_seq = seq_a_emettre; 
        paquet.somme_ctrl = generer_controle(&paquet);

        int ack_recu = 0;
        
        while (!ack_recu) {
            vers_reseau(&paquet);
            depart_temporisateur(100); 
            
            evenement = attendre(); 
            
            // Si la fonction attendre() renvoie -1, c'est l'événement PAQUET_RECU
            if (evenement == PAQUET_RECU) {
                de_reseau(&ack);
                
                // On valide que c'est un ACK et qu'il porte le bon numéro
                if (ack.type == ACK && ack.num_seq == seq_a_emettre) {
                    arret_temporisateur(); 
                    ack_recu = 1;
                    printf("[TRP] Paquet %d acquitte.\n", seq_a_emettre);
                }
            } else {
                
                printf("[TRP] Timeout ! Retransmission du paquet %d.\n", seq_a_emettre);
            }
        }
        
        seq_a_emettre = inc(seq_a_emettre, 2);
        de_application(message, &taille_msg);
    }
    
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
