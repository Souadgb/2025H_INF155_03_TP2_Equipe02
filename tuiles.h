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

// Calcul du nombre de tuiles possibles dans l'image
int get_nb_tuiles(BMP *original, int nbcol_tuile, int nblig_tuile);

// Initialisation d'une tuile avec sa taille
void init_tuile(int nbcol_tuile, int nblig_tuile, t_tuile *tuile);

// Positionnement d'une tuile sp�cifique dans l'image
int get_pos_kieme_tuile(BMP *original, int k, t_tuile *tuile);

// Calcul de l'int�grale de luminosit� avec seuil
double calcul_integrale_seuil_lumin(t_spectre_gris *ptr_sp, double seuil);

// Cr�ation d'un spectre de luminosit� pour une tuile
t_spectre_gris *creer_spectre_tuile(BMP *original, const t_tuile *tuile);

// Extraction d'une tuile sous forme de nouvelle image BMP
BMP *get_bitmap_tuile(BMP *original, const t_tuile *tuile);

// Extraction d'une tuile en niveaux de gris avec seuil
BMP *get_bitmap_gris_tuile(BMP *original, const t_tuile *tuile, double seuil_lum);

/* ===== Fonctions informatrices ===== */

// Obtention de l'identifiant d'une tuile
int get_id_tuile(const t_tuile *tu);

// Obtention des d�calages d'une tuile
void get_offsets_tuile(const t_tuile *tu, int *offset_x, int *offset_y);

// Obtention de l'int�grale sans seuil
double get_integrale_seuil0(const t_spectre_gris *ptr_sp);

// Obtention du seuil utilis�
double get_seuil_variant(const t_spectre_gris *ptr_sp);

// Obtention de l'int�grale avec seuil
double get_integrale_seuil_variant(const t_spectre_gris *ptr_sp);

// Obtention de la tuile associ�e � un spectre
void get_tuile(const t_spectre_gris *ptr_sp, t_tuile *tuile);

// Affichage du spectre (pour d�bogage)
void afficher_spectre(const t_spectre_gris *ptr_sp);

/* ======= Fonctions de la partie 2 ======= */

//Assurer un meilleur ajustement de la tuile dans l'image originale
void calibrer_taille_tuile(BMP *original, int *nb_col, int *nb_lig);

//Indiquer si deux tuiles sont voisines
int tuiles_voisines(const t_tuile *tuile1, const t_tuile *tuile2);

//Copier le contenu d'un bitmap-tuile dans une image-resulat pixel par pixel
void copier_tuile_a_image(BMP *image_res, BMP *imag, const t_tuile *tuile);


#endif
