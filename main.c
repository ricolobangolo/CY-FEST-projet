#include <stdio.h> // Inclusion de la bibliothèque standard d'entrée/sortie
#include <stdlib.h> // Inclusion de la bibliothèque standard pour les fonctions utilitaires
#include <string.h> // Inclusion de la bibliothèque pour les manipulations de chaînes de caractères
#include <time.h> // Inclusion de la bibliothèque pour la gestion du temps
#include "everything_here.h" // Inclusion du fichier d'en-tête personnalisé

// Couleurs ANSI pour l'affichage coloré dans le terminal
#define ANSI_RESET "\x1b[0m" // Réinitialiser la couleur
#define ANSI_GREEN "\x1b[32m" // Couleur verte
#define ANSI_YELLOW "\x1b[33m" // Couleur jaune
#define ANSI_RED "\x1b[31m" // Couleur rouge

#define MAX_TRIES 3 // Définition du nombre maximum de tentatives pour entrer le mot de passe

char motDePasseManager[100] = "admin"; // Mot de passe initial par défaut

int main() {
    menuPrincipal(); // Appel de la fonction du menu principal
    return 0; // Fin du programme principal
}

// Fonction pour afficher l'heure de fin de concert au format HH:MM
void afficherHeureFinConcert(int heureFin) {
    int heures = heureFin / 100; // Extraire les heures
    int minutes = heureFin % 100; // Extraire les minutes
    printf("Heure de fin de concert: %02d:%02d\n", heures, minutes); // Afficher l'heure de fin
}

// Vérifie si le concert est terminé en fonction de l'heure actuelle
void verifierFinConcert(SalleDeConcert* salle, int heureActuelle) {
    if (heureActuelle >= salle->heureFin && !salle->concertTermine) {
        viderSieges(salle); // Vide les sièges si le concert est terminé
        salle->concertTermine = 1; // Marque le concert comme terminé
    }
}

// Menu pour les festivaliers avec plusieurs options
void menuFestivalier(SalleDeConcert** salles, int nbSalles) {
    int choix; // Variable pour stocker le choix de l'utilisateur
    do {
        printf("\nMenu Festivalier:\n");
        printf("1. Afficher les plans de toutes les salles\n");
        printf("2. Réserver un siège\n");
        printf("3. Vérifier l'état du concert\n");
        printf("4. Retour au menu principal\n");
        choix = lireEntier("Choisissez une option: "); // Lire le choix de l'utilisateur

        switch (choix) {
            case 1:
                // Affiche les plans de toutes les salles
                for (int i = 0; i < nbSalles; i++) {
                    afficherPlanSalle(salles[i]); // Appel de la fonction pour afficher le plan de la salle
                }
                break;
            case 2: {
                // Réserve un siège dans une salle spécifique
                int salleIndex = lireEntier("Choisissez une salle pour réserver un siège (0 à nbSalles-1): ");
                if (salleIndex >= 0 && salleIndex < nbSalles) {
                    int rangee = lireEntier("Choisissez une rangée: ");
                    int siege = lireEntier("Choisissez un siège: ");
                    printf("Choisissez une catégorie (A, B ou C): ");
                    char categorie;
                    scanf(" %c", &categorie); // Lire un caractère pour la catégorie
                    reserverSiege(salles[salleIndex], rangee, siege, categorie); // Réserver le siège
                } else {
                    printf("Numéro de salle invalide.\n");
                }
                break;
            }
            case 3: {
                // Vérifie l'état du concert dans une salle spécifique
                int salleIndex = lireEntier("Choisissez une salle pour vérifier l'état du concert (0 à nbSalles-1): ");
                if (salleIndex >= 0 && salleIndex < nbSalles) {
                    int heureActuelle = obtenirHeureActuelle(); // Obtenir l'heure actuelle
                    verifierFinConcert(salles[salleIndex], heureActuelle); // Vérifier si le concert est terminé
                    afficherHeureFinConcert(salles[salleIndex]->heureFin); // Afficher l'heure de fin du concert
                } else {
                    printf("Numéro de salle invalide.\n");
                }
                break;
            }
            case 4:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir à nouveau.\n");
                break;
        }
    } while (choix != 4); // Répéter le menu tant que l'utilisateur ne choisit pas de revenir au menu principal
}

