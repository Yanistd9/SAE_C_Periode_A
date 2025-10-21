#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable : 4996)

enum {
    MAX_CHAR_ETU = 30,
    MAX_NB_ETU = 180,
    NB_UE = 6,
    NB_SEM = 6,
    NB_BILAN = 3
};

typedef struct {
    int id;
    char prenom[MAX_CHAR_ETU];
    char nom[MAX_CHAR_ETU];
    float notes[NB_SEM + 1][NB_UE];    
    char codes[NB_SEM + 1][NB_UE][4];  
    float rcue[NB_BILAN + 1][NB_UE];   
    char codes_rcue[NB_BILAN + 1][NB_UE][4];
    int semestre;
    char statut[15];
    char statut_sem[NB_SEM + 1][15];
} ETUDIANT;

void GererErreur() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inscription(ETUDIANT tab_etu[], int* nb_etu) {
    char prenom[MAX_CHAR_ETU], nom[MAX_CHAR_ETU];
    scanf("%s %s", prenom, nom);

    for (int i = 0; i < *nb_etu; i++) {
        if (strcmp(tab_etu[i].prenom, prenom) == 0 &&
            strcmp(tab_etu[i].nom, nom) == 0) {
            printf("Nom incorrect\n");
            return;
        }
    }

    if (*nb_etu < MAX_NB_ETU) {
        ETUDIANT* e = &tab_etu[*nb_etu];
        e->id = *nb_etu + 1;
        strcpy(e->prenom, prenom);
        strcpy(e->nom, nom);

        for (int s = 1; s <= NB_SEM; s++) {
            for (int j = 0; j < NB_UE; j++) {
                e->notes[s][j] = -1;
                strcpy(e->codes[s][j], "*");
            }
        }

        for (int b = 1; b <= NB_BILAN; b++) {
            for (int j = 0; j < NB_UE; j++) {
                e->rcue[b][j] = -1;
                strcpy(e->codes_rcue[b][j], "*");
            }
        }

        e->semestre = 1;
        strcpy(e->statut, "en cours");
        for (int s = 1; s <= NB_SEM; s++)
            strcpy(e->statut_sem[s], "*");
        strcpy(e->statut_sem[1], "en cours");

        printf("Inscription enregistree (%d)\n", e->id);
        (*nb_etu)++;
    }
    else {
        printf("Nombre maximum d'etudiants atteint\n");
    }
}

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

    e->notes[e->semestre][ue - 1] = valeur;
    printf("Note enregistree\n");
}

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

void jury(ETUDIANT tab_etu[], int nb_etu) {
    int semestre;
    scanf("%d", &semestre);

    if (semestre < 1 || semestre > NB_SEM) {
        printf("Semestre incorrect\n");
        return;
    }

    int notes_manquantes = 0;
    int nb_etudiants = 0;

    for (int i = 0; i < nb_etu; i++) {
        ETUDIANT* e = &tab_etu[i];
        if (e->semestre == semestre && strcmp(e->statut, "en cours") == 0) {
            for (int j = 0; j < NB_UE; j++) {
                if (e->notes[semestre][j] < 0) {
                    notes_manquantes = 1;
                    break;
                }
            }
        }
        if (notes_manquantes)
            break;
    }

    if (notes_manquantes) {
        printf("Des notes sont manquantes\n");
        return;
    }

    for (int i = 0; i < nb_etu; i++) {
        ETUDIANT* e = &tab_etu[i];

        if (e->semestre == semestre && strcmp(e->statut, "en cours") == 0) {
            nb_etudiants++;

            for (int j = 0; j < NB_UE; j++) {
                float note = e->notes[semestre][j];
                if (note >= 10.0) strcpy(e->codes[semestre][j], "ADM");
                else strcpy(e->codes[semestre][j], "AJ");
            }

            if (semestre % 2 == 1) {
                float somme = 0;
                for (int j = 0; j < NB_UE; j++) somme += e->notes[semestre][j];
                float moyenne = somme / NB_UE;

                if (moyenne >= 10.0) {
                    strcpy(e->statut_sem[semestre], "ADM");

                    e->semestre = semestre + 1;
                    strcpy(e->statut, "en cours");

                    strcpy(e->statut_sem[e->semestre], "en cours");
                    for (int j = 0; j < NB_UE; j++) {
                        e->notes[e->semestre][j] = -1;
                        strcpy(e->codes[e->semestre][j], "*");
                    }
                }
                else {
                    strcpy(e->statut_sem[semestre], "AJ");
                    strcpy(e->statut, "ajourne");
                }
            }

            else if (semestre % 2 == 0) {
                int bilan = semestre / 2;
                int nb_adm = 0, nb_ajb = 0;

                for (int j = 0; j < NB_UE; j++) {
                    float s1 = e->notes[semestre - 1][j];
                    float s2 = e->notes[semestre][j];
                    float moy = (s1 + s2) / 2.0;
                    e->rcue[bilan][j] = moy;

                    if (moy >= 10.0) {
                        strcpy(e->codes_rcue[bilan][j], "ADM");
                        nb_adm++;
                        if (!strcmp(e->codes[semestre - 1][j], "AJ"))
                            strcpy(e->codes[semestre - 1][j], "ADC");
                        if (!strcmp(e->codes[semestre][j], "AJ"))
                            strcpy(e->codes[semestre][j], "ADC");
                    }
                    else if (moy >= 8.0) strcpy(e->codes_rcue[bilan][j], "AJ");
                    else {
                        strcpy(e->codes_rcue[bilan][j], "AJB");
                        nb_ajb++;
                    }
                }

                if (bilan < 3) {
                    if (nb_adm >= 4 && nb_ajb == 0) {
                        strcpy(e->statut_sem[semestre], "ADM");  
                        e->semestre = semestre + 1;
                        strcpy(e->statut, "en cours");
                        strcpy(e->statut_sem[e->semestre], "en cours");
                        for (int j = 0; j < NB_UE; j++) {
                            e->notes[e->semestre][j] = -1;
                            strcpy(e->codes[e->semestre][j], "*");
                        }
                    }
                    else {
                        strcpy(e->statut_sem[semestre], "AJ");
                        strcpy(e->statut, "ajourne");
                    }
                }

                if (semestre == 6) {
                    int toutes_valides = 1;
                    for (int b = 1; b <= 3; b++) {
                        for (int j = 0; j < NB_UE; j++) {
                            if (strcmp(e->codes_rcue[b][j], "ADM") != 0) {
                                toutes_valides = 0;
                                break;
                            }
                        }
                        if (!toutes_valides) break;
                    }
                    if (toutes_valides) {
                        strcpy(e->statut_sem[semestre], "ADM");
                        strcpy(e->statut, "diplome");
                    }
                    else {
                        strcpy(e->statut_sem[semestre], "AJ");
                        strcpy(e->statut, "ajourne");
                    }
                }
            }
        }
    }

    printf("Semestre termine pour %d etudiant(s)\n", nb_etudiants);
}


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

