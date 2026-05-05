#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "type_ast.h"
#include "ast_construction.h"

/* --------------------------------------------------------------------- */

static Ast allouer_noeud(void) {
    Ast n = (Ast)malloc(sizeof(NoeudAst));
    if (n == NULL) {
        fprintf(stderr, "ERREUR : allocation memoire echouee\n");
        exit(EXIT_FAILURE);
    }
    n->gauche = NULL;
    n->droite = NULL;
    n->nom[0] = '\0';
    n->valeur = 0.0;
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit) {
    if (opde_gauche == NULL || opde_droit == NULL) {
        fprintf(stderr, "ERREUR : operandes NULL dans creer_operation\n");
        exit(EXIT_FAILURE);
    }
    Ast n = allouer_noeud();
    n->nature    = OPERATION;
    n->operateur = opr;
    n->gauche    = opde_gauche;
    n->droite    = opde_droit;
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_valeur(double val) {
    Ast n = allouer_noeud();
    n->nature = VALEUR;
    n->valeur = val;
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_idf(const char *nom) {
    Ast n = allouer_noeud();
    n->nature = N_IDF;
    strncpy(n->nom, nom, 255);
    n->nom[255] = '\0';
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_aff(const char *nom, Ast expr) {
    Ast n = allouer_noeud();
    n->nature = N_AFF;
    strncpy(n->nom, nom, 255);
    n->nom[255] = '\0';
    n->gauche = expr;   /* l'expression affectee */
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_lire(const char *nom) {
    Ast n = allouer_noeud();
    n->nature = N_LIRE;
    strncpy(n->nom, nom, 255);
    n->nom[255] = '\0';
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_ecrire(Ast expr) {
    Ast n = allouer_noeud();
    n->nature = N_ECRIRE;
    n->gauche = expr;   /* l'expression a afficher */
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_seqinst(Ast inst, Ast suite) {
    Ast n = allouer_noeud();
    n->nature = N_SEPINST;
    n->gauche = inst;   /* instruction courante       */
    n->droite = suite;  /* suite (peut etre NULL)     */
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_condition(TypeAst op, Ast gauche, Ast droite) {
    Ast n = allouer_noeud();
    n->nature = op;       /* N_INF, N_SUP, N_INF_EG, N_SUP_EG, N_EGAL ou N_DIFF */
    n->gauche = gauche;
    n->droite = droite;
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_if(Ast cond, Ast alors_branch, Ast sinon_branch) {
    Ast n = allouer_noeud();
    n->nature = N_IF;
    n->gauche = cond;
    /* on reutilise N_SEPINST pour stocker les deux branches */
    n->droite = creer_seqinst(alors_branch, sinon_branch);
    return n;
}

/* --------------------------------------------------------------------- */

Ast creer_while(Ast cond, Ast corps) {
    Ast n = allouer_noeud();
    n->nature = N_WHILE;
    n->gauche = cond;   /* condition de la boucle   */
    n->droite = corps;  /* corps (seq_inst)          */
    return n;
}
