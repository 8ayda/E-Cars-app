#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VOITURES 100
#define MAX_LOCATIONS 1000

// Structure pour représenter une voiture
typedef struct {
    int id;
    char marque[50];
    char modele[50];
    bool disponible;
    bool enPanne;
    char description[100];
} Voiture;

Voiture voitures[MAX_VOITURES];
int nombreVoitures = 0; // Garder une trace du nombre de voitures actuellement enregistrées

// Structure pour représenter une date
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

// Structure pour représenter une location
typedef struct {
    int idVoiture;
    Date date;
} Location;


Location historique[MAX_LOCATIONS];
int nombreLocations = 0;

Date obtenirDateActuelle() {
    time_t timestamp = time(NULL); // Récupération du timestamp actuel
    struct tm *timeinfo = localtime(&timestamp); // Conversion en structure tm

    Date dateActuelle;

    // Assignation des membres de la structure Date
    dateActuelle.jour = timeinfo->tm_mday;
    dateActuelle.mois = timeinfo->tm_mon + 1;
    dateActuelle.annee = timeinfo->tm_year + 1900;

    return dateActuelle;
}

// Fonction pour lire les donnees du fichier voitures
void lireDonneesVoitures(Voiture *voitures, int *nombreVoitures) {
    FILE *file = fopen("voiture_data.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %s %s %d %d\n", 
                &voitures[*nombreVoitures].id, voitures[*nombreVoitures].marque,
                voitures[*nombreVoitures].modele, voitures[*nombreVoitures].description,
                &voitures[*nombreVoitures].disponible, &voitures[*nombreVoitures].enPanne) != EOF) {
            (*nombreVoitures)++;
        }
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
}

// fonction pour lire les donnees du fichier localisarions
void lireDonneesLocalisations(Location *historique, int *nombreLocations) {
    FILE *file = fopen("locations_data.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %d %d %d\n", 
                &historique[*nombreLocations].idVoiture, 
                &historique[*nombreLocations].date.jour, 
                &historique[*nombreLocations].date.mois, 
                &historique[*nombreLocations].date.annee) != EOF) {
            (*nombreLocations)++;
        }
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
}

// fonction pour rechercher la voiture par ID
int trouverIndexVoitureParID(Voiture *voitures, int nombreVoitures, int idVoiture) {
    for (int i = 0; i < nombreVoitures; i++) {
        if (voitures[i].id == idVoiture) {
            return i; // Retourne l'index de la voiture si l'ID correspond
        }
    }
    return -1; // Retourne -1 si la voiture n'est pas trouvée
}

// Fonction pour louer une voiture
void louerVoiture(Voiture *voitures, int idVoiture, Date date) {

    int index = trouverIndexVoitureParID(voitures, nombreVoitures, idVoiture);
  
    if (index != -1 && voitures[index].disponible && !voitures[index].enPanne) {
        // Marquer la voiture comme non disponible
        voitures[index].disponible = false;

        // Enregistrer la location dans l'historique
        historique[nombreLocations].idVoiture = idVoiture;
        historique[nombreLocations].date = date;
        nombreLocations++;

        printf("Voiture louée avec succès !\n");
    } else {
        printf("Cette voiture n'est pas disponible pour la location.\n");
    }

    FILE *file = fopen("locations_data.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %d %d %d\n", idVoiture, date.jour, date.mois, date.annee);
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
}

// Fonction pour afficher la description d'une voiture
void afficherDescription(Voiture *voitures, int nombreVoitures, int idVoiture){
    int index = trouverIndexVoitureParID(voitures, nombreVoitures, idVoiture);
    if (index != -1) {
        printf("ID: %d\n", voitures[index].id);
        printf("Marque: %s\n", voitures[index].marque);
        printf("Modele: %s\n", voitures[index].modele);
        printf("Description: %s\n", voitures[index].description);
        printf("Disponible: %s\n", voitures[index].disponible ? "Oui" : "Non");
        printf("En panne: %s\n", voitures[index].enPanne ? "Oui" : "Non");
    }else{
        printf("Aucune voiture correspondant à cet ID n'a été trouvée.\n");
    }
}


