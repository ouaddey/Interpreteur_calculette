#ifndef ANALYSE_SYNTAXIQUE_H
#define ANALYSE_SYNTAXIQUE_H

#include "type_ast.h"

void analyser(char *fichier, Ast *arbre);
/*
-- e.i : indifferent
-- e.f : une Expression Arithmetique Generale a ete lue dans fichier
-- si elle est correcte, *arbre contient l'arbre abstrait correspondant
-- sinon le pgm termine sur un message d'erreur
*/

void analyser_programme(char *fichier, Ast *arbre);
/*
-- e.i : indifferent
-- e.f : un Programme (sequence d'instructions) a ete lu dans fichier
-- si il est correct, *arbre contient l'arbre abstrait correspondant
-- sinon le pgm termine sur un message d'erreur
-- grammaire :
--   pgm          -> seq_inst
--   seq_inst     -> inst suite_seq_inst | epsilon
--   suite_seq_inst -> SEPINST seq_inst
--   inst         -> IDF AFF eag
--   inst         -> LIRE  PARO IDF PARF
--   inst         -> ECRIRE PARO eag PARF
*/

#endif
