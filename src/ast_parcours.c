#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parcours.h"
#include "table_symbole.h"

/* ===================================================================== */
/* Table des symboles (variables du programme)                           */
/* ===================================================================== */

static void set_variable(const char *nom, double val) {
    table_symbole_set(nom, val);
}

static double get_variable(const char *nom) {
    return table_symbole_get(nom);
}

void reinitialiser_table_symboles(void) {
    table_symbole_reinitialiser();
}

void afficher_table_symboles(FILE *flux) {
    table_symbole_afficher(flux);
}

/* ===================================================================== */
/* Affichage de l'arbre abstrait                                         */
/* ===================================================================== */

static void aff_operateur(TypeOperateur op) {
    switch (op) {
        case N_PLUS:  printf("+"); break;
        case N_MOINS: printf("-"); break;
        case N_MUL:   printf("*"); break;
        case N_DIV:   printf("/"); break;
    }
}

static const char *nature_comp_vers_chaine(TypeAst nat) {
    switch (nat) {
        case N_INF:    return "<";
        case N_SUP:    return ">";
        case N_INF_EG: return "<=";
        case N_SUP_EG: return ">=";
        case N_EGAL:   return "==";
        case N_DIFF:   return "!=";
        default:       return "?";
    }
}

static int est_operateur_comp(TypeAst nat) {
    return nat == N_INF || nat == N_SUP || nat == N_INF_EG
        || nat == N_SUP_EG || nat == N_EGAL || nat == N_DIFF;
}

void afficher_ast(Ast expr) {
    if (expr == NULL) {
        printf("(vide)");
        return;
    }
    switch (expr->nature) {

        case OPERATION:
            printf("(");
            afficher_ast(expr->gauche);
            aff_operateur(expr->operateur);
            afficher_ast(expr->droite);
            printf(")");
            break;

        case VALEUR:
            printf("%g", expr->valeur);
            break;

        case N_IDF:
            printf("%s", expr->nom);
            break;

        case N_AFF:
            printf("AFF(%s, ", expr->nom);
            afficher_ast(expr->gauche);
            printf(")");
            break;

        case N_LIRE:
            printf("LIRE(%s)", expr->nom);
            break;

        case N_ECRIRE:
            printf("ECRIRE(");
            afficher_ast(expr->gauche);
            printf(")");
            break;

        case N_SEPINST:
            /* affiche chaque instruction sur une ligne */
            afficher_ast(expr->gauche);
            printf("\n");
            if (expr->droite != NULL) {
                afficher_ast(expr->droite);
            }
            break;

        case N_INF:
        case N_SUP:
        case N_INF_EG:
        case N_SUP_EG:
        case N_EGAL:
        case N_DIFF:
            afficher_ast(expr->gauche);
            printf(" %s ", nature_comp_vers_chaine(expr->nature));
            afficher_ast(expr->droite);
            break;

        case N_IF:
            printf("SI(");
            afficher_ast(expr->gauche);  /* condition */
            printf(", ALORS(");
            afficher_ast(expr->droite->gauche);  /* branche alors */
            printf("), SINON(");
            afficher_ast(expr->droite->droite);  /* branche sinon */
            printf("))");
            break;

        case N_WHILE:
            printf("TANQUE(");
            afficher_ast(expr->gauche);  /* condition */
            printf(", FAIRE(");
            afficher_ast(expr->droite);  /* corps     */
            printf("))");
            break;

        default:
            fprintf(stderr, "ERREUR afficher_ast : nature inconnue\n");
            exit(EXIT_FAILURE);
    }
}

/* ===================================================================== */
/* Evaluation d'une expression arithmetique pure (sans variables)        */
/* ===================================================================== */

double evaluation(Ast expr) {
    if (expr == NULL) {
        fprintf(stderr, "AST invalide\n");
        exit(EXIT_FAILURE);
    }
    switch (expr->nature) {
        case VALEUR:
            return expr->valeur;

        case OPERATION: {
            double gauche = evaluation(expr->gauche);
            double droite = evaluation(expr->droite);
            switch (expr->operateur) {
                case N_PLUS:  return gauche + droite;
                case N_MOINS: return gauche - droite;
                case N_MUL:   return gauche * droite;
                case N_DIV:   return gauche / droite;
                default:
                    fprintf(stderr, "Operateur inconnu\n");
                    exit(EXIT_FAILURE);
            }
        }

        default:
            fprintf(stderr, "Nature de noeud inconnue dans evaluation\n");
            exit(EXIT_FAILURE);
    }
}

