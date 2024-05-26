
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "everything_here.h"

// Fonction pour lire un caractère depuis l'entrée utilisateur avec une invite
char lireCaractere(const char *invite) {
    char buffer[100]; // Tampon pour lire l'entrée utilisateur
    while (1) {
        printf("%s", invite); // Affiche l'invite à l'utilisateur
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) { // Lit l'entrée utilisateur
            // Vérifie que l'entrée contient un seul caractère alphabétique
            if (strlen(buffer) == 2 && isalpha(buffer[0])) {
                return toupper(buffer[0]); // Retourne le caractère en majuscule
            } else {
                printf("Entrée invalide. Veuillez entrer un caractère valide.\n");
            }
        }
    }
}

// Fonction pour afficher l'état des salles de concert
void afficherEtatSalles(SalleDeConcert** salles, int nbSalles) {
    for (int i = 0; i < nbSalles; i++) { // Parcourt chaque salle
        int totalSieges = 0; // Initialise le compteur de sièges totaux
        // Calcule le nombre total de sièges dans la salle
        for (int j = 0; j < salles[i]->nbRangees; j++) {
            totalSieges += salles[i]->rangees[j].nbSieges;
        }
        // Obtient le nombre de sièges réservés dans la salle
        int siegesReserves = obtenirNombreSiegesReserves(salles[i]);
        // Calcule le pourcentage de sièges réservés
        float pourcentageReserves = (float)siegesReserves / totalSieges * 100.0;
        printf("Salle %s:\n", salles[i]->nom); // Affiche le nom de la salle
        printf("- Nombre total de sièges: %d\n", totalSieges); // Affiche le nombre total de sièges
        printf("- Nombre de sièges réservés: %d\n", siegesReserves); // Affiche le nombre de sièges réservés
        printf("- Pourcentage de sièges réservés: %.2f%%\n", pourcentageReserves); // Affiche le pourcentage de sièges réservés
    }
}

// Fonction pour créer une nouvelle salle de concert
SalleDeConcert* creerSalleDeConcert(const char* nom, int nbRangeesA, int nbRangeesB, int nbSiegesCategorieA, int nbSiegesCategorieB, int nbSiegesCategorieC) {
    SalleDeConcert* salle = (SalleDeConcert*)malloc(sizeof(SalleDeConcert)); // Alloue de la mémoire pour la nouvelle salle
    salle->nom = strdup(nom); // Duplique le nom de la salle
    salle->nbRangeesCategorieA = nbRangeesA; // Définition du nombre de rangées catégorie A
    salle->nbRangeesCategorieB = nbRangeesB; // Définition du nombre de rangées catégorie B
    salle->nbRangeesCategorieC = 10 - nbRangeesA - nbRangeesB; // Définition du nombre de rangées catégorie C

    // Calculer le nombre total de rangées
    salle->nbRangees = nbRangeesA + nbRangeesB + salle->nbRangeesCategorieC;

    // Allouer de la mémoire pour les rangées
    salle->rangees = (Rangee*)malloc(salle->nbRangees * sizeof(Rangee));
    // Allouer de la mémoire pour les sièges par catégorie
    salle->nbSiegesCategorie = (int*)malloc(3 * sizeof(int));
    salle->nbSiegesCategorie[0] = nbSiegesCategorieA; // Définition du nombre de sièges catégorie A
    salle->nbSiegesCategorie[1] = nbSiegesCategorieB; // Définition du nombre de sièges catégorie B
    salle->nbSiegesCategorie[2] = nbSiegesCategorieC; // Définition du nombre de sièges catégorie C

    int i;
    // Création des rangées pour la catégorie A
    for (i = 0; i < nbRangeesA; i++) {
        salle->rangees[i] = *creerRangee(nbSiegesCategorieA, 'A');
    }
    // Création des rangées pour la catégorie B
    for (; i < nbRangeesA + nbRangeesB; i++) {
        salle->rangees[i] = *creerRangee(nbSiegesCategorieB, 'B');
    }
    // Création des rangées pour la catégorie C
    for (; i < salle->nbRangees; i++) {
        salle->rangees[i] = *creerRangee(nbSiegesCategorieC, 'C');
    }
    salle->aFosse = 0; // Initialise la variable aFosse à 0
    salle->prixCategorieA = 0.0; // Initialise le prix des sièges catégorie A à 0.0
    salle->prixCategorieB = 0.0; // Initialise le prix des sièges catégorie B à 0.0
    salle->prixCategorieC = 0.0; // Initialise le prix des sièges catégorie C à 0.0
    salle->heureFin = 0; // Initialise l'heure de fin du concert à 0
    salle->concertTermine = 0; // Initialise la variable concertTermine à 0
    return salle; // Retourne le pointeur vers la nouvelle salle
}

// Fonction pour libérer la mémoire allouée pour une salle de concert
void libererSalleDeConcert(SalleDeConcert* salle) {
    if (salle) {
        // Libère la mémoire allouée pour chaque rangée
        for (int i = 0; i < salle->nbRangees; i++) {
            libererRangee(&(salle->rangees[i]));
        }
        free(salle->rangees); // Libère la mémoire allouée pour les rangées
        free(salle->nbSiegesCategorie); // Libère la mémoire allouée pour les sièges par catégorie
        free(salle->nom); // Libère la mémoire allouée pour le nom de la salle
        free(salle); // Libère la mémoire allouée pour la salle
    }
}

