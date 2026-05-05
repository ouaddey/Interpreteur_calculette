#ifndef TABLE_SYMBOLE_H
#define TABLE_SYMBOLE_H

#include <stdio.h>

void table_symbole_reinitialiser(void);
void table_symbole_set(const char *nom, double valeur);
double table_symbole_get(const char *nom);
void table_symbole_afficher(FILE *flux);

#endif