/* ===================================================================== */
/* Interpretation du programme                                           */
/* ===================================================================== */

/* declaration anticipee (evaluer_condition utilise evaluer_expr)        */
static double evaluer_expr(Ast expr);

/* evaluer_condition : evalue une condition booleenne (retourne 1 ou 0)  */
static int evaluer_condition(Ast cond) {
    if (cond == NULL || !est_operateur_comp(cond->nature)) {
        fprintf(stderr, "ERREUR : noeud de comparaison attendu\n");
        exit(EXIT_FAILURE);
    }
    double gauche = evaluer_expr(cond->gauche);
    double droite = evaluer_expr(cond->droite);
    switch (cond->nature) {
        case N_INF:    return gauche <  droite;
        case N_SUP:    return gauche >  droite;
        case N_INF_EG: return gauche <= droite;
        case N_SUP_EG: return gauche >= droite;
        case N_EGAL:   return gauche == droite;
        case N_DIFF:   return gauche != droite;
        default:
            fprintf(stderr, "Operateur de comparaison inconnu\n");
            exit(EXIT_FAILURE);
    }
}

/* evaluer_expr : evalue une expression (peut contenir des variables)    */
static double evaluer_expr(Ast expr) {
    if (expr == NULL) {
        fprintf(stderr, "Expression NULL\n");
        exit(EXIT_FAILURE);
    }
    switch (expr->nature) {

        case VALEUR:
            return expr->valeur;

        case N_IDF:
            return get_variable(expr->nom);

        case OPERATION: {
            double g = evaluer_expr(expr->gauche);
            double d = evaluer_expr(expr->droite);
            switch (expr->operateur) {
                case N_PLUS:  return g + d;
                case N_MOINS: return g - d;
                case N_MUL:   return g * d;
                case N_DIV:   return g / d;
                default:
                    fprintf(stderr, "Operateur inconnu\n");
                    exit(EXIT_FAILURE);
            }
        }

        default:
            fprintf(stderr, "Noeud non-expression dans evaluer_expr\n");
            exit(EXIT_FAILURE);
    }
}

/* interpreter_inst : execute une instruction                            */
static void interpreter_inst(Ast inst) {
    if (inst == NULL) return;
    switch (inst->nature) {

        case N_AFF: {
            double val = evaluer_expr(inst->gauche);
            set_variable(inst->nom, val);
            break;
        }

        case N_LIRE: {
            double val;
            printf("? ");
            fflush(stdout);
            if (scanf("%lf", &val) != 1) {
                fprintf(stderr, "ERREUR : lecture impossible pour '%s'\n", inst->nom);
                exit(EXIT_FAILURE);
            }
            set_variable(inst->nom, val);
            break;
        }

        case N_ECRIRE: {
            double val = evaluer_expr(inst->gauche);
            printf("%g\n", val);
            break;
        }

        case N_IF: {
            /* inst->gauche = condition, inst->droite = N_SEPINST(alors, sinon) */
            int vrai = evaluer_condition(inst->gauche);
            if (vrai) {
                interpreter(inst->droite->gauche);  /* branche alors */
            } else {
                interpreter(inst->droite->droite);  /* branche sinon (peut etre NULL) */
            }
            break;
        }

        case N_WHILE: {
            /* inst->gauche = condition, inst->droite = corps (seq_inst) */
            while (evaluer_condition(inst->gauche)) {
                interpreter(inst->droite);  /* executer le corps */
            }
            break;
        }

        default:
            fprintf(stderr, "ERREUR interpreter_inst : noeud inattendu\n");
            exit(EXIT_FAILURE);
    }
}

/* interpreter : interprete une sequence d'instructions                  */
void interpreter(Ast pgm) {
    if (pgm == NULL) return;   /* sequence vide */

    if (pgm->nature == N_SEPINST) {
        interpreter_inst(pgm->gauche);  /* execute l'instruction courante */
        interpreter(pgm->droite);       /* puis la suite                  */
    } else {
        /* noeud instruction seul */
        interpreter_inst(pgm);
    }
}
