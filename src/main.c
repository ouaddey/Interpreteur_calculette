#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse_syntaxique.h"
#include "ast_parcours.h"

static void detruire_ast(Ast ast) {
    if (ast == NULL) return;
    switch (ast->nature) {
        case OPERATION:
        case N_AFF:
        case N_ECRIRE:
        case N_SEPINST:
        case N_INF:
        case N_SUP:
        case N_INF_EG:
        case N_SUP_EG:
        case N_EGAL:
        case N_DIFF:
        case N_IF:
        case N_WHILE:
            detruire_ast(ast->gauche);
            detruire_ast(ast->droite);
            break;
        default:
            break;
    }
    free(ast);
}

int main(int argc, char *argv[]) {
    char *source  = "";
    int   verbose = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (source[0] == '\0') {
            source = argv[i];
        } else {
            fprintf(stderr, "usage: %s [--verbose] [fichier]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    Ast arbre = NULL;
    analyser_programme(source, &arbre);

    if (verbose) {
        printf("=== Arbre abstrait du programme ===\n");
        if (arbre == NULL) {
            printf("(programme vide)\n");
        } else {
            afficher_ast(arbre);
        }
        printf("===================================\n\n");
        printf("=== Execution ===\n");
    }
    reinitialiser_table_symboles();
    interpreter(arbre);

    printf("\n=== Table des symboles ===\n");
    afficher_table_symboles(stdout);

    detruire_ast(arbre);
    return 0;
}