// Fonction pour supprimer une voiture en panne
void supprimerVoiturePanne(Voiture *voitures, int idVoiture) {

    int index = trouverIndexVoitureParID(voitures, nombreVoitures, idVoiture);

    // Supprimer la voiture du tableau
    if (index != -1) {
        for (int i = index; i < nombreVoitures - 1; i++) {
            voitures[i] = voitures[i + 1];
        }
        nombreVoitures--;

        printf("Voiture en panne supprimée avec succès.\n");
    } else {
        printf("Aucune voiture en panne correspondant à cet ID n'a été trouvée.\n");
    }
}

// Fonction pour modifier la description et l'état d'une voiture
void modifierVoiture(Voiture *voitures, int idVoiture, char nouvelleDescription[100], bool nouvelEtat) {
    
    int index = trouverIndexVoitureParID(voitures, nombreVoitures, idVoiture);

    if (index != -1) {
        // Modifier la description et l'état de la voiture spécifiée
        int j;
        for (j = 0; j < sizeof(voitures[index].description) - 1 && nouvelleDescription[j] != '\0'; j++) {
            voitures[index].description[j] = nouvelleDescription[j];
        }
        voitures[index].description[j] = '\0'; 
        // Mettre à jour l'état de la voiture
        voitures[index].enPanne = nouvelEtat;
        
        printf("Description et état de la voiture modifiés avec succès.\n");

    } else {
        printf("Aucune voiture correspondant à cet ID n'a été trouvée.\n");
    }
}

// Fonction pour afficher l'historique des locations
void afficherHistoriqueLocations(Location *historique, int nombreLocations) {

    // Tri des locations par date 
    for (int i = 0; i < nombreLocations - 1; i++) {
        for (int j = 0; j < nombreLocations - i - 1; j++) {
            if (historique[j].date.annee > historique[j + 1].date.annee ||
                (historique[j].date.annee == historique[j + 1].date.annee &&
                 historique[j].date.mois > historique[j + 1].date.mois) ||
                (historique[j].date.annee == historique[j + 1].date.annee &&
                 historique[j].date.mois == historique[j + 1].date.mois &&
                 historique[j].date.jour > historique[j + 1].date.jour)) {
                Location temp = historique[j];
                historique[j] = historique[j + 1];
                historique[j + 1] = temp;
            }
        }
    }

    // Affichage de l'historique trié
    printf("Historique des locations jour par jour :\n");
    for (int i = 0; i < nombreLocations; i++) {
        printf("ID Voiture: %d | Date: %02d/%02d/%d\n", historique[i].idVoiture,
               historique[i].date.jour, historique[i].date.mois, historique[i].date.annee);
    }

}

// Fonction pour gérer le retour d'une voiture 
void retourVoiture(Voiture *voitures, int idVoiture) {

    int index = trouverIndexVoitureParID(voitures, nombreVoitures, idVoiture);

    if (index != -1) {            
        voitures[index].disponible = true;
        printf("La voiture a été retournée avec succès.\n");
    } else {
        printf("Aucune voiture correspondant à cet ID n'a été trouvée.\n");
    }
}

// fonction pour afficher les voitures disponibles
void afficherVoituresDisponibles(Voiture *voitures, int nombreVoitures) {
    printf("Voitures disponibles :\n");

    for (int i = 0; i < nombreVoitures; i++) {
        if (voitures[i].disponible) {
            printf("ID: %d | Marque: %s | Modele: %s | Description: %s\n",
                   voitures[i].id, voitures[i].marque, voitures[i].modele, voitures[i].description);
        }
    }
}

