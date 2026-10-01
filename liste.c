#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

/* --- compteur d'allocations, interne au module --- */

static int blocs = 0;

static void *suivi_malloc(size_t taille, const char *etiquette)
{
    void *p = malloc(taille);

    if (p != NULL) blocs++;

    fprintf(stderr, "[alloc] %zu octets (%s) -> %p\n", taille, etiquette, p);

    return p;
}

static void suivi_free(void *p)
{
    if (p != NULL) {
        blocs--;
        free(p);
    }
}

int liste_blocs_en_circulation(void)
{
    return blocs;
}

Maillon *liste_inserer(Maillon *tete, int valeur)
{
    Maillon *m = suivi_malloc(sizeof(Maillon), "liste_inserer");
    if (m == NULL) { perror("malloc"); exit(EXIT_FAILURE); }
    m->valeur  = valeur;
    m->suivant = tete;
    return m;
}

int liste_longueur(const Maillon *tete)
{
    int n = 0;
    for (const Maillon *m = tete; m != NULL; m = m->suivant) n++;
    return n;
}

bool liste_contient(const Maillon *tete, int valeur)
{
    for (const Maillon *m = tete; m != NULL; m = m->suivant)
        if (m->valeur == valeur) return true;
    return false;
}

void liste_afficher(const Maillon *tete)
{
    for (const Maillon *m = tete; m != NULL; m = m->suivant)
        printf("%d -> ", m->valeur);
    printf("NULL\n");
}

void liste_liberer(Maillon *tete)
{
    Maillon *m = tete;
    while (m != NULL) {
        Maillon *suiv = m->suivant;
        suivi_free(m);
        m = suiv;
    }
}

bool liste_maximum(const Maillon *tete, int *resultat)
{
    if (tete == NULL) return false;          /* liste vide */

    int max = tete->valeur;                  /* le premier element */

    for (const Maillon *m = tete->suivant; m != NULL; m = m->suivant)
        if (m->valeur > max) max = m->valeur;

    *resultat = max;

    return true;
}

static Maillon *dernier(Maillon *tete)
{
    Maillon *m = tete;
    while (m != NULL && m->suivant != NULL) m = m->suivant;
    return m;
}

Maillon *liste_inserer_en_queue(Maillon *tete, int valeur)
{
    Maillon *nouveau = suivi_malloc(sizeof(Maillon), "liste_inserer_en_queue");
    if (nouveau == NULL) { perror("malloc"); exit(EXIT_FAILURE); }
    nouveau->valeur  = valeur;
    nouveau->suivant = NULL;

    if (tete == NULL) return nouveau;

    dernier(tete)->suivant = nouveau;
    return tete;
}
