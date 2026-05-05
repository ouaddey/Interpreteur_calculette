#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__

#include <stdio.h>

#include "type_ast.h"

/* --- affichage --- */

void afficher_ast(Ast expr);
/* affiche l'arbre abstrait (expression ou programme) */

#define afficher afficher_ast

/* --- evaluation d'expressions pures (sans variables) --- */

double evaluation(Ast expr);
/* calcule la valeur d'une expression arithmetique (sans variables) */

/* --- interpretation d'un programme (TP6) --- */

void interpreter(Ast pgm);
/* interprete le programme pgm (sequence d'instructions).
   Utilise une table des symboles interne.                */

void reinitialiser_table_symboles(void);
void afficher_table_symboles(FILE *flux);

#endif
