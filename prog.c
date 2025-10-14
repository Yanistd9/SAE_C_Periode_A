#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    MAX_CHAR = 30,
    MAX_ETU = 180
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
    scanf("%s %s", prenom, nom); // lecture du pr�nom et nom sur la m�me ligne

    // V�rification des doublons (m�me pr�nom et m�me nom)
    for (int i = 0; i < *nb_etu; i++) {
        if (strcmp(tab_etu[i].prenom, prenom) == 0 &&
            strcmp(tab_etu[i].nom, nom) == 0) {
            printf("Nom incorrect\n");
            return;
        }
    }

    // Enregistrement du nouvel �tudiant
    if (*nb_etu < MAX_ETU) {
        tab_etu[*nb_etu].id = *nb_etu + 1; // commence � 1
        strcpy(tab_etu[*nb_etu].prenom, prenom);
        strcpy(tab_etu[*nb_etu].nom, nom);
        printf("Inscription enregistree (%d)\n", tab_etu[*nb_etu].id);
        (*nb_etu)++;
    }
    else {
        printf("Nombre maximum d'etudiants atteint\n");
    }
}

int main() {
    ETUDIANT tab_etu[MAX_ETU];
    char saisie[MAX_CHAR];
    int nb_der = 0;
    int nb_etu = 0;

    while (nb_etu < MAX_ETU) {
        scanf("%s", saisie);

        if (strcmp(saisie, "EXIT") == 0) { // EXIT
            break;
        }
        else if (strcmp(saisie, "INSCRIRE") == 0) { // INSCRIRE
            inscription(tab_etu, &nb_etu);
        }
        else {
            printf("Commande inconnue\n");
        }
        GererErreur();
    }
    return 0;
}