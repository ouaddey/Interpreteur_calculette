
#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

/* --- expressions --- */

Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit);
/* renvoie un noeud OPERATION d'operateur opr, de fils gauche et droit */

Ast creer_valeur(double val);
/* renvoie une feuille VALEUR de valeur val */

Ast creer_idf(const char *nom);
/* renvoie une feuille N_IDF de nom nom */

/* --- instructions --- */

Ast creer_aff(const char *nom, Ast expr);
/* renvoie un noeud N_AFF : affectation  nom = expr */

Ast creer_lire(const char *nom);
/* renvoie un noeud N_LIRE : lecture dans la variable nom */

Ast creer_ecrire(Ast expr);
/* renvoie un noeud N_ECRIRE : ecriture de la valeur de expr */

Ast creer_seqinst(Ast inst, Ast suite);
/* renvoie un noeud N_SEPINST : inst est l'instruction courante,
   suite est la suite de la sequence (peut etre NULL)            */

/* --- instruction conditionnelle --- */

Ast creer_condition(TypeAst op, Ast gauche, Ast droite);
/* renvoie un noeud de comparaison (N_INF, N_SUP, ...) : gauche OP droite */

Ast creer_if(Ast cond, Ast alors_branch, Ast sinon_branch);
/* renvoie un noeud N_IF : cond est la condition,
   alors_branch et sinon_branch sont les sequences d'instructions */

/* --- instruction iterative --- */

Ast creer_while(Ast cond, Ast corps);
/* renvoie un noeud N_WHILE : cond est la condition de boucle,
   corps est la sequence d'instructions a repeter             */

#endif