// Gère la configuration de la fosse dans une salle de concert
void gererFosse(SalleDeConcert* salle, int aFosse) {
    if (aFosse) {
        // Double le nombre de sièges dans chaque rangée de catégorie A pour simuler une fosse
        for (int i = 0; i < salle->nbRangeesCategorieA; i++) {
            free(salle->rangees[i].sieges); // Libère la mémoire des sièges actuels
            salle->rangees[i].sieges = (int*)malloc(sizeof(int) * salle->rangees[i].nbSieges * 2); // Alloue de la mémoire pour les nouveaux sièges
            salle->rangees[i].nbSieges *= 2; // Double le nombre de sièges
            memset(salle->rangees[i].sieges, 0, sizeof(int) * salle->rangees[i].nbSieges); // Initialise tous les sièges à 0 (non réservés)
        }
    }
}

// Obtient l'heure actuelle au format HHMM
int obtenirHeureActuelle() {
    time_t now; // Variable pour stocker le temps actuel
    time(&now); // Obtenir le temps actuel
    struct tm *local = localtime(&now); // Convertir le temps actuel en heure locale
    return local->tm_hour * 100 + local->tm_min; // Retourner l'heure actuelle au format HHMM
}

// Lit et renvoie un entier saisi par l'utilisateur avec un message d'invite
int lireEntier(const char *invite) {
    char buffer[100]; // Buffer pour stocker l'entrée de l'utilisateur
    int valeur; // Variable pour stocker la valeur entière lue
    while (1) {
        printf("%s", invite); // Afficher le message d'invite
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valeur) == 1) {
                return valeur; // Retourner la valeur entière lue
            } else {
                printf("Entrée invalide. Veuillez entrer un nombre entier.\n");
            }
        }
    }
}

// Lit et renvoie un nombre flottant saisi par l'utilisateur avec un message d'invite
float lireFlottant(const char *invite) {
    float valeur; // Variable pour stocker la valeur flottante lue
    char buffer[100]; // Buffer pour stocker l'entrée de l'utilisateur
    while (1) {
        printf("%s", invite); // Afficher le message d'invite
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &valeur) == 1) {
                return valeur; // Retourner la valeur flottante lue
            }
        }
        printf("Entrée invalide. Veuillez entrer un nombre flottant.\n");
    }
}

// Vide tous les sièges d'une salle de concert (met les sièges à 0)
void viderSieges(SalleDeConcert* salle) {
    for (int i = 0; i < salle->nbRangees; i++) {
        memset(salle->rangees[i].sieges, 0, sizeof(int) * salle->rangees[i].nbSieges); // Mettre tous les sièges à 0 (non réservés)
    }
}

void afficherPlanSalle(SalleDeConcert* salle) {
    // Affiche le plan de la salle de concert avec le nom de la salle
    printf("🤘 Plan de la %s 🤘\n", salle->nom);
    // Affiche une représentation de la scène
    printf("🎵 SCENE 🎵\n");
    // Séparateur visuel
    printf("----------------------------------\n");

    // Parcourt toutes les rangées de la salle
    for (int i = 0; i < salle->nbRangees; i++) {
        // Affiche l'indice de la rangée
        printf("R%2d ", i + 1);
        // Parcourt tous les sièges de la rangée
        for (int j = 0; j < salle->rangees[i].nbSieges; j++) {
            // Si le siège est réservé, affiche un 'X' rouge
            if (salle->rangees[i].sieges[j] == 1) {
                printf(ANSI_RED "X " ANSI_RESET);
            } else {
                // Sinon, affiche 'O' avec la couleur en fonction de la catégorie
                if (i < salle->nbRangeesCategorieA) {
                    printf(ANSI_GREEN "O " ANSI_RESET);
                } else if (i < salle->nbRangeesCategorieA + salle->nbRangeesCategorieB) {
                    printf(ANSI_YELLOW "O " ANSI_RESET);
                } else {
                    printf("O ");
                }
            }
        }
        printf("\n"); // Nouvelle ligne après chaque rangée
    }

    // Affiche la légende pour expliquer les symboles utilisés
    printf("\nLégende:\n");
    printf(ANSI_GREEN "O" ANSI_RESET " - Catégorie A (%.2f euros)\n", salle->prixCategorieA);
    printf(ANSI_YELLOW "O" ANSI_RESET " - Catégorie B (%.2f euros)\n", salle->prixCategorieB);
    printf("O - Catégorie C (%.2f euros)\n", salle->prixCategorieC);
    printf(ANSI_RED "X" ANSI_RESET " - Siège réservé\n");

    // Appelle la fonction pour afficher l'heure de fin du concert
    afficherHeureFinConcert(salle->heureFin);
}