// Fonction pour sauvegarder les données d'une salle de concert dans un fichier texte
void sauvegarderSalleDeConcert(SalleDeConcert* salle, int id) {
    char filename[100]; // Nom du fichier de sauvegarde
    sprintf(filename, "salle_%d.txt", id); // Crée le nom de fichier avec l'ID de la salle
    FILE* file = fopen(filename, "w"); // Ouvre le fichier en mode écriture
    if (file != NULL) { // Vérifie si le fichier a été ouvert avec succès
        fprintf(file, "%s\n", salle->nom); // Écrit le nom de la salle dans le fichier
        for (int i = 0; i < 10; i++) { // Parcourt chaque rangée de la salle
            for (int j = 0; j < salle->rangees[i].nbSieges; j++) { // Parcourt chaque siège de la rangée
                fprintf(file, "%d ", salle->rangees[i].sieges[j]); // Écrit l'état du siège dans le fichier
            }
            fprintf(file, "\n"); // Ajoute une nouvelle ligne après chaque rangée
        }
        fclose(file); // Ferme le fichier
    }
}

// Fonction pour charger les données d'une salle de concert depuis un fichier texte
SalleDeConcert* chargerSallesDeConcert(const char* filename) {
    FILE* file = fopen(filename, "r"); // Ouvre le fichier en mode lecture
    if (file == NULL) { // Vérifie si le fichier n'a pas pu être ouvert
        return NULL; // Retourne NULL si le fichier n'a pas pu être ouvert
    }
    char nom[100]; // Tampon pour stocker le nom de la salle
    fgets(nom, 100, file); // Lit le nom de la salle depuis le fichier
    nom[strcspn(nom, "\n")] = '\0'; // Supprime le retour à la ligne à la fin du nom

    // Crée une nouvelle salle de concert avec le nom lu depuis le fichier et des valeurs par défaut pour les autres attributs
    SalleDeConcert* salle = creerSalleDeConcert(nom, 3, 4, 10, 8, 6);
    for (int i = 0; i < 10; i++) { // Parcourt chaque rangée de la salle
        for (int j = 0; j < salle->rangees[i].nbSieges; j++) { // Parcourt chaque siège de la rangée
            fscanf(file, "%d", &salle->rangees[i].sieges[j]); // Lit l'état du siège depuis le fichier
        }
    }
    fclose(file); // Ferme le fichier
    return salle; // Retourne un pointeur vers la salle chargée depuis le fichier
}

// Fonction pour modifier la configuration d'une salle de concert avec de nouveaux paramètres
void modifierConfigurationSalle(SalleDeConcert* salle, int nbRangeesA, int nbRangeesB, float prixA, float prixB, float prixC, int aFosse, int heureFin) {
    // Met à jour les attributs de la salle avec les nouveaux paramètres reçus
    salle->nbRangeesCategorieA = nbRangeesA;
    salle->nbRangeesCategorieB = nbRangeesB;
    salle->nbRangeesCategorieC = 10 - nbRangeesA - nbRangeesB;
    salle->prixCategorieA = prixA;
    salle->prixCategorieB = prixB;
    salle->prixCategorieC = prixC;
    salle->aFosse = aFosse;
    salle->heureFin = heureFin;
}

// Fonction pour obtenir le prix d'un siège dans une catégorie spécifiée
float obtenirPrix(SalleDeConcert* salle, char categorie) {
    switch (categorie) {
        case 'A':
            return salle->prixCategorieA;
        case 'B':
            return salle->prixCategorieB;
        case 'C':
            return salle->prixCategorieC;
        default:
            return -1; // Indique une catégorie invalide
    }
}

// Fonction pour réserver un siège dans une salle de concert
void reserverSiege(SalleDeConcert* salle, int rangee, int siege, char categorie) {
    // Ajuste les indices de l'utilisateur (de 1-based à 0-based)
    rangee -= 1;
    siege -= 1;

    if (rangee >= 0 && rangee < salle->nbRangees && siege >= 0 && siege < salle->rangees[rangee].nbSieges) {
        if (salle->rangees[rangee].sieges[siege] == 0) { // Vérifie si le siège est disponible
            float prix = obtenirPrix(salle, categorie); // Obtient le prix du siège dans la catégorie spécifiée
            if (prix >= 0) {
                salle->rangees[rangee].sieges[siege] = 1; // Marque le siège comme réservé
                printf("Siège réservé avec succès. Prix: %.2f euros\n", prix); // Affiche un message de succès avec le prix
            } else {
                printf("Catégorie invalide.\n"); // Affiche un message d'erreur si la catégorie est invalide
            }
        } else {
            printf("Le siège est déjà réservé.\n"); // Affiche un message si le siège est déjà réservé
        }
    } else {
        printf("Numéro de rangée ou de siège invalide.\n"); // Affiche un message si les indices de rangée ou de siège sont invalides
    }
}

// Fonction pour afficher les salles de concert disponibles (avec une heure de fin définie)
void afficherSallesDisponibles(SalleDeConcert** salles, int nbSalles) {
    for (int i = 0; i < nbSalles; i++) { // Parcourt chaque salle de concert
        if (salles[i]->heureFin > 0) { // Vérifie si la salle a une heure de fin définie
            printf("Salle : %s\n", salles[i]->nom); // Affiche le nom de la salle
        }
    }
}
