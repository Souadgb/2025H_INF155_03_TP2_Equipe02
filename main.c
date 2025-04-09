//
// Created by Cherifa Mihoub on 2025-04-09.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "qdbmp.h"
#include "tuiles.h"

int main() {
    //===========================
    // 1. OUVERTURE DE L'IMAGE
    //===========================
    BMP* original = BMP_ReadFile("Images/image_test.bmp");
    if (BMP_GetError() != BMP_OK || original == NULL) {
        printf("Erreur d'ouverture : %s\n", BMP_GetErrorDescription());
        return 1;
    }
    printf("Image lue avec succes!\n\n");

    //===========================
    // 2. DÉFINITION DES TAILLES DE TUILES
    //===========================
    int tailles_tuiles[3][2] = {
        {50, 50},   // 50x50
        {100, 80},  // 100x80
        {200, 150}  // 200x150
    };

    srand(time(NULL));

    for (int i = 0; i < 3; ++i) {
        int nb_col = tailles_tuiles[i][0];
        int nb_lig = tailles_tuiles[i][1];

        //===========================
        // 3. CALCUL DU NOMBRE DE TUILES
        //===========================
        int total = get_nb_tuiles(original, nb_col, nb_lig);
        int k = rand() % total;
        t_tuile tuile;
        init_tuile(nb_col, nb_lig, &tuile);

        if (!get_pos_kieme_tuile(original, k, &tuile)) {
            printf("Tuile invalide!\n");
            continue;
        }

        //===========================
        // 4. CRÉATION DU SPECTRE ET AFFICHAGE DES INTÉGRALES
        //===========================
        t_spectre_gris* spectre = creer_spectre_tuile(original, &tuile);
        if (spectre == NULL) {
            printf("Erreur lors de la creation du spectre.\n");
            continue;
        }

        double integ_complet = get_integrale_seuil0(spectre);
        double integ_seuil05 = calcul_integrale_seuil_lumin(spectre, 0.5);

        printf("--- Tuile %d --- Taille : %d x %d\n", i + 1, nb_col, nb_lig);
        printf("Nombre total de tuiles : %d\n", total);
        printf("Tuile selectionnee : #%d\n\n", k);

        printf("Analyse spectre :\n");
        printf("  - Integrale lumineuse complete       : %.4f [unites pixel * intensite]\n", integ_complet);
        printf("  - Integrale lumineuse (seuil 0.5)    : %.4f [unites pixel * intensite]\n", integ_seuil05);

        //===========================
        // 5. CRÉATION DES DEUX BITMAPS (COULEUR + GRIS)
        //===========================
        BMP* bmp_couleur = get_bitmap_tuile(original, &tuile);
        BMP* bmp_gris = get_bitmap_gris_tuile(original, &tuile, 0.0);

        //===========================
        // 6. SAUVEGARDE DES IMAGES EN FICHIERS
        //===========================
        char nom1[100], nom2[100];
        sprintf(nom1, "Tuile%d.bmp", k);
        sprintf(nom2, "TuileGris%d.bmp", k);

        BMP_WriteFile(bmp_couleur, nom1);
        BMP_WriteFile(bmp_gris, nom2);

        printf("\nImages sauvegardees : %s (couleur), %s (gris)\n\n", nom1, nom2);

        // Libération mémoire
        BMP_Free(bmp_couleur);
        BMP_Free(bmp_gris);
        free(spectre);
    }

    BMP_Free(original);
    printf("\nToutes les tuiles ont ete traitees avec succes!\n");
    return 0;
}
