#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tuiles.h"
#include "qdbmp.h"

#define SEUIL_ROUGE 0.8

int main(void) {
    // pour conserver les valeurs RGB d'un pixel
    UCHAR red, green, blue;

    // image originale et ses valeurs
    char nom_image[80];
    BMP *original;
    int nb_col, nb_lig, codage;

// la nouvelle image extraite de l'originale et ses valeurs de taille et d'offsets    BMP *nouv_image;
    BMP * nouv_image;
    int col_depart, lig_depart, colonnes, lignes;

    // les itarateurs matriciels des pixels de l'image
    int pos_x, pos_y;

    // demander le nom du fichier bitmap
    printf("Quel est le nom du fichier Bitmap? ");
    scanf("%s", nom_image);

    // ouvrir le fichier original et obtenir ses caract�ristiques principales
    original = BMP_ReadFile(nom_image);
    if (!original) {
        printf("Erreur: Impossible de lire l'image.\n");
        return 1;
    }

    nb_lig = BMP_GetHeight(original);
    nb_col = BMP_GetWidth(original);
    codage = BMP_GetDepth(original);

    // PREMIERE EXTRACTION (en vert uniquement)


    lig_depart = nb_lig / 4;
    lignes = nb_lig / 2;
    col_depart = nb_col / 4;
    colonnes = nb_col / 2;
    nouv_image = BMP_Create(colonnes, lignes, codage);

    for (pos_x = 0; pos_x < colonnes; ++pos_x) {
        for (pos_y = 0; pos_y < lignes; ++pos_y) {
            BMP_GetPixelRGB(original, col_depart + pos_x, lig_depart + pos_y, &red, &green, &blue);
            BMP_SetPixelRGB(nouv_image, pos_x, pos_y, red, green, blue);
        }
    }
    BMP_WriteFile(nouv_image, "nouvelle00.bmp");
    BMP_Free(nouv_image);

    // DEUXIÈME IMAGE : moité gauche en vert
    lignes = nb_lig;
    colonnes = nb_col / 2;
    nouv_image = BMP_Create(colonnes, lignes, codage);
// extraire la demie gauche de l'image originale et le copier en teinte verte unique
        for (pos_y = 0; pos_y < lignes; ++pos_y) {
            // obtenir les couleurs du pixel � la position [pos_y, pos_x]
            BMP_GetPixelRGB(original, pos_x, pos_y, &red, &green, &blue);
            // on copie que la valeur du VERT (autres couleurs = 0)
            BMP_SetPixelRGB(nouv_image, pos_x, pos_y, 0, green, 0);
        }

// sauvegarder l'image et lib�rer ensuite l'espace occup� par le BMP
BMP_WriteFile(nouv_image,"nouvelle01.bmp");
    BMP_Free(nouv_image);

// LA TROISIEME IMAGE EXTRAITE VA SEUILLER LE ROUGE
// seul un rouge fonc� � 80% et plus est conserv� aux pixels

// les tailles de la sous-image vont d�pendre de celles de l'original (M�ME taille)
    lignes = nb_lig;
    colonnes = nb_col;
    nouv_image = BMP_Create(colonnes, lignes,codage);

// extraire l'image originale et le copier en seuillant le rouge de chaque pixel
    for (pos_x = 0; pos_x < colonnes; ++pos_x) {
        for (pos_y = 0; pos_y < lignes; ++pos_y) {
            // obtenir les couleurs du pixel � la position [pos_y, pos_x]
            BMP_GetPixelRGB(original, pos_x, pos_y, &red, &green, &blue);

            // d�terminer si la valeur de rouge est suffisante (80% de 255 et plus)
            red = (red / 255.0) < SEUIL_ROUGE ? 0 : red;
            // on copie la valeur du ROUGE ou 0 si en bas du seuil (autres couleurs tel quel)
            BMP_SetPixelRGB(nouv_image, pos_x, pos_y, red, green, blue);
        }
    }

// sauvegarder l'image et lib�rer ensuite l'espace occup� par le BMP
    BMP_WriteFile(nouv_image, "nouvelle02.bmp");
    BMP_Free(nouv_image);

    // MANDAT 1 — Tests automatiques des fonctions
    int tailles[3][2] = {{64, 64}, {128, 128}, {256, 256}};

    for (int i = 0; i < 3; ++i) {
        int nbcol = tailles[i][0];
        int nblig = tailles[i][1];
        int nb_tuiles = get_nb_tuiles(original, nbcol, nblig);
        printf("Taille %dx%d: %d tuiles.\n", nbcol, nblig, nb_tuiles);

        int k = nb_tuiles / 2;
        t_tuile tuile;
        init_tuile(nbcol, nblig, &tuile);
        get_pos_kieme_tuile(original, k, &tuile);

        t_spectre_gris *spectre = creer_spectre_tuile(original, &tuile);
        if (spectre) {
            afficher_spectre(spectre);

            BMP *bmp_couleur = get_bitmap_tuile(original, &tuile);
            BMP *bmp_gris_0 = get_bitmap_gris_tuile(original, &tuile, 0.0);

            char nom_couleur[64], nom_gris[64];
            sprintf(nom_couleur, "Tuile_%dx%d.bmp", nbcol, nblig);
            sprintf(nom_gris, "TuileGris_%dx%d.bmp", nbcol, nblig);

            BMP_WriteFile(bmp_couleur, nom_couleur);
            BMP_WriteFile(bmp_gris_0, nom_gris);

            BMP_Free(bmp_couleur);
            BMP_Free(bmp_gris_0);

            BMP *bmp_gris_05 = get_bitmap_gris_tuile(original, &tuile, 0.5);
            BMP *bmp_gris_1 = get_bitmap_gris_tuile(original, &tuile, 1.0);

            BMP_WriteFile(bmp_gris_05, "TuileGrisSeuil_0_5.bmp");
            BMP_WriteFile(bmp_gris_1, "TuileGrisSeuil_1_0.bmp");

            BMP_Free(bmp_gris_05);
            BMP_Free(bmp_gris_1);
            free(spectre);
        }
    }
// le free fonctionnel du BMP original
    BMP_Free(original);
    printf("3 nouvelles images ont ete copiees dans le repertoire de travail..\n");
    system("pause");
    return 0;
}
