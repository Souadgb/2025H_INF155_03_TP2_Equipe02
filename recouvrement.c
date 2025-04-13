//
// Created by souad on 2025-04-12.
//

#include "recouvrement.h"
#include <stdlib.h>
#include <string.h>



// Initialise un recouvrement avec un tableau de spectres.
t_recouvrement *init_recouvrement(int nb_tuiles, int nbcol_tuile, int nblig_tuile) {
    t_recouvrement *rec = (t_recouvrement *)malloc(sizeof(t_recouvrement));
    if (!rec) return NULL;

    rec->tab_spectres = (t_spectre_gris **)calloc(nb_tuiles, sizeof(t_spectre_gris *));
    if (!rec->tab_spectres) {
        free(rec);
        return NULL;
    }

    rec->taille_tab_spectres = nb_tuiles;
    rec->largeur_tuile = nbcol_tuile;
    rec->hauteur_tuile = nblig_tuile;
    return rec;
}

// Libère la mémoire du recouvrement.
void detruire_recouvrement(t_recouvrement *rec) {
    if (!rec) return;

    for (int i = 0; i < rec->taille_tab_spectres; i++) {
        if (rec->tab_spectres[i]) {
            free(rec->tab_spectres[i]);
        }
    }
    free(rec->tab_spectres);
    free(rec);
}

// Ajoute un spectre au recouvrement.
int ajouter_spectre_rec(t_spectre_gris *ptr_sp, t_recouvrement *rec) {
    if (!ptr_sp || !rec || ptr_sp->tuile.id_enum < 0 || ptr_sp->tuile.id_enum >= rec->taille_tab_spectres) {
        return 0;
    }
    rec->tab_spectres[ptr_sp->tuile.id_enum] = ptr_sp;
    return 1;
}