void cursus(ETUDIANT tab_etu[], int nb_etu) {
    int id;
    scanf("%d", &id);

    if (id < 1 || id > nb_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    ETUDIANT* e = &tab_etu[id - 1];

    printf("%d %s %s\n", e->id, e->prenom, e->nom);

    for (int s = 1; s <= e->semestre; s++) {
        printf("S%d - ", s);

        for (int j = 0; j < NB_UE; j++) {
            if (e->notes[s][j] >= 0)
                printf("%.1f (%s)", e->notes[s][j], e->codes[s][j]);
            else
                printf("* (*)");

            if (j < NB_UE - 1) printf(" - ");
        }

        if (s == e->semestre)
            printf(" - %s\n", e->statut);
        else
            printf(" - %s\n", e->statut_sem[s]); 

        if (s % 2 == 0 && e->semestre > s) {
            int b = s / 2;
            printf("B%d - ", b);
            for (int j = 0; j < NB_UE; j++) {
                if (e->rcue[b][j] >= 0)
                    printf("%.1f (%s)", e->rcue[b][j], e->codes_rcue[b][j]);
                else
                    printf("* (*)");
                if (j < NB_UE - 1) printf(" - ");
            }
            printf(" -\n");
        }
    }
}

void bilan(ETUDIANT tab_etu[], int nb_etu) {
    int annee;
    scanf("%d", &annee);

    if (annee < 1 || annee > 3) {
        printf("Annee incorrecte\n");
        return;
    }

    int s_first = 2 * annee - 1;
    int s_last = 2 * annee;

    int c_dem = 0, c_def = 0, c_encours = 0, c_aj = 0, c_passe = 0;

    for (int i = 0; i < nb_etu; i++) {
        ETUDIANT* e = &tab_etu[i];

        if (e->semestre < s_first && strcmp(e->statut, "DEM") != 0 && strcmp(e->statut, "DEF") != 0)
            continue;

        int in_year_now = (e->semestre == s_first || e->semestre == s_last);

        if (annee < 3) {
            if (e->semestre >= s_last + 1) {
                c_passe++;
                continue;
            }
        }
        else {
            if (strcmp(e->statut, "diplome") == 0) {
                c_passe++;
                continue;
            }
        }

        if (in_year_now) {
            if (strcmp(e->statut, "DEM") == 0) {
                c_dem++;
            }
            else if (strcmp(e->statut, "DEF") == 0) {
                c_def++;
            }
            else if (strcmp(e->statut, "en cours") == 0) {
                c_encours++;
            }
            else if (strcmp(e->statut, "ajourne") == 0) {
                c_aj++;
            }
        }
    }

    printf("BILAN %d\n", annee);
    printf("%d demission(s)\n", c_dem);
    printf("%d defaillance(s)\n", c_def);
    printf("%d en cours\n", c_encours);
    printf("%d ajourne(s)\n", c_aj);
    printf("%d passe(s)\n", c_passe);
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
        else if (strcmp(saisie, "BILAN") == 0) {
            bilan(tab_etu, nb_etu);
        }
        else {
            printf("Commande inconnue\n");
        }
        GererErreur();
    }
    return 0;
}
