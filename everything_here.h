#ifndef EVERYTHING_HERE_H
#define EVERYTHING_HERE_H

// Définition de la structure Rangee représentant une rangée de sièges
typedef struct {
    int* sieges; // Tableau représentant l'état de chaque siège dans la rangée
    int nbSieges; // Nombre total de sièges dans la rangée
    char categorie; // Catégorie de la rangée (A, B, ou C)
} Rangee;

// Définition de la structure SalleDeConcert représentant une salle de concert
typedef struct {
    char* nom; // Nom de la salle
    Rangee* rangees; // Tableau des rangées de sièges dans la salle
    int nbRangees; // Nombre total de rangées dans la salle
    int* nbSiegesCategorie; // Tableau contenant le nombre de sièges par catégorie (A, B, C)
    int nbRangeesCategorieA; // Nombre de rangées dans la catégorie A
    int nbRangeesCategorieB; // Nombre de rangées dans la catégorie B
    int nbRangeesCategorieC; // Nombre de rangées dans la catégorie C
    float prixCategorieA; // Prix des sièges dans la catégorie A
    float prixCategorieB; // Prix des sièges dans la catégorie B
    float prixCategorieC; // Prix des sièges dans la catégorie C
    int aFosse; // Indique si la salle a une fosse (1 pour oui, 0 pour non)
    int heureFin; // Heure de fin du concert (format HHMM)
    int concertTermine; // Indique si le concert dans la salle est terminé (1 pour oui, 0 pour non)
} SalleDeConcert;

// Prototypes des fonctions
void afficherHeureFinConcert(int heureFin);
void menuPrincipal();
void menuManager(SalleDeConcert** salles, int* nbSalles);
void menuFestivalier(SalleDeConcert** salles, int nbSalles);
void gererFosse(SalleDeConcert* salle, int aFosse);
int obtenirHeureActuelle();
int lireEntier(const char *invite);
float lireFlottant(const char *invite);
void verifierFinConcert(SalleDeConcert* salle, int heureActuelle);
void viderSieges(SalleDeConcert* salle);
void afficherPlanSalle(SalleDeConcert* salle);
Rangee* creerRangee(int nbSieges, char categorie);
void libererRangee(Rangee* rangee);
char lireCaractere(const char *invite);
SalleDeConcert* creerSalleDeConcert(const char* nom, int nbRangeesA, int nbRangeesB, int nbSiegesCategorieA, int nbSiegesCategorieB, int nbSiegesCategorieC);
void libererSalleDeConcert(SalleDeConcert* salle);
void sauvegarderSalleDeConcert(SalleDeConcert* salle, int id);
SalleDeConcert* chargerSallesDeConcert(const char* filename);
void modifierConfigurationSalle(SalleDeConcert* salle, int nbRangeesA, int nbRangeesB, float prixA, float prixB, float prixC, int aFosse, int heureFin);
void reserverSiege(SalleDeConcert* salle, int rangee, int siege, char categorie);
void afficherSallesDisponibles(SalleDeConcert** salles, int nbSalles);
void verifierFinConcert(SalleDeConcert* salle, int heureActuelle);
void viderSieges(SalleDeConcert* salle);
int obtenirNombreSiegesReserves(SalleDeConcert* salle);

#endif