void menuPrincipal() {
    SalleDeConcert* salles[10]; // Tableau pour stocker les salles de concert
    int nbSalles = 0; // Compteur pour le nombre de salles

    // Charger les salles existantes depuis un fichier
    salles[nbSalles] = chargerSallesDeConcert("concerts.txt");
    if (salles[nbSalles] != NULL) {
        nbSalles++; // Incrémente le compteur si une salle est chargée avec succès
    } else {
        // Crée une salle par défaut si le chargement échoue
        salles[nbSalles] = creerSalleDeConcert("Main Stage", 3, 4, 10, 8, 6);
        nbSalles++; // Incrémente le compteur après avoir créé une salle par défaut
    }

    int choix; // Variable pour stocker le choix de l'utilisateur
    do {
        // Affiche le menu principal avec les options disponibles
        printf("\nMenu Principal:\n");
        printf("1. Mode Manager\n");
        printf("2. Mode Festivalier\n");
        printf("3. Quitter\n");
        choix = lireEntier("Choisissez une option: "); // Lit le choix de l'utilisateur

        switch (choix) {
            case 1:
                // Si le mot de passe est vérifié avec succès, affiche le menu manager
                if (verifierMotDePasse()) {
                    menuManager(salles, &nbSalles);
                } else {
                    // Affiche un message d'erreur en cas d'échec de vérification du mot de passe
                    printf("Accès refusé. Trop de tentatives échouées.\n");
                }
                break;
            case 2:
                // Affiche le menu festivalier
                menuFestivalier(salles, nbSalles);
                break;
            case 3:
                // Affiche un message d'au revoir et quitte le programme
                printf("Au revoir !\n");
                break;
            default:
                // Affiche un message d'erreur en cas de choix invalide
                printf("Choix invalide. Veuillez choisir à nouveau.\n");
                break;
        }
    } while (choix != 3); // Répète le menu tant que l'utilisateur ne choisit pas de quitter

    // Libère la mémoire associée aux salles de concert
    for (int i = 0; i < nbSalles; i++) {
        libererSalleDeConcert(salles[i]);
    }
}

int verifierMotDePasse() {
    char motDePasseEntree[100]; // Variable pour stocker le mot de passe entré par l'utilisateur
    int essais = 0; // Compteur pour le nombre de tentatives

    while (essais < MAX_TRIES) {
        // Demande à l'utilisateur d'entrer le mot de passe
        printf("Entrez le mot de passe pour le mode manager: ");
        if (fgets(motDePasseEntree, sizeof(motDePasseEntree), stdin) != NULL) {
            // Enlève le retour à la ligne en fin de chaîne
            motDePasseEntree[strcspn(motDePasseEntree, "\n")] = '\0';

            if (strlen(motDePasseEntree) == 0) {
                // Affiche un message d'erreur si le mot de passe est vide
                printf("Le mot de passe ne peut pas être vide. Veuillez réessayer.\n");
            } else if (strcmp(motDePasseEntree, motDePasseManager) == 0) {
                return 1; // Retourne 1 si le mot de passe est correct
            } else {
                // Incrémente le compteur de tentatives et affiche un message d'erreur
                essais++;
                printf("Mot de passe incorrect. Vous avez %d tentatives restantes.\n", MAX_TRIES - essais);
            }
        } else {
            // Affiche un message d'erreur en cas de problème de lecture du mot de passe
            printf("Erreur lors de la lecture du mot de passe. Veuillez réessayer.\n");
        }
    }

    return 0; // Retourne 0 après trop de tentatives échouées
}

