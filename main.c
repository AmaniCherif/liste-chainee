#include <stdio.h>
#include "liste.h"
#include "test.h"

int main(void)
{
    Maillon *liste = NULL;
    for (int i = 1; i <= 5; i++) liste = liste_inserer(liste, i * 10);
    test_verifier(liste_blocs_en_circulation() == 5, "5 blocs apres construction");

    printf("liste     : ");
    liste_afficher(liste);
    test_verifier(liste_longueur(liste) == 5, "longueur 5");
    test_verifier(liste_contient(liste, 30), "contient 30");
    test_verifier(!liste_contient(liste, 99), "ne contient pas 99");

    int max = -1;
    test_verifier(liste_maximum(liste, &max) && max == 50, "maximum 50");
    test_verifier(!liste_maximum(NULL, &max) && max == 50, "maximum(NULL) intact");

    liste = liste_inserer_en_queue(liste, 5);
    test_verifier(liste_longueur(liste) == 6, "longueur 6 apres queue");
    int dernier_val = 0;
    for (const Maillon *m = liste; m != NULL; m = m->suivant) dernier_val = m->valeur;
    test_verifier(dernier_val == 5, "la queue vaut 5");

    Maillon *vide = liste_inserer_en_queue(NULL, 1);
    test_verifier(liste_longueur(vide) == 1, "queue sur liste vide");
    liste_liberer(vide);

    liste_liberer(liste);
    test_verifier(liste_blocs_en_circulation() == 0, "0 bloc apres liberation");

    return test_bilan() == 0 ? 0 : 1;
}
