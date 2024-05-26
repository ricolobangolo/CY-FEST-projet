#include <stdlib.h>
#include <string.h>
#include "everything_here.h"

// Fonction pour obtenir le nombre de sièges réservés dans une salle de concert
int obtenirNombreSiegesReserves(SalleDeConcert* salle) {
    int siegesReserves = 0; // Initialisation du compteur de sièges réservés à 0
    // Parcourt chaque rangée de la salle
    for (int i = 0; i < salle->nbRangees; i++) {
        // Parcourt chaque siège de la rangée
        for (int j = 0; j < salle->rangees[i].nbSieges; j++) {
            // Vérifie si le siège est réservé (valeur de 1)
            if (salle->rangees[i].sieges[j] == 1) {
                siegesReserves++; // Incrémente le compteur si le siège est réservé
            }
        }
    }
    return siegesReserves; // Retourne le nombre total de sièges réservés
}

// Fonction pour créer une nouvelle rangée avec un nombre spécifié de sièges et une catégorie
Rangee* creerRangee(int nbSieges, char categorie) {
    // Allocation de mémoire pour une nouvelle rangée
    Rangee* rangee = (Rangee*)malloc(sizeof(Rangee));
    rangee->nbSieges = nbSieges; // Définition du nombre de sièges
    rangee->categorie = categorie; // Définition de la catégorie
    // Allocation de mémoire pour le tableau des sièges, initialisés à 0
    rangee->sieges = (int*)calloc(nbSieges, sizeof(int));
    return rangee; // Retourne le pointeur vers la nouvelle rangée
}

// Fonction pour libérer la mémoire allouée pour une rangée
void libererRangee(Rangee* rangee) {
    if (rangee) {
        // Libère la mémoire allouée pour le tableau des sièges
        free(rangee->sieges);
        // Libère la mémoire allouée pour la structure de la rangée elle-même
        free(rangee);
    }
}
