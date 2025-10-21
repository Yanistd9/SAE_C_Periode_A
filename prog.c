#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    MAX_CHAR_ETU = 30,
    MAX_NB_ETU = 180,
    NB_UE = 6
};

typedef struct {
    int id;
    char prenom[MAX_CHAR_ETU];
    char nom[MAX_CHAR_ETU];
    float notes[NB_UE];
    int semestre;
    char statut[15];
} ETUDIANT;

// Permet d'ignorer les caractères restants sur la ligne
void GererErreur() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// INSCRIRE : ajoute un étudiant
void inscription(ETUDIANT tab_etu[], int* nb_etu) {
    char prenom[MAX_CHAR_ETU], nom[MAX_CHAR_ETU];
    scanf("%s %s", prenom, nom);

    // Vérifie doublon
    for (int i = 0; i < *nb_etu; i++) {
        if (strcmp(tab_etu[i].prenom, prenom) == 0 &&
            strcmp(tab_etu[i].nom, nom) == 0) {
            printf("Nom incorrect\n");
            return;
        }
    }

    // Enregistre l'étudiant
    if (*nb_etu < MAX_NB_ETU) {
        tab_etu[*nb_etu].id = *nb_etu + 1;
        strcpy(tab_etu[*nb_etu].prenom, prenom);
        strcpy(tab_etu[*nb_etu].nom, nom);
        for (int i = 0; i < NB_UE; i++)
            tab_etu[*nb_etu].notes[i] = -1;

        tab_etu[*nb_etu].semestre = 1;           // S1 au départ
        strcpy(tab_etu[*nb_etu].statut, "en cours");

        printf("Inscription enregistree (%d)\n", tab_etu[*nb_etu].id);
        (*nb_etu)++;
    }
    else {
        printf("Nombre maximum d'etudiants atteint\n");
    }
}

// NOTE : attribue une note si l'étudiant est en cours
void note(ETUDIANT tab_etu[], int nb_etu) {
    int id, ue;
    float valeur;
    scanf("%d %d %f", &id, &ue, &valeur);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }
    if (ue < 1 || ue > NB_UE) {
        printf("UE incorrect\n");
        return;
    }
    if (valeur < 0 || valeur > 20) {
        printf("Note incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];

    if (strcmp(e->statut, "en cours") != 0) {
        printf("Etudiant hors formation\n");
        return;
    }

    e->notes[ue - 1] = valeur;
    printf("Note enregistree\n");
}

// DEMISSION : statut -> DEM
void demission(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];

    if (strcmp(e->statut, "en cours") == 0) {
        strcpy(e->statut, "demission");
    }
    else {
        printf("Demission impossible\n");
    }
}

// DEFAILLANCE : statut -> DEF
void defaillance(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];

    if (strcmp(e->statut, "en cours") == 0) {
        strcpy(e->statut, "defaillance");
    }
    else {
        printf("Defaillance impossible\n");
    }
}

// JURY : calcule le statut des étudiants (semestre impair)
void jury(ETUDIANT tab_etu[], int nb_etu) {
    for (int i = 0; i < nb_etu; i++) {
        ETUDIANT* e = &tab_etu[i];

        // Ne traite que les étudiants "en cours"
        if (strcmp(e->statut, "en cours") == 0) {
            int nb_notes = 0;
            float somme = 0.0;

            for (int j = 0; j < NB_UE; j++) {
                if (e->notes[j] >= 0) {
                    nb_notes++;
                    somme += e->notes[j];
                }
            }

            if (nb_notes < NB_UE) {
                // Pas toutes les notes => reste "en cours"
                continue;
            }

            float moyenne = somme / NB_UE;
            if (moyenne >= 10) {
                strcpy(e->statut, "ADM");
                e->semestre++;  // Passe au semestre suivant
            }
            else {
                strcpy(e->statut, "AJ");
            }
        }
    }
}

// ETUDIANTS : affiche tous les étudiants
void etudiants(ETUDIANT tab_etu[], int nb_etu) {
    for (int i = 0; i < nb_etu; ++i) {
        printf("%d - %s %s - S%d - %s\n",
            tab_etu[i].id,
            tab_etu[i].prenom,
            tab_etu[i].nom,
            tab_etu[i].semestre,
            tab_etu[i].statut);
    }
}

// CURSUS : affiche le cursus d'un étudiant
void cursus(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];

    printf("%d %s %s\n", e->id, e->prenom, e->nom);
    printf("S%d - ", e->semestre);

    for (int j = 0; j < NB_UE; j++) {
        if (e->notes[j] >= 0)
            printf("%.1f (*)", e->notes[j]);
        else
            printf("* (*)");

        if (j < NB_UE - 1)
            printf(" - ");
    }

    printf(" - %s\n", e->statut);
}

int main() {
    ETUDIANT tab_etu[MAX_NB_ETU];
    char saisie[MAX_CHAR_ETU];
    int nb_etu = 0;

    while (1) {
        scanf("%s", saisie);

        if (strcmp(saisie, "EXIT") == 0) {
            break;
        }
        else if (strcmp(saisie, "INSCRIRE") == 0) {
            inscription(tab_etu, &nb_etu);
        }
        else if (strcmp(saisie, "NOTE") == 0) {
            note(tab_etu, nb_etu);
        }
        else if (strcmp(saisie, "CURSUS") == 0) {
            cursus(tab_etu, nb_etu);
        }
        else if (strcmp(saisie, "ETUDIANTS") == 0) {
            etudiants(tab_etu, nb_etu);
        }
        else if (strcmp(saisie, "DEMISSION") == 0) {
            demission(tab_etu, nb_etu);
        }
        else if (strcmp(saisie, "DEFAILLANCE") == 0) {
            defaillance(tab_etu, nb_etu);
        }
        else if (strcmp(saisie, "JURY") == 0) {
            jury(tab_etu, nb_etu);
        }
        else {
            printf("Commande inconnue\n");
        }
        GererErreur();
    }
    return 0;
}
