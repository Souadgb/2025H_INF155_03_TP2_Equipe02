//
// Created by souad on 2025-04-12.
//

#ifndef RECOUVREMENT_H
#define RECOUVREMENT_H
#include "tuiles.h"

#endif //RECOUVREMENT_H


typedef struct {
    t_spectre_gris **tab_spectres;  // Tableau dynamique de pointeurs vers des spectres.
    int taille_tab_spectres;        // Nombre total de tuiles (taille du tableau dynamique tab_spectre)
    int largeur_tuile;              // Largeur des tuiles.
    int hauteur_tuile;              // Hauteur des tuiles.
} t_recouvrement;


/*======== Fonctions principales =========*/

// Crée un nouveau recouvrement vide
t_recouvrement *init_recouvrement(int nb_tuiles, int nbcol_tuile, int nblig_tuile);

// Detruit un recouvrement et libere sa memoire
void detruire_recouvrement(t_recouvrement *rec);

// Ajoute un spectre au recouvrement
int ajouter_spectre_rec(t_spectre_gris *ptr_sp, t_recouvrement *rec);

// Recupere le spectre de la k-ieme tuile
t_spectre_gris *get_kieme_ptr_sp(const t_recouvrement *rec, int k);

// Recalcule toutes les integrales avec un nouveau seuil
void calcul_integrales_seuil(t_recouvrement *rec, double seuil);

// Trie les spectres par luminosité (décroissante)
void trier_spectres(t_recouvrement *rec, double seuil);

// Reconstruit l'image finale en conservant les tuiles lumineuses
BMP *reconstruire_image(BMP *original, const t_recouvrement *rec, double prop_garde, double prop_min, char *fichier_log);