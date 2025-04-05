//
// Created by souad on 2025-03-17.
//
#include "tuiles.h"
#include <stdlib.h>
#include <math.h>

int get_nb_tuiles(BMP *original, int nbcol_tuile, int nblig_tuile) {
    if(original == NULL || nbcol_tuile <= 0 || nblig_tuile <= 0) {
        return 0;
    }

    UINT largeur = BMP_GetWidth(original);
    UINT hauteur = BMP_GetHeight(original);

    return (largeur / nbcol_tuile) * (hauteur / nblig_tuile);
}

void init_tuile(int nbcol_tuile, int nblig_tuile, t_tuile *tuile) {
    if(tuile == NULL || nbcol_tuile <= 0 || nblig_tuile <= 0) return;

    tuile->offset_col = -1;
    tuile->offset_lig = -1;
    tuile->nb_col = nbcol_tuile;
    tuile->nb_lig = nblig_tuile;
    tuile->id_enum = -1;
}

int get_pos_kieme_tuile(BMP *original, int k, t_tuile *tuile) {
    if(original == NULL || tuile == NULL || k < 0) return 0;

    UINT largeur = BMP_GetWidth(original);
    UINT hauteur = BMP_GetHeight(original);
    int nb_tuiles_ligne = largeur / tuile->nb_col;
    int nb_total = nb_tuiles_ligne * (hauteur / tuile->nb_lig);

    if(k >= nb_total) return 0;

    tuile->offset_col = (k % nb_tuiles_ligne) * tuile->nb_col;
    tuile->offset_lig = (k / nb_tuiles_ligne) * tuile->nb_lig;
    tuile->id_enum = k;

    return 1;
}

double calcul_integrale_seuil_lumin(t_spectre_gris *ptr_sp, double seuil) {
    if(ptr_sp == NULL || seuil < 0.0 || seuil > 1.0) return -1.0;

    double integrale = 0.0;
    for(int i = 0; i < NB_FREQUENCES; i++) {
        double intensite = i / 255.0;
        if(seuil == 0.0 || intensite >= seuil) {
            integrale += intensite * ptr_sp->spectre[i];
        }
    }

    if(seuil == 0.0) {
        ptr_sp->integrale_lumin_compl = integrale;
    } else {
        ptr_sp->seuil_lumin = seuil;
        ptr_sp->integrale_lumin_seuil = integrale;
    }

    return integrale;
}

t_spectre_gris *creer_spectre_tuile(BMP *original, const t_tuile *tuile) {
    if(original == NULL || tuile == NULL) return NULL;

    t_spectre_gris *spectre = (t_spectre_gris*)malloc(sizeof(t_spectre_gris));
    if(spectre == NULL) return NULL;

    // Initialisation
    spectre->tuile = *tuile;
    spectre->integrale_lumin_compl = 0.0;
    spectre->seuil_lumin = 0.0;
    spectre->integrale_lumin_seuil = 0.0;
    for(int i = 0; i < NB_FREQUENCES; i++) spectre->spectre[i] = 0;

    // Calcul du spectre
    UCHAR r, g, b;
    for(int y = 0; y < tuile->nb_lig; y++) {
        for(int x = 0; x < tuile->nb_col; x++) {
            BMP_GetPixelRGB(original, tuile->offset_col + x, tuile->offset_lig + y, &r, &g, &b);
            UCHAR gris = (UCHAR)(0.299*r + 0.587*g + 0.114*b);
            spectre->spectre[gris]++;
        }
    }

    calcul_integrale_seuil_lumin(spectre, 0.0);
    return spectre;
}

BMP *get_bitmap_tuile(BMP *original, const t_tuile *tuile) {
    if(original == NULL || tuile == NULL) return NULL;

    BMP *nouvelle = BMP_Create(tuile->nb_col, tuile->nb_lig, BMP_GetDepth(original));
    if(nouvelle == NULL) return NULL;

    UCHAR r, g, b;
    for(int y = 0; y < tuile->nb_lig; y++) {
        for(int x = 0; x < tuile->nb_col; x++) {
            BMP_GetPixelRGB(original, tuile->offset_col + x, tuile->offset_lig + y, &r, &g, &b);
            BMP_SetPixelRGB(nouvelle, x, y, r, g, b);
        }
    }

    return nouvelle;
}

BMP *get_bitmap_gris_tuile(BMP *original, const t_tuile *tuile, double seuil_lum) {
    if(original == NULL || tuile == NULL || seuil_lum < 0.0 || seuil_lum > 1.0) return NULL;

    BMP *nouvelle = BMP_Create(tuile->nb_col, tuile->nb_lig, BMP_GetDepth(original));
    if(nouvelle == NULL) return NULL;

    UCHAR r, g, b, gris;
    for(int y = 0; y < tuile->nb_lig; y++) {
        for(int x = 0; x < tuile->nb_col; x++) {
            BMP_GetPixelRGB(original, tuile->offset_col + x, tuile->offset_lig + y, &r, &g, &b);
            gris = (UCHAR)(0.299*r + 0.587*g + 0.114*b);
            if((gris/255.0) < seuil_lum) gris = 0;
            BMP_SetPixelRGB(nouvelle, x, y, gris, gris, gris);
        }
    }

    return nouvelle;
}

// Fonctions informatrices
int get_id_tuile(const t_tuile *tu) {
    return tu ? tu->id_enum : -1;
}

void get_offsets_tuile(const t_tuile *tu, int *offset_x, int *offset_y) {
    if(tu && offset_x && offset_y) {
        *offset_x = tu->offset_col;
        *offset_y = tu->offset_lig;
    }
}

double get_integrale_seuil0(const t_spectre_gris *ptr_sp) {
    return ptr_sp ? ptr_sp->integrale_lumin_compl : -1.0;
}

double get_seuil_variant(const t_spectre_gris *ptr_sp) {
    return ptr_sp ? ptr_sp->seuil_lumin : -1.0;
}

double get_integrale_seuil_variant(const t_spectre_gris *ptr_sp) {
    return ptr_sp ? ptr_sp->integrale_lumin_seuil : -1.0;
}

void get_tuile(const t_spectre_gris *ptr_sp, t_tuile *tuile) {
    if(ptr_sp && tuile) *tuile = ptr_sp->tuile;
}

void afficher_spectre(const t_spectre_gris *ptr_sp) {
    if(!ptr_sp) return;

    printf("Spectre de la tuile %d:\n", ptr_sp->tuile.id_enum);
    printf("Integrale complete: %.2f\n", ptr_sp->integrale_lumin_compl);
    if(ptr_sp->seuil_lumin > 0.0) {
        printf("Integrale avec seuil %.2f: %.2f\n",
               ptr_sp->seuil_lumin, ptr_sp->integrale_lumin_seuil);
    }
}