#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    MAX_CHAR = 30,
    MAX_ETU = 180,
    NB_UE = 6
};

typedef struct {
    int id;
    char prenom[MAX_CHAR];
    char nom[MAX_CHAR];
} ETUDIANT;

void GererErreur() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inscription(ETUDIANT tab_etu[], int* nb_etu) {
    char prenom[MAX_CHAR], nom[MAX_CHAR];
    scanf("%s %s", prenom, nom); // lecture du prénom et nom sur la même ligne

    // Vérification des doublons (même prénom et même nom)
    for (int i = 0; i < *nb_etu; i++) {
        if (strcmp(tab_etu[i].prenom, prenom) == 0 &&
            strcmp(tab_etu[i].nom, nom) == 0) {
            printf("Nom incorrect\n");
            return;
        }
    }

    // Enregistrement du nouvel étudiant
    if (*nb_etu < MAX_ETU) {
        tab_etu[*nb_etu].id = *nb_etu + 1; // commence à 1
        strcpy(tab_etu[*nb_etu].prenom, prenom);
        strcpy(tab_etu[*nb_etu].nom, nom);
        printf("Inscription enregistree (%d)\n", tab_etu[*nb_etu].id);
        (*nb_etu)++;
    }
    else {
        printf("Nombre maximum d'etudiants atteint\n");
    }
}
// fonction cursus
void cursus(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id); // lecture de l’identifiant

    int trouve = 0;
    for (int i = 0; i < nb_etu; i++) {
        if (tab_etu[i].id == id) {
            trouve = 1;
            printf("%d %s %s\n", tab_etu[i].id, tab_etu[i].prenom, tab_etu[i].nom);
            printf("S1 - ");
            for (int j = 0; j < NB_UE; j++) {
                printf("* (*)");
                if (j < NB_UE - 1)
                    printf(" - ");
            }
            printf(" - en cours\n");
            break;
        }
    }

    if (!trouve)
        printf("Identifiant incorrect\n");
}

int main() {
    ETUDIANT tab_etu[MAX_ETU];
    char saisie[MAX_CHAR];
    int nb_der = 0;
    int nb_etu = 0;

    while (1) {
        scanf("%s", saisie);

        if (strcmp(saisie, "EXIT") == 0) { // EXIT
            break;
        }
        else if (strcmp(saisie, "INSCRIRE") == 0) { // INSCRIRE
            inscription(tab_etu, &nb_etu);
        }
        else if (strcmp(saisie, "CURSUS") == 0) {
            cursus(tab_etu, nb_etu);
        }
        else {
            printf("Commande inconnue\n");
        }
        GererErreur();
    }
    return 0;
}
