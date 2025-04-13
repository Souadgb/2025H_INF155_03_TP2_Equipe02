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
// Récupère le spectre d'indice k
t_spectre_gris *get_kieme_ptr_sp(const t_recouvrement *rec, int k) {
    if (!rec || k < 0 || k >= rec->taille_tab_spectres) return NULL;
    return rec->tab_spectres[k];
}

// Recalcule toutes les intégrales avec un seuil donné
void calcul_integrales_seuil(t_recouvrement *rec, double seuil) {
    if (!rec || seuil < 0.0 || seuil > 1.0) return;
    for (int i = 0; i < rec->taille_tab_spectres; i++) {
        if (rec->tab_spectres[i]) {
            calcul_integrale_seuil_lumin(rec->tab_spectres[i], seuil);
        }
    }
}

// Fonction de comparaison (utilisée pour qsort)
int comparer_spectres(const void *a, const void *b) {
    const t_spectre_gris *sp1 = *(const t_spectre_gris **)a;
    const t_spectre_gris *sp2 = *(const t_spectre_gris **)b;
    double lum1 = sp1 ? sp1->integrale_lumin_seuil : -1.0;
    double lum2 = sp2 ? sp2->integrale_lumin_seuil : -1.0;
    return (lum2 > lum1) - (lum2 < lum1);
}

// Trie les spectres selon leur luminosité
void trier_spectres(t_recouvrement *rec, double seuil) {
    if (!rec) return;
    calcul_integrales_seuil(rec, seuil);
    qsort(rec->tab_spectres, rec->taille_tab_spectres, sizeof(t_spectre_gris *), comparer_spectres);
}

// Reconstruit une image avec les tuiles lumineuses seulement
BMP * reconstruire_image(BMP * original, const t_recouvrement *rec,
                         double prop_garde, double prop_min,
                         char * fichier_log) {

    if (!original || !rec || prop_garde <= 0.0 || prop_garde > 1.0 || prop_min < 0.0 || prop_min > 1.0) return NULL;

    BMP *resultat = BMP_Create(BMP_GetWidth(original), BMP_GetHeight(original), BMP_GetDepth(original));
    if (!resultat) return NULL;

    int total = rec->taille_tab_spectres;
    double max_lum = rec->tab_spectres[0]->integrale_lumin_seuil;
    int *copiee = calloc(total, sizeof(int));

    FILE *log = NULL;
    if (fichier_log) log = fopen(fichier_log, "w");

    // Entête log : seuil, taille, prop_garde, prop_min
    if (log)
        fprintf(log, "%.1f %d %d %.1f %.1f\n",
                rec->tab_spectres[0]->seuil_lumin,
                rec->tab_spectres[0]->tuile.nb_col,
                rec->tab_spectres[0]->tuile.nb_lig,
                prop_garde, prop_min);

    for (int i = 0; i < total; ++i) {
        t_spectre_gris *sp = rec->tab_spectres[i];
        if (!sp) continue;

        double R = sp->integrale_lumin_seuil / max_lum;
        t_tuile tuile = sp->tuile;

        // Tuile principale : R > prop_garde
        if (R > prop_garde && !copiee[tuile.id_enum]) {
            for (int y = 0; y < tuile.nb_lig; y++) {
                for (int x = 0; x < tuile.nb_col; x++) {
                    UCHAR r, g, b;
                    BMP_GetPixelRGB(original, tuile.offset_col + x, tuile.offset_lig + y, &r, &g, &b);
                    BMP_SetPixelRGB(resultat, tuile.offset_col + x, tuile.offset_lig + y, r, g, b);
                }
            }
            if (log)
                fprintf(log, "%d, %d, %d, %.2f, %.3f\n",
                        tuile.id_enum, tuile.offset_col, tuile.offset_lig, sp->integrale_lumin_seuil, R);
            copiee[tuile.id_enum] = 1;
        }
    }

    // Deuxième passe : tuiles voisines (prop_garde >= R > prop_min)
    for (int i = 0; i < total; ++i) {
        t_spectre_gris *sp = rec->tab_spectres[i];
        if (!sp) continue;

        double R = sp->integrale_lumin_seuil / max_lum;
        t_tuile tuile = sp->tuile;

        if (R <= prop_garde && R > prop_min && !copiee[tuile.id_enum]) {
            for (int y = 0; y < tuile.nb_lig; y++) {
                for (int x = 0; x < tuile.nb_col; x++) {
                    UCHAR r, g, b;
                    BMP_GetPixelRGB(original, tuile.offset_col + x, tuile.offset_lig + y, &r, &g, &b);
                    BMP_SetPixelRGB(resultat, tuile.offset_col + x, tuile.offset_lig + y, r, g, b);
                }
            }
            if (log)
                fprintf(log, "%d, %d, %d, %.2f, %.3f\n",
                        tuile.id_enum, tuile.offset_col, tuile.offset_lig, sp->integrale_lumin_seuil, R);
            copiee[tuile.id_enum] = 1;
        }
    }

    if (log) fclose(log);
    free(copiee);
    return resultat;
}