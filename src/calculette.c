#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse_syntaxique.h"
#include "ast_parcours.h"

static void detruire_ast(Ast ast) {
    if (ast == NULL) {
        return;
    }
    if (ast->nature == OPERATION) {
        detruire_ast(ast->gauche);
        detruire_ast(ast->droite);
    }
    free(ast);
}

int main (int argc, char *argv[]) {
    char *source = "";
    int verbose = 0;

    for (int i = 1; i < argc; ++i) {
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
    analyser(source, &arbre);
    if (arbre == NULL) {
        fprintf(stderr, "Aucune expression lue.\n");
        return 1;
    }

    double resultat = evaluation(arbre);
    if (verbose) {
        printf("Expression syntactiquement correcte.\n");
        printf("Arbre abstrait de l'expression\n");
        afficher(arbre);
        printf("\n");
    }
    printf("Resultat = %.10g\n", resultat);
    detruire_ast(arbre);
    return 0 ;
}
