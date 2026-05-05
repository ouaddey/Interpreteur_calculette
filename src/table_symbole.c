#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table_symbole.h"

#define MAX_VARS 256

typedef struct {
    char nom[256];
    double valeur;
} EntreeSymbole;

static EntreeSymbole table_symboles[MAX_VARS];
static int nb_variables = 0;

void table_symbole_reinitialiser(void) {
    nb_variables = 0;
}

static int trouver_variable(const char *nom) {
    for (int i = 0; i < nb_variables; i++) {
        if (strcmp(table_symboles[i].nom, nom) == 0) {
            return i;
        }
    }
    return -1;
}

void table_symbole_set(const char *nom, double valeur) {
    int index = trouver_variable(nom);
    if (index >= 0) {
        table_symboles[index].valeur = valeur;
        return;
    }
    if (nb_variables >= MAX_VARS) {
        fprintf(stderr, "ERREUR : trop de variables (max %d)\n", MAX_VARS);
        exit(EXIT_FAILURE);
    }
    strncpy(table_symboles[nb_variables].nom, nom, 255);
    table_symboles[nb_variables].nom[255] = '\0';
    table_symboles[nb_variables].valeur = valeur;
    nb_variables++;
}

double table_symbole_get(const char *nom) {
    int index = trouver_variable(nom);
    if (index >= 0) {
        return table_symboles[index].valeur;
    }
    fprintf(stderr, "ERREUR : variable '%s' non definie\n", nom);
    exit(EXIT_FAILURE);
}

void table_symbole_afficher(FILE *flux) {
    if (flux == NULL) {
        flux = stdout;
    }
    if (nb_variables == 0) {
        fprintf(flux, "(table vide)\n");
        return;
    }
    for (int i = 0; i < nb_variables; i++) {
        fprintf(flux, "%s = %g\n", table_symboles[i].nom, table_symboles[i].valeur);
    }
}
