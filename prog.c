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

// permet d'afficher qu'une seule fois le message "commande inconnue" quand tout est entré sur la même ligne
void GererErreur() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void determinerSemestreEtStatut(ETUDIANT* e) {
    // Détermination du semestre (ici, tous en S1 tant qu’il n’y a qu’un semestre)
    e->semestre = 1;

    // Calcul du statut
    int nb_notes = 0;
    float somme = 0;

    for (int i = 0; i < NB_UE; i++) {
        if (e->notes[i] >= 0) { // note enregistrée
            somme += e->notes[i];
            nb_notes++;
        }
    }

    if (nb_notes < NB_UE) {
        strcpy(e->statut, "en cours");
    }
    else {
        float moyenne = somme / NB_UE;
        if (moyenne >= 10)
            strcpy(e->statut, "ADM");
        else
            strcpy(e->statut, "AJ");
    }
}

// fonction INSCRIPTION permet d'enregistrer les étudiants
void inscription(ETUDIANT tab_etu[], int* nb_etu) {
    char prenom[MAX_CHAR_ETU], nom[MAX_CHAR_ETU];
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
    if (*nb_etu < MAX_NB_ETU) {
        tab_etu[*nb_etu].id = *nb_etu + 1; // commence à 1
        strcpy(tab_etu[*nb_etu].prenom, prenom);
        strcpy(tab_etu[*nb_etu].nom, nom);
        for (int i = 0; i < NB_UE; i++)
            tab_etu[*nb_etu].notes[i] = -1;
        printf("Inscription enregistree (%d)\n", tab_etu[*nb_etu].id);
        (*nb_etu)++;
    }
    else {
        printf("Nombre maximum d'etudiants atteint\n");
    }
}
// fonction NOTE permet d'assigner les notes aux etudiants 
void note(ETUDIANT tab_etu[], int nb_etu) {
    int id, ue;
    float valeur;
    scanf("%d %d %f", &id, &ue, &valeur);

    // Vérifications des entrées
    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }
    else if (ue < 1 || ue > NB_UE) {
        printf("UE incorrect\n");
        return;
    }
    else if (valeur < 0 || valeur > 20) {
        printf("Note incorrect\n");
        return;
    }

    // Vérifier que l'étudiant est encore en formation
    ETUDIANT* e = &tab_etu[id - 1];
    determinerSemestreEtStatut(e); // on s'assure que le statut est à jour

    if (strcmp(e->statut, "en cours") != 0) {
        printf("Etudiant hors formation\n");
        return;
    }

    // Enregistrement de la note
    tab_etu[id - 1].notes[ue - 1] = valeur;
    determinerSemestreEtStatut(&tab_etu[id - 1]);
    printf("Note enregistree\n");
}

// DEMISSION : change le statut d'un étudiant "en cours" en "DEM"
void demission(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];
    determinerSemestreEtStatut(e); // statut actuel à jour

    if (strcmp(e->statut, "en cours") == 0) {
        strcpy(e->statut, "demission");
    }
    else {
        printf("Demission impossible\n");
    }
}

// DEFAILLANCE : change le statut d'un étudiant "en cours" en "DEF"
void defaillance(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];
    determinerSemestreEtStatut(e); // statut à jour

    if (strcmp(e->statut, "en cours") == 0) {
        strcpy(e->statut, "defaillance");
    }
    else {
        printf("Defaillance impossible\n");
    }
}

//fonction ETUDIANTS permet d'afficher la liste d'etudiants
void etudiants(ETUDIANT tab_etu[], int nb_etu) {
    for (int i = 0; i < nb_etu; ++i) {
        determinerSemestreEtStatut(&tab_etu[i]);
        printf("%d - %s %s - S%d - %s\n",
            tab_etu[i].id,
            tab_etu[i].prenom,
            tab_etu[i].nom,
            tab_etu[i].semestre,
            tab_etu[i].statut);
    }
}

// fonction CURSUS permet d'afficher la situation de l'étudiant (semestre et note)
void cursus(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];
    determinerSemestreEtStatut(e);

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

        if (strcmp(saisie, "EXIT") == 0) { // EXIT
            break;
        }
        else if (strcmp(saisie, "INSCRIRE") == 0) { // INSCRIRE
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
        else {
            printf("Commande inconnue\n");
        }
        GererErreur();
    }
    return 0;
}
