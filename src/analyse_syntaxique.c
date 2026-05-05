#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "ast_construction.h"
#include "analyse_syntaxique.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */

static void erreur_syntaxe(const char *message_erreur, int code) {
    Lexeme courant = lexeme_courant();
    const char *texte_lexeme = (courant.chaine[0] != '\0')
                                   ? courant.chaine
                                   : Nature_vers_Chaine(courant.nature);
    fprintf(stderr,
            "ERREUR SYNTAXIQUE (ligne %d, colonne %d): %s. Lexeme courant: %s\n",
            courant.ligne, courant.colonne, message_erreur, texte_lexeme);
    arreter();
    exit(code);
}

/* --------------------------------------------------------------------- */
/* Declarations des fonctions de reconnaissance                           */

static Ast rec_exp(void);
static Ast rec_eag(void);
static Ast rec_seq_terme(void);
static Ast rec_suite_seq_terme(Ast gauche);
static Ast rec_terme(void);
static Ast rec_seq_facteur(void);
static Ast rec_suite_seq_facteur(Ast gauche);
static Ast rec_facteur(void);

/* programme et instructions */
static Ast rec_pgm(void);
static Ast rec_seq_inst(void);
static Ast rec_suite_seq_inst(void);
static Ast rec_inst(void);

/* instruction conditionnelle */
static Ast rec_condition(void);
static Ast rec_inst_si(void);

/* instruction iterative */
static Ast rec_inst_tanque(void);

/* --------------------------------------------------------------------- */

static int est_op1(Nature_Lexeme nature) {
    return nature == PLUS || nature == MOINS;
}

static int est_op2(Nature_Lexeme nature) {
    return nature == MUL || nature == DIVISION;
}

static TypeOperateur operateur_depuis_op1(Nature_Lexeme nature) {
    switch (nature) {
        case PLUS:  return N_PLUS;
        case MOINS: return N_MOINS;
        default:
            erreur_syntaxe("operateur + ou - attendu", 4);
            return N_PLUS;
    }
}

static TypeOperateur operateur_depuis_op2(Nature_Lexeme nature) {
    switch (nature) {
        case MUL:      return N_MUL;
        case DIVISION: return N_DIV;
        default:
            erreur_syntaxe("operateur * ou / attendu", 5);
            return N_MUL;
    }
}

/* --------------------------------------------------------------------- */
/* analyser : analyse une expression arithmetique (TP5)                   */

void analyser(char *fichier, Ast *arbre) {
    demarrer(fichier);
    Ast resultat = rec_exp();
    arreter();
    if (arbre != NULL) {
        *arbre = resultat;
    }
}

/* --------------------------------------------------------------------- */
/* analyser_programme : analyse un programme complet (TP6)                */

void analyser_programme(char *fichier, Ast *arbre) {
    demarrer(fichier);
    Ast resultat = rec_pgm();
    arreter();
    if (arbre != NULL) {
        *arbre = resultat;
    }
}

/* --------------------------------------------------------------------- */
/* Regles de la grammaire des expressions                                 */

static Ast rec_exp(void) {
    Ast expr = rec_eag();
    if (lexeme_courant().nature != FIN_SEQUENCE) {
        erreur_syntaxe("fin de sequence attendue", 1);
    }
    return expr;
}

static Ast rec_eag(void) {
    return rec_seq_terme();
}

static Ast rec_seq_terme(void) {
    Ast gauche = rec_terme();
    return rec_suite_seq_terme(gauche);
}

static Ast rec_suite_seq_terme(Ast gauche) {
    Lexeme courant = lexeme_courant();
    if (est_op1(courant.nature)) {
        TypeOperateur op = operateur_depuis_op1(courant.nature);
        avancer();
        Ast droite = rec_terme();
        Ast combinaison = creer_operation(op, gauche, droite);
        return rec_suite_seq_terme(combinaison);
    }
    return gauche;
}

static Ast rec_terme(void) {
    return rec_seq_facteur();
}

