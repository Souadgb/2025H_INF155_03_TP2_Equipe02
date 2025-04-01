/**************************************************************
PAR : Hugues Saulnier. R�vis� par Eric Th�
DATE: Hiver 2025

Le module d'outils demand� dans la premi�re partie du TP2.

Le module offre les types-struct : t_tuile, t_spectre_gris

Le module offre les fonctions suivantes :
 - get_nb_tuiles()
 - init_tuile()
 - get_pos_kieme_tuile()
 - calcul_integrale_seuil_lumin()
 - creer_spectre_tuile()
 - get_bitmap_tuile()
 - get_bitmap_gris_tuile()
 - get_id_tuile()
 - get_offsets_tuile()
 - get_integrale_seuil0()
 - get_seuil_variant()
 - get_integrale_seuil_variant()
 - get_tuile()
 - afficher_spectre()

 - calibrer_taille_tuile()  //PARTIE 2
 - tuiles_voisines()		//PARTIE 2
 - copier_tuile_a_image()	//PARTIE 2

****************************************************************/

#ifndef __OUTILS_41028955_H__
#define __OUTILS_41028955_H__

//**************************************************************

// merci Chai Braudo pour le beau petit module Quick n' Dirty BMP
#include "qdbmp.h"

#define  NB_FREQUENCES  256     //le nombre d'intensit�s et taille du spectre


//**************************************************************
//**************************************************************
// DEUX TYPES ESSENTIELS dont voici les d�finitions

/*
  LA TUILE
  une tuile a une taille bien d�finie et occupe une position bien definie
  dans un bitmap original. Elle est repr�sent�e par 5 entiers
*/
typedef struct {
	// les decalages dans l'image originale
	int offset_col;		// decalage-colonne
	int offset_lig;		// decalage-ligne

	// sa taille v�ritable en pixels
	int nb_lig;
	int nb_col;

	// son num�ro unique dans l'�num�ration des tuiles d'une image
	int id_enum;
} t_tuile;

/*
  LE SPECTRE
  c'est dans un spectre qu'on va conserver tout ce qui est pertinent �
  la mesure de la luminosit� d'une tuile
*/
typedef struct {
	// la tuile dont est issu le spectre est conserv�
	t_tuile tuile;

	// le tableau des fr�quences de chaque teinte de gris dans la tuile
	UINT spectre[NB_FREQUENCES];

	// int�grale compl�te de la luminosit� d'un spectre SANS seuil (= 0) obtenue
	// de la somme des produits ((i/255.0) * spectre[i]) pour i allant de 0 � 255
	double integrale_lumin_compl;

	// les deux derniers membres sont associ�s � une mesure de luminosit� seuill�e
	double seuil_lumin;     // le seuil entre 0 et 1

	// int�grale avec seuil, idem � l'integrale pr�c�dente sauf que tous les i tels
	// que (i/255.0) < seuil_lumen ne seront PAS consid�r�s dans la sommation
	double integrale_lumin_seuil;
} t_spectre_gris;


//**************************************************************
// LES D�CLARATIONS DES 7 FONCTIONS DE CALCULS DE LA PARTIE 1
//**************************************************************

int get_nb_tuiles(BMP *original, int nbcol_tuile, int nblig_tuile);

void init_tuile(int nbcol_tuile, int nblig_tuile, t_tuile * tuile);

int get_pos_kieme_tuile(BMP *original, int k, t_tuile * tuile);

double calcul_integrale_seuil_lumin(t_spectre_gris * ptr_sp, double seuil);

t_spectre_gris * creer_spectre_tuile(BMP *original, const t_tuile * tuile);

BMP * get_bitmap_tuile(BMP *original, const t_tuile * tuile);

BMP * get_bitmap_gris_tuile(BMP *original, const t_tuile * tuile, double seuil_lum);






#endif
