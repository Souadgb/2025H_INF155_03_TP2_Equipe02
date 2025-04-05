/*
	EXEMPLE D'UTILISATION DU MODULE QUICK & DIRTY BMP

	D'UNE SEULE IMAGE ORIGINALE LUE (BMP_ReadFile)
	ON VA CR�ER TROIS NOUVEAUX BITMAPS (BMP_Create) POUR :

	1- COPIER TEL QUEL LE QUART DE L'IMAGE ORIGINALE
	2- COPIER UNIQUEMENT LE VERT DE LA DEMIE GAUCHE DE L'IMAGE
	3- SEUILLER � 0.8 LE ROUGE DES PIXELS DE L'IMAGE COMPL�TE
*/

/*
	CHOISIR VOTRE IMAGE :
	choisissez une image *.bmp
	copiez-la avec les fichiers du projet ( .h et .c )
    mettre le nom complet de l'image lorsque demand�
	tout va fonctionner
*/

#define  _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "tuiles.h"

#include "qdbmp.h"    //le module de traitement de Bitmaps

#define SEUIL_ROUGE 0.8    //seuil de 8
int main() {
	BMP *image = BMP_ReadFile("Images/image_test.bmp");

	if (BMP_GetError() != BMP_OK) {
		printf("Erreur QDBMP: %s\n", BMP_GetErrorDescription());
		return 1;
	}

	if (image == NULL) {
		printf("Erreur: Impossible de lire l'image.\n");
		return 1;
	}

	int tailles[][2] = {{64, 64}, {128, 128}, {256, 256}};
	int i;

	for (i = 0; i < 3; ++i) {
		int nb_col = tailles[i][0];
		int nb_lig = tailles[i][1];

		int nb_tuiles = get_nb_tuiles(image, nb_col, nb_lig);
		printf("Taille de tuile %dx%d: %d tuiles possibles.\n", nb_col, nb_lig, nb_tuiles);

		int k = nb_tuiles / 2; // tuile au milieu

		t_tuile tuile;
		init_tuile(nb_col, nb_lig, &tuile);
		get_pos_kieme_tuile(image, k, &tuile);

		t_spectre_gris *spectre = creer_spectre_tuile(image, &tuile);
		if (spectre != NULL) {
			printf("Spectre de la tuile #%d cree.\n", k);
			printf("Integrale seuil 0.0 = %.3f\n", get_integrale_seuil0(spectre));
			BMP *tuile_couleur = get_bitmap_tuile(image, &tuile);
			BMP *tuile_gris = get_bitmap_gris_tuile(image, &tuile, 0.0);

			char nom_couleur[64], nom_gris[64];
			sprintf(nom_couleur, "Tuile_%d_%d.bmp", nb_col, nb_lig);
			sprintf(nom_gris, "TuileGris_%d_%d.bmp", nb_col, nb_lig);

			BMP_WriteFile(tuile_couleur, nom_couleur);
			BMP_WriteFile(tuile_gris, nom_gris);

			printf("Images sauvegardees: %s et %s\n", nom_couleur, nom_gris);

			BMP_Free(tuile_couleur);
			BMP_Free(tuile_gris);
			free(spectre);
		}
	}

	BMP_Free(image);
	return 0;
}