static Ast rec_seq_facteur(void) {
    Ast gauche = rec_facteur();
    return rec_suite_seq_facteur(gauche);
}

static Ast rec_suite_seq_facteur(Ast gauche) {
    Lexeme courant = lexeme_courant();
    if (est_op2(courant.nature)) {
        TypeOperateur op = operateur_depuis_op2(courant.nature);
        avancer();
        Ast droite = rec_facteur();
        Ast combinaison = creer_operation(op, gauche, droite);
        return rec_suite_seq_facteur(combinaison);
    }
    return gauche;
}

/* rec_facteur : reconnait un facteur (nombre, variable, ou expression
   entre parentheses)                                                     */
static Ast rec_facteur(void) {
    Lexeme courant = lexeme_courant();

    /* nombre entier ou flottant */
    if (courant.nature == ENTIER || courant.nature == FLOTTANT) {
        double val = (courant.nature == ENTIER)
                     ? (double)courant.valeur
                     : courant.valeur_flottante;
        Ast noeud = creer_valeur(val);
        avancer();
        return noeud;
    }

    /* identificateur : cree un noeud N_IDF */
    if (courant.nature == IDF) {
        Ast noeud = creer_idf(courant.chaine);
        avancer();
        return noeud;
    }

    /* expression parenthesee */
    if (courant.nature == PARO) {
        avancer();
        Ast sous_expr = rec_eag();
        if (lexeme_courant().nature != PARF) {
            erreur_syntaxe("parenthese fermante ')' attendue", 2);
        }
        avancer();
        return sous_expr;
    }

    erreur_syntaxe("facteur attendu (nombre, variable ou '(')", 3);
    return NULL;
}

/* --------------------------------------------------------------------- */
/* Regles de la grammaire du programme                                    */

/* rec_pgm : pgm -> seq_inst  */
static Ast rec_pgm(void) {
    Ast pgm = rec_seq_inst();
    if (lexeme_courant().nature != FIN_SEQUENCE) {
        erreur_syntaxe("fin de sequence attendue apres le programme", 1);
    }
    return pgm;
}

/* rec_seq_inst :
     seq_inst -> inst suite_seq_inst
     seq_inst -> epsilon                                                   */
static Ast rec_seq_inst(void) {
    Nature_Lexeme nat = lexeme_courant().nature;
    if (nat == IDF || nat == LIRE || nat == ECRIRE || nat == SI || nat == TANQUE) {
        Ast inst  = rec_inst();
        Ast suite = rec_suite_seq_inst();
        return creer_seqinst(inst, suite);
    }
    /* epsilon : sequence vide */
    return NULL;
}

/* rec_suite_seq_inst :
     suite_seq_inst -> SEPINST seq_inst
     suite_seq_inst -> epsilon                                             */
static Ast rec_suite_seq_inst(void) {
    if (lexeme_courant().nature == SEPINST) {
        avancer();  /* consommer le ';' */
        return rec_seq_inst();
    }
    /* epsilon */
    return NULL;
}

/* rec_inst :
     inst -> IDF AFF eag
     inst -> LIRE  PARO IDF  PARF
     inst -> ECRIRE PARO eag PARF
     inst -> SI condition ALORS seq_inst SINON seq_inst FSI
     inst -> TANQUE condition FAIRE seq_inst FAIT                          */