// Fonction pour ajouter une nouvelle voiture
void ajouterNouvelleVoiture(Voiture *voitures, int *nombreVoitures) {
    if (*nombreVoitures < MAX_VOITURES) {
        Voiture nouvelleVoiture;

        printf("Entrez l'ID de la nouvelle voiture : ");
        scanf("%d", &nouvelleVoiture.id);

        printf("Entrez la marque de la voiture : ");
        scanf("%s", nouvelleVoiture.marque);

        printf("Entrez le modèle de la voiture : ");
        scanf("%s", nouvelleVoiture.modele);

        printf("Entrez une description pour la voiture : ");
        scanf("%s", nouvelleVoiture.description);

        nouvelleVoiture.disponible = true;
        nouvelleVoiture.enPanne = false;

        voitures[*nombreVoitures] = nouvelleVoiture;
        (*nombreVoitures)++;

       
        FILE *file = fopen("car_data.txt", "a");
        if (file != NULL) {
            fprintf(file, "%d %s %s %s %d %d\n", 
                nouvelleVoiture.id, nouvelleVoiture.marque, nouvelleVoiture.modele,
                nouvelleVoiture.description, nouvelleVoiture.disponible, nouvelleVoiture.enPanne);
            fclose(file);
        } else {
            printf("Erreur lors de l'ouverture du fichier.\n");
        }

        printf("Nouvelle voiture ajoutée avec succès !\n");
    } else {
        printf("Impossible d'ajouter une nouvelle voiture, la limite est atteinte.\n");
    }  
}



int main() {
    lireDonneesVoitures(voitures, &nombreVoitures);
    lireDonneesVoitures(voitures, &nombreVoitures);
    int choix = 0;

    do {
        printf("\n======================= Menu =======================\n");
        printf("1. Louer une voiture\n");
        printf("2. Afficher les voitures disponibles\n");
        printf("3. Afficher la description d'une voiture\n");
        printf("4. Supprimer une voiture en panne\n");
        printf("5. Modifier la description et l'état d'une voiture\n");
        printf("6. Afficher l'historique des locations\n");
        printf("7. Retour d'une voiture \n");
        printf("8. ajouter une nouvelle voiture\n");
        printf("9. Quitter\n");
        printf("\n====================================================\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Entrez l'ID de la voiture que vous souhaitez louer : ");
                int idLouer; 
                scanf("%d", &idLouer);
                louerVoiture(voitures, idLouer, obtenirDateActuelle()); 
                break;
            
            case 2:
                afficherVoituresDisponibles(voitures, nombreVoitures);
                break;

            case 3:
                printf("entrez l'ID de la voiture dont vous souhaitez afficher la discription: ");
                int idDescription; 
                scanf("%d", &idDescription);
                afficherDescription(voitures, nombreVoitures, idDescription);
                break;

            case 4:
                printf("Entrez l'ID de la voiture en panne que vous souhaitez supprimer : ");
                int idSupprimer; 
                scanf("%d", &idSupprimer);
                supprimerVoiturePanne(voitures, idSupprimer);
                break;

            case 5:
                printf("Entrez l'ID de la voiture que vous souhaitez modifier : ");
                int idModifier; 
                scanf("%d", &idModifier);

                char newDescription[100];
                printf("Entrez la nouvelle description : ");
                scanf("%s", newDescription);

                printf("Entrez le nouvel état (0 pour non disponible, 1 pour disponible) : ");
                int newState;
                scanf("%d", &newState);

                modifierVoiture(voitures, idModifier, newDescription, newState);
                break;

            case 6:
                afficherHistoriqueLocations(historique, nombreLocations);
                break;

            case 7:
                printf("Entrez l'ID de la voiture à retourner : ");
                int idReclamation; 
                scanf("%d", &idReclamation);
                retourVoiture(voitures, idReclamation);
                break;

            case 8:
                ajouterNouvelleVoiture(voitures, &nombreVoitures);
                break;

            case 9:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide. Veuillez choisir une option valide.\n");
                break;
        }
    } while (choix != 9);

    return 0;
}


