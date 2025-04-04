//
// Created by souad on 2025-03-17.
//
#include "tuiles.h"

// BPM *original pointeur vers image BPM
int get_nb_tuiles(BMP *original, int nbcol_tuile, int nblig_tuile) {
    if(original=NULL || nbcol_tuile <= 0 || nblig_tuile <=0 ) { // verifie si image ou dimension invalides
        return 0;
    }
    UINT largeur = BMP_GetWidth(original);
    UINT hauteur= BMP_GetHeight(original);

    int nb_tuiles_hotiz = largeur / nbcol_tuile;
    int nb_tuiles_vert = hauteur / nblig_tuile;

    return nb_tuiles_hotiz * nb_tuiles_vert; //calcul le nombre de tuiles dan l'image

}

void init_tuile(int nbcol_tuile, int nblig_tuile, t_tuile * tuile) {
    if (tuile == NULL || nbcol_tuile <= 0 || nblig_tuile <= 0)
    return; // verifie si pointeur tuile n'est ps vide (valide, pointe vers vrai structure)
    tuile->offset_col = 0;
    tuile->offset_lig = 0;
    tuile->nb_col = nbcol_tuile;
    tuile->nb_lig = nblig_tuile;
    tuile->id_enum = 0;
} //preparer tuile avant de l'utiliser


int get_pos_kieme_tuile(BMP *original, int k, t_tuile * tuile) {
    if (original == NULL || tuile == NULL || k < 0)
        return 0; //verifie que image et tuile existent, que numero tuile et positif

    UINT largeur = BMP_GetWidth(original);
    UINT hauteur = BMP_GetHeight(original);

    int nb_tuiles_horiz = largeur / tuile->nb_col;
    int nb_tuiles_vert = hauteur / tuile->nb_lig;
    int nb_total = nb_tuiles_horiz * nb_tuiles_vert; //calcul nombre total de tuile

    if (k >= nb_total)
        return 0; //verifie que k n'est pas hors limite

    int lig_index = k / nb_tuiles_horiz; //numero de la ligne où se trouve tuile k
    int col_index = k % nb_tuiles_horiz; //numero colonne tuile k
    /*
     ex: tuile k=5 donc ligne 1 et colonne 1
    Ligne 0 :  [0] [1] [2] [3]
    Ligne 1 :  [4] [5] [6] [7]
    Ligne 2 :  [8] [9] [10] [11]
    */

    // les coordonnées en pixels de la tuile
    tuile->offset_col = col_index * tuile->nb_col;
    tuile->offset_lig = lig_index * tuile->nb_lig;
    tuile->id_enum = k;

    return 1; //positionne tuile dans l'image BMP
}

double calcul_integrale_seuil_lumin(t_spectre_gris * ptr_sp, double seuil) {

}

t_spectre_gris * creer_spectre_tuile(BMP *original, const t_tuile * tuile) {

}

BMP * get_bitmap_tuile(BMP *original, const t_tuile * tuile) {

}

BMP * get_bitmap_gris_tuile(BMP *original, const t_tuile * tuile, double seuil_lum) {

}

int get_id_tuile(const t_tuile * tu) {

}

void get_offsets_tuile(const t_tuile * tu, int * offset_x, int * offset_y) {

}

double get_integrale_seuil0(const t_spectre_gris * ptr_sp) {

}

double get_seuil_variant(const t_spectre_gris * ptr_sp) {

}

double get_integrale_seuil_variant(const t_spectre_gris * ptr_sp) {

}

void get_tuile(const t_spectre_gris * ptr_sp, t_tuile * tuile) {

}