static Ast rec_inst(void) {
    Lexeme courant = lexeme_courant();

    /* instruction d'affectation : x = expr */
    if (courant.nature == IDF) {
        char nom[256];
        strcpy(nom, courant.chaine);
        avancer();
        if (lexeme_courant().nature != AFF) {
            erreur_syntaxe("'=' attendu apres l'identificateur", 6);
        }
        avancer();
        Ast expr = rec_eag();
        return creer_aff(nom, expr);
    }

    /* instruction de lecture : lire (x) */
    if (courant.nature == LIRE) {
        avancer();
        if (lexeme_courant().nature != PARO) {
            erreur_syntaxe("'(' attendu apres lire", 7);
        }
        avancer();
        if (lexeme_courant().nature != IDF) {
            erreur_syntaxe("identificateur attendu dans lire(...)", 8);
        }
        char nom[256];
        strcpy(nom, lexeme_courant().chaine);
        avancer();
        if (lexeme_courant().nature != PARF) {
            erreur_syntaxe("')' attendu pour fermer lire(...)", 9);
        }
        avancer();
        return creer_lire(nom);
    }

    /* instruction d'ecriture : ecrire (expr) */
    if (courant.nature == ECRIRE) {
        avancer();
        if (lexeme_courant().nature != PARO) {
            erreur_syntaxe("'(' attendu apres ecrire", 10);
        }
        avancer();
        Ast expr = rec_eag();
        if (lexeme_courant().nature != PARF) {
            erreur_syntaxe("')' attendu pour fermer ecrire(...)", 11);
        }
        avancer();
        return creer_ecrire(expr);
    }

    /* instruction conditionnelle : si condition alors seq_inst sinon seq_inst fsi */
    if (courant.nature == SI) {
        return rec_inst_si();
    }

    /* instruction iterative : tanque condition faire seq_inst fait */
    if (courant.nature == TANQUE) {
        return rec_inst_tanque();
    }

    erreur_syntaxe("instruction attendue (identificateur, lire, ecrire, si ou tanque)", 12);
    return NULL;
}

/* rec_condition :
     condition -> eag OPCOMP eag
     OPCOMP : < > <= >= == !=                                             */
static Ast rec_condition(void) {
    Ast gauche = rec_eag();
    Lexeme courant = lexeme_courant();
    TypeAst op;
    switch (courant.nature) {
        case INF:    op = N_INF;    break;
        case SUP:    op = N_SUP;    break;
        case INF_EG: op = N_INF_EG; break;
        case SUP_EG: op = N_SUP_EG; break;
        case EGAL:   op = N_EGAL;   break;
        case DIFF:   op = N_DIFF;   break;
        default:
            erreur_syntaxe("operateur de comparaison attendu (<, >, <=, >=, ==, !=)", 13);
            return NULL;
    }
    avancer();
    Ast droite = rec_eag();
    return creer_condition(op, gauche, droite);
}

/* rec_inst_si :
     inst -> SI condition ALORS seq_inst SINON seq_inst FSI
     (la branche SINON est optionnelle)                                   */
static Ast rec_inst_si(void) {
    avancer();  /* consommer SI */
    Ast cond = rec_condition();
    if (lexeme_courant().nature != ALORS) {
        erreur_syntaxe("'alors' attendu apres la condition", 14);
    }
    avancer();  /* consommer ALORS */
    Ast alors_branch = rec_seq_inst();
    Ast sinon_branch = NULL;
    if (lexeme_courant().nature == SINON) {
        avancer();  /* consommer SINON */
        sinon_branch = rec_seq_inst();
    }
    if (lexeme_courant().nature != FSI) {
        erreur_syntaxe("'fsi' attendu pour fermer l'instruction si", 15);
    }
    avancer();  /* consommer FSI */
    return creer_if(cond, alors_branch, sinon_branch);
}

/* rec_inst_tanque :
     inst -> TANQUE condition FAIRE seq_inst FAIT                          */
static Ast rec_inst_tanque(void) {
    avancer();  /* consommer TANQUE */
    Ast cond = rec_condition();
    if (lexeme_courant().nature != FAIRE) {
        erreur_syntaxe("'faire' attendu apres la condition", 16);
    }
    avancer();  /* consommer FAIRE */
    Ast corps = rec_seq_inst();
    if (lexeme_courant().nature != FAIT) {
        erreur_syntaxe("'fait' attendu pour fermer l'instruction tanque", 17);
    }
    avancer();  /* consommer FAIT */
    return creer_while(cond, corps);
}
