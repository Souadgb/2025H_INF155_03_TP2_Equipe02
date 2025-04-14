//
// Created by Cherifa Mihoub on 2025-04-09.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "qdbmp.h"
#include "tuiles.h"
#include "recouvrement.h"

void afficher_spectre_detail(const t_spectre_gris *ptr_sp) {
    if (!ptr_sp) return;
    // integrale lumineuse complete: chaque pixel * son intensité lumineuse
    // seuil ignoré intensité trop faibles
    printf("Spectre de la tuile\n");
    printf("  Integrale lumineuse complete    : %.2f [unites pixel * intensite]\n", ptr_sp->integrale_lumin_compl);
    printf("  Integrale lumineuse (Seuil %.2f) : %.2f [unites pixel * intensite]\n ", ptr_sp->seuil_lumin, ptr_sp->integrale_lumin_seuil);

    printf(" Frequences non-nulles (affichage proportionnel a 20 etoiles max) :\n");

    // Étape 1 : trouver la valeur max du spectre
    unsigned int max = 0;
    for (int i = 0; i < NB_FREQUENCES; ++i) {
        if (ptr_sp->spectre[i] > max) {
            max = ptr_sp->spectre[i];
        }
    }

    // Éviter division par zéro
    if (max == 0) return;

    // Étape 2 : affichage proportionnel avec 20 étoiles
    for (int i = 0; i < NB_FREQUENCES; ++i) {
        unsigned int count = ptr_sp->spectre[i];
        if (count > 0) {
            printf("    Niveau %3d : ", i);
            // Calculer le nombre d'étoiles proportionnel (arrondi)
            int nb_etoiles = (int)((((float)count / max) * 20) + 0.5);
            for (int k = 0; k < nb_etoiles; ++k) {
                putchar('*');
            }
            printf(" (%u pixels)\n", count);
        }
    }
}

// FONCTION MAIN //
int main() {

      // OUVERTURE DE L'IMAGE //

    BMP* original = BMP_ReadFile("Images/image_test.bmp");
    if (BMP_GetError() != BMP_OK || original == NULL) {
        printf("Erreur d'ouverture : %s\n", BMP_GetErrorDescription());
        return 1;
    }
    printf("L'image est lue avec succes!\n\n");

    //DÉFINITION DES TAILLES DE TUILES

    int tailles_tuiles[3][2] = {
        {64, 64},
        {125, 125},
        {165, 165}
    };

    srand(time(NULL));

    for (int i = 0; i < 3; ++i) { //Pour chaque taille de tuile
        int nb_col = tailles_tuiles[i][0];
        int nb_lig = tailles_tuiles[i][1]; //largeur et hauteur de la tuile


        //CALCUL DU NOMBRE DE TUILES//

        int total = get_nb_tuiles(original, nb_col, nb_lig); //calcul combien de tuiles dans image
        int k = rand() % total; //selectionne tuile au hasard
        t_tuile tuile; //structure représente tuile
        init_tuile(nb_col, nb_lig, &tuile); //initialise tuile avec taille voulu

        if (!get_pos_kieme_tuile(original, k, &tuile)) {
            printf("Tuile invalide!\n");
            continue;
        }

        //CRÉATION DU SPECTRE ET AFFICHAGE DES INTÉGRALES

        t_spectre_gris* spectre = creer_spectre_tuile(original, &tuile); //converti pixel a valeur de gris
        if (spectre == NULL) {
            printf("Erreur lors de la creation du spectre.\n");
            continue;
        }

        double integ_complet = get_integrale_seuil0(spectre); //somme toutes les intensités
        double integ_seuil05 = calcul_integrale_seuil_lumin(spectre, 0.5); //les pixels dont la luminosité est ≥ 0.5

        printf("Tuile %d \nTaille : %d x %d\n", i + 1, nb_col, nb_lig);
        printf("Nombre total de tuiles : %d\n", total);
        printf("Tuile selectionnee : #%d\n\n", k);



        afficher_spectre_detail(spectre);


        //CRÉATION DES DEUX BITMAPS (COULEUR + GRIS)//

        BMP* bmp_couleur = get_bitmap_tuile(original, &tuile);
        BMP* bmp_gris = get_bitmap_gris_tuile(original, &tuile, 0.0);


        // SAUVEGARDE DES IMAGES EN FICHIERS//

        char nom1[100], nom2[100];
        sprintf(nom1, "Tuile%d.bmp", k);
        sprintf(nom2, "TuileGris%d.bmp", k);

        BMP_WriteFile(bmp_couleur, nom1);
        BMP_WriteFile(bmp_gris, nom2);

        printf("\nImages sauvegardees : %s (couleur), %s (gris)\n\n", nom1, nom2);

        // Libération de la mémoire
        BMP_Free(bmp_couleur);
        BMP_Free(bmp_gris);
        free(spectre);
    }

    // PARTIE 2 - RECOUVREMENT COMPLET
    int nb_col = 64, nb_lig = 64;
    double seuil_lum = 0.5, prop_garde = 0.2, prop_min = 0.3;

    FILE* log_file = fopen("log_resultat.txt", "w");
    if (!log_file) {
        printf("Erreur : impossible d'ouvrir le fichier log.\n");
        return 1;
    }
    fprintf(log_file, "%.1f %d %d %.1f %.1f\n", seuil_lum, nb_col, nb_lig, prop_garde, prop_min);

    int nb_tuiles = get_nb_tuiles(original, nb_col, nb_lig);
    t_recouvrement *rec = init_recouvrement(nb_tuiles, nb_col, nb_lig);

    for (int k = 0; k < nb_tuiles; ++k) {
        t_tuile tuile;
        init_tuile(nb_col, nb_lig, &tuile);
        if (get_pos_kieme_tuile(original, k, &tuile)) {
            t_spectre_gris *sp = creer_spectre_tuile(original, &tuile);
            ajouter_spectre_rec(sp, rec);
        }
    }

    trier_spectres(rec, seuil_lum);
    BMP *res = reconstruire_image(original, rec, prop_garde, prop_min, log_file);
    BMP_WriteFile(res, "image_reconstruite.bmp");
    printf("Image reconstruite sauvegardee dans 'image_reconstruite.bmp'\n");

    BMP_Free(res);
    detruire_recouvrement(rec);
    BMP_Free(original);
    fclose(log_file);
    printf("\nToutes les tuiles ont ete traitees avec succes!\n");


    return 0;
}
// Ouvre le fichier de configuration et retourne son pointeur + nb_tests
FILE *ouvrir_fich_data(const char * nom_fich, int * nb_tests) {
    FILE *f = fopen(nom_fich, "r");
    if (!f) return NULL;
    fscanf(f, "%d", nb_tests);
    return f;
}

// Lit les données de la prochaine ligne (nom image, seuil, tuiles, garde, min)
int lire_donnees_fich(FILE * fich, char * nom_image, double * seuil,
                      int * nb_col_tu, int * nb_lig_tu,
                      double * prop_garde, double * prop_min) {
    return fscanf(fich, "%s %lf %d %d %lf %lf",
                  nom_image, seuil, nb_col_tu, nb_lig_tu, prop_garde, prop_min) == 6;
}
