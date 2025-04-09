//
// Created by Cherifa Mihoub on 2025-04-09.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "qdbmp.h"
#include "tuiles.h"

void afficher_spectre_detail(const t_spectre_gris *ptr_sp) {
    if (!ptr_sp) return;
    printf("Spectre de la tuile\n");
    printf("  Integrale lumineuse complete    : %.2f [unites pixel * intensite]\n", ptr_sp->integrale_lumin_compl);
    printf("  Integrale lumineuse (Seuil %.2f) : %.2f [unites pixel * intensite]\n ", ptr_sp->seuil_lumin,ptr_sp->integrale_lumin_seuil);

    printf("  Frequences non-nulles :\n");
    for (int i = 0; i < NB_FREQUENCES; ++i) {
        unsigned int count = ptr_sp->spectre[i];
        if (count > 0) {
            printf("    Niveau %3d : ", i);
            for (unsigned int k = 0; k < count; ++k) {
                putchar('*');
            }
            printf(" pixel\n");
        }
    }
}


int main() {
    /*===========================
       OUVERTURE DE L'IMAGE
    ===========================*/
    BMP* original = BMP_ReadFile("Images/image_test.bmp");
    if (BMP_GetError() != BMP_OK || original == NULL) {
        printf("Erreur d'ouverture : %s\n", BMP_GetErrorDescription());
        return 1;
    }
    printf("l'Image lue avec succès!\n\n");

    /*===========================
    DÉFINITION DES TAILLES DE TUILES
    ===========================*/
    int tailles_tuiles[3][2] = {
        {64, 64},   // 50x50
        {125, 125},  // 100x80
        {165, 165}  // 200x150
    };

    srand(time(NULL));

    for (int i = 0; i < 3; ++i) {
        int nb_col = tailles_tuiles[i][0];
        int nb_lig = tailles_tuiles[i][1];

        /*===========================
        CALCUL DU NOMBRE DE TUILES
        ===========================*/
        int total = get_nb_tuiles(original, nb_col, nb_lig); //calcul combien de tuiles dans image
        int k = rand() % total; //selectionne tuile au hasard
        t_tuile tuile; //structure représente tuile
        init_tuile(nb_col, nb_lig, &tuile); //initialise tuile avec taille voulu

        if (!get_pos_kieme_tuile(original, k, &tuile)) {
            printf("Tuile invalide!\n");
            continue;
        }

        /*===========================
        CRÉATION DU SPECTRE ET AFFICHAGE DES INTÉGRALES
        ===========================*/
        t_spectre_gris* spectre = creer_spectre_tuile(original, &tuile); //converti pixel a valeur de gris
        if (spectre == NULL) {
            printf("Erreur lors de la creation du spectre.\n");
            continue;
        }

        double integ_complet = get_integrale_seuil0(spectre); //somme toutes les intensités
        double integ_seuil05 = calcul_integrale_seuil_lumin(spectre, 0.5); //les pixels dont la luminosité est ≥ 0.5

        printf("Tuile %d Taille : %d x %d\n", i + 1, nb_col, nb_lig);
        printf("Nombre total de tuiles : %d\n", total);
        printf("Tuile selectionnee : #%d\n\n", k);



        afficher_spectre_detail(spectre);

        /*===========================
        CRÉATION DES DEUX BITMAPS (COULEUR + GRIS)
        ===========================*/
        BMP* bmp_couleur = get_bitmap_tuile(original, &tuile);
        BMP* bmp_gris = get_bitmap_gris_tuile(original, &tuile, 0.0);

        /*===========================
         SAUVEGARDE DES IMAGES EN FICHIERS
        ===========================*/
        char nom1[100], nom2[100];
        sprintf(nom1, "Tuile%d.bmp", k);
        sprintf(nom2, "TuileGris%d.bmp", k);

        BMP_WriteFile(bmp_couleur, nom1);
        BMP_WriteFile(bmp_gris, nom2);

        printf("\nImages sauvegardées : %s (couleur), %s (gris)\n\n", nom1, nom2);

        // Libération mémoire
        BMP_Free(bmp_couleur);
        BMP_Free(bmp_gris);
        free(spectre);
    }

    BMP_Free(original);
    printf("\nToutes les tuiles ont ete traitées avec succés!\n");
    return 0;
}
