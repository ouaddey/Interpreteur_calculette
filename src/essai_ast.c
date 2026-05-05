#include <stdio.h> 
#include <stdlib.h>

#include "ast_construction.h"
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

static Ast construire_expression1(void) {
      Ast v2a = creer_valeur(2);
      Ast v3 = creer_valeur(3);
      Ast v5 = creer_valeur(5);
      Ast v2b = creer_valeur(2);
      Ast mult = creer_operation(N_MUL, v3, v5);
      Ast somme = creer_operation(N_PLUS, v2a, mult);
      return creer_operation(N_MOINS, somme, v2b);
}

static Ast construire_expression2(void) {
      Ast gauche = creer_operation(N_PLUS, creer_valeur(2), creer_valeur(3));
      Ast droite = creer_operation(N_MOINS, creer_valeur(5), creer_valeur(2));
      return creer_operation(N_MUL, gauche, droite);
}

static Ast construire_expression3(void) {
      Ast sous = creer_operation(N_MOINS, creer_valeur(5), creer_valeur(2));
      Ast division = creer_operation(N_DIV, creer_valeur(3), sous);
      return creer_operation(N_PLUS, creer_valeur(2), division);
}

static Ast construire_expression4(void) {
      Ast premiere_diff = creer_operation(N_MOINS, creer_valeur(5), creer_valeur(2));
      Ast denominateur = creer_operation(N_MOINS, premiere_diff, creer_valeur(3));
      return creer_operation(N_DIV, creer_valeur(3), denominateur);
}

static void tester_expression(const char *description, Ast (*construction)(void)) {
      printf("Expression testee : %s\n", description);
      Ast ast = construction();
      afficher(ast);
      printf("\n");
      double valeur = evaluation(ast);
      printf("\nValeur de l'expression : %.10g\n\n", valeur);
      detruire_ast(ast);
}

int main(void) {
      tester_expression("2 + 3 * 5 - 2", construire_expression1);
      tester_expression("(2 + 3) * (5 - 2)", construire_expression2);
      tester_expression("2 + 3 / (5 - 2)", construire_expression3);
      tester_expression("3 / (5 - 2 - 3)", construire_expression4);
      return 0;
}