void changerMotDePasse() {
    // Déclaration d'une variable pour stocker le nouveau mot de passe
    char nouveauMotDePasse[100];
    // Demande à l'utilisateur d'entrer le nouveau mot de passe
    printf("Entrez le nouveau mot de passe: ");
    // Vérifie si l'entrée utilisateur a été lue correctement
    if (fgets(nouveauMotDePasse, sizeof(nouveauMotDePasse), stdin) != NULL) {
        // Enlève le retour à la ligne en fin de chaîne
        nouveauMotDePasse[strcspn(nouveauMotDePasse, "\n")] = '\0';
        // Vérifie si le mot de passe n'est pas vide
        if (strlen(nouveauMotDePasse) > 0) {
            // Copie le nouveau mot de passe dans la variable globale
            strcpy(motDePasseManager, nouveauMotDePasse);
            // Affiche un message de succès
            printf("Mot de passe changé avec succès.\n");
        } else {
            // Affiche un message d'erreur si le mot de passe est vide
            printf("Le mot de passe ne peut pas être vide. Mot de passe non changé.\n");
        }
    } else {
        // Affiche un message d'erreur en cas de problème de lecture
        printf("Erreur lors de la lecture du nouveau mot de passe. Mot de passe non changé.\n");
    }
}

void menuManager(SalleDeConcert** salles, int* nbSalles) {
    int choix; // Variable pour stocker le choix de l'utilisateur
    do {
        // Affiche le menu manager avec les options disponibles
        printf("\nMenu Manager:\n");
        printf("1. Créer une nouvelle salle de concert\n");
        printf("2. Attribuer un concert à une salle existante\n");
        printf("3. Modifier une salle de concert existante\n");
        printf("4. Sauvegarder toutes les salles\n");
        printf("5. Observer l’état des différentes salles\n");
        printf("6. Changer le mot de passe\n");
        printf("7. Retour au menu principal\n");
        choix = lireEntier("Choisissez une option: "); // Lit le choix de l'utilisateur

        switch (choix) {
            case 1: {
                // Déclarations de variables pour stocker les détails de la nouvelle salle
                char nom[100];
                int nbRangeesA, nbRangeesB, nbSiegesCategorieA, nbSiegesCategorieB, nbSiegesCategorieC;
                // Demande à l'utilisateur d'entrer les détails de la nouvelle salle
                printf("Nom de la salle: ");
                scanf("%s", nom);
                nbRangeesA = lireEntier("Nombre de rangées en catégorie A: ");
                nbRangeesB = lireEntier("Nombre de rangées en catégorie B: ");
                nbSiegesCategorieA = lireEntier("Nombre de sièges par rangée en catégorie A: ");
                nbSiegesCategorieB = lireEntier("Nombre de sièges par rangée en catégorie B: ");
                nbSiegesCategorieC = lireEntier("Nombre de sièges par rangée en catégorie C: ");

                // Crée une nouvelle salle de concert avec les détails fournis
                salles[*nbSalles] = creerSalleDeConcert(nom, nbRangeesA, nbRangeesB, nbSiegesCategorieA, nbSiegesCategorieB, nbSiegesCategorieC);
                if (salles[*nbSalles] != NULL) {
                    (*nbSalles)++; // Incrémente le compteur de salles si la création est réussie
                    printf("Salle de concert créée avec succès.\n");
                    // Sauvegarde la nouvelle salle dans un fichier
                    sauvegarderSalleDeConcert(salles[*nbSalles - 1], *nbSalles);
                } else {
                    // Affiche un message d'erreur en cas d'échec de création
                    printf("Erreur lors de la création de la salle de concert.\n");
                }
                break;
            }
            case 2: {
                // Demande à l'utilisateur de choisir une salle existante pour attribuer un concert
                int salleIndex = lireEntier("Entrez l'index de la salle à attribuer un concert: ");
                if (salleIndex >= 0 && salleIndex < *nbSalles) {
                    // Demande à l'utilisateur les détails du concert
                    int aFosse = lireEntier("Le concert aura-t-il une fosse ? (1 pour oui, 0 pour non): ");
                    float prixCategorieA = lireFlottant("Prix des sièges catégorie A: ");
                    float prixCategorieB = lireFlottant("Prix des sièges catégorie B: ");
                    float prixCategorieC = lireFlottant("Prix des sièges catégorie C: ");
                    int heureFin = lireEntier("Heure de fin du concert (HHMM): ");
                    // Modifie la configuration de la salle avec les détails du concert
                    modifierConfigurationSalle(salles[salleIndex], salles[salleIndex]->nbRangeesCategorieA, salles[salleIndex]->nbRangeesCategorieB, prixCategorieA, prixCategorieB, prixCategorieC, aFosse, heureFin);
                    // Gère la fosse si nécessaire
                    gererFosse(salles[salleIndex], aFosse);
                    printf("Concert attribué avec succès à la salle %s.\n", salles[salleIndex]->nom);
                } else {
                    // Affiche un message d'erreur si l'index de la salle est invalide
                    printf("Index de salle invalide.\n");
                }
                break;
            }
            case 3: {
                // Demande à l'utilisateur de choisir une salle existante à modifier
                int salleIndex = lireEntier("Entrez l'index de la salle à modifier: ");
                if (salleIndex >= 0 && salleIndex < *nbSalles) {
                    // Demande à l'utilisateur les nouveaux détails de la salle
                    int nbRangeesA = lireEntier("Nombre de rangées en catégorie A: ");
                    int nbRangeesB = lireEntier("Nombre de rangées en catégorie B: ");
                    float prixCategorieA = lireFlottant("Prix des sièges catégorie A: ");
                    float prixCategorieB = lireFlottant("Prix des sièges catégorie B: ");
                    float prixCategorieC = lireFlottant("Prix des sièges catégorie C: ");
                    int aFosse = lireEntier("Le concert aura-t-il une fosse ? (1 pour oui, 0 pour non): ");
                    int heureFin = lireEntier("Heure de fin du concert (HHMM): ");
                    // Modifie la configuration de la salle avec les nouveaux détails
                    modifierConfigurationSalle(salles[salleIndex], nbRangeesA, nbRangeesB, prixCategorieA, prixCategorieB, prixCategorieC, aFosse, heureFin);
                    // Gère la fosse si nécessaire
                    gererFosse(salles[salleIndex], aFosse);
                    printf("Salle de concert modifiée avec succès.\n");
                } else {
                    // Affiche un message d'erreur si l'index de la salle est invalide
                    printf("Index de salle invalide.\n");
                }
                break;
            }
            case 4:
                // Sauvegarde toutes les salles de concert
                for (int i = 0; i < *nbSalles; i++) {
                    sauvegarderSalleDeConcert(salles[i], i + 1);
                }
                printf("Toutes les salles de concert ont été sauvegardées avec succès.\n");
                break;
            case 5:
                // Affiche l'état de toutes les salles de concert
                for (int i = 0; i < *nbSalles; i++) {
                    printf("Salle %s\n", salles[i]->nom);
                    afficherPlanSalle(salles[i]);
                    int totalSieges = salles[i]->nbRangees * salles[i]->rangees[0].nbSieges; // Calcul du nombre total de sièges
                    int siegesReserves = 0;
                    // Parcourt tous les sièges pour compter les sièges réservés
                    for (int j = 0; j < salles[i]->nbRangees; j++) {
                        for (int k = 0; k < salles[i]->rangees[j].nbSieges; k++) {
                            if (salles[i]->rangees[j].sieges[k] == 1) {
                                siegesReserves++;
                            }
                        }
                    }
                    // Affiche le ratio des sièges réservés par rapport au nombre total de sièges
                    printf("Ratio sièges réservés / nombre total de sièges: %d / %d\n", siegesReserves, totalSieges);
                    // Affiche l'heure de fin du concert
                    afficherHeureFinConcert(salles[i]->heureFin);
                }
                break;
            case 6:
                // Change le mot de passe du manager
                changerMotDePasse();
                printf("Retour au menu principal...\n");
                break;
            case 7:
                // Retourne au menu principal
                printf("Retour au menu principal...\n");
                break;
            default:
                // Affiche un message d'erreur en cas de choix invalide
                printf("Choix invalide. Veuillez choisir à nouveau.\n");
                break;
        }
    } while (choix != 7); // Répète le menu tant que l'utilisateur ne choisit pas de retourner au menu principal
}
