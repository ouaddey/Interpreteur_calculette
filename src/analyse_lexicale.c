#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_en_cours ;   /* le lexeme courant */

   void ajouter_caractere(char *s, char c);
   Nature_Caractere nature_caractere(char c);
   int est_separateur(char c);
   int est_chiffre(char c);
   int est_symbole(char c);
   int est_lettre(char c);
   void reconnaitre_lexeme();

   /* --------------------------------------------------------------------- */

static void erreur_lexicale(int ligne, int colonne, const char *format, ...) {
      va_list args;
      fprintf(stderr, "ERREUR LEXICALE (ligne %d, colonne %d): ", ligne, colonne);
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      fprintf(stderr, "\n");
      arreter_car();
      exit(EXIT_FAILURE);
   }

   void demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      avancer();
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   }

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() {
      return lexeme_en_cours;
   }

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car();
   }

   /* --------------------------------------------------------------------- */
   /* Reconnaissance d'un nouveau lexeme.
      Etat initial  : le caractere courant est separateur ou 1er car d'un lexeme.
      Etat final    : lexeme_en_cours contient le prochain lexeme reconnu ;
                      le caractere courant est separateur, fin de fichier,
                      ou 1er caractere du lexeme suivant.                     */

   void reconnaitre_lexeme() {
      typedef enum {E_INIT, E_NOMBRE, E_FIN} Etat_Automate;
      Etat_Automate etat = E_INIT;
      int point_present = 0;

      /* ignorer les separateurs */
      while (est_separateur(caractere_courant())) {
         avancer_car();
      }

      lexeme_en_cours.chaine[0] = '\0';

      while (etat != E_FIN) {

         switch (etat) {

            case E_INIT:

               switch (nature_caractere(caractere_courant())) {

                  case C_FIN_SEQUENCE:
                     lexeme_en_cours.nature = FIN_SEQUENCE;
                     etat = E_FIN;
                     break;

                  case CHIFFRE:
                     lexeme_en_cours.nature = ENTIER;
                     lexeme_en_cours.ligne   = numero_ligne();
                     lexeme_en_cours.colonne = numero_colonne();
                     ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                     point_present = 0;
                     lexeme_en_cours.valeur = 0;
                     lexeme_en_cours.valeur_flottante = 0.0;
                     etat = E_NOMBRE;
                     avancer_car();
                     break;

                  case MOT_CAR:
                     /* Reconnaissance d'un identificateur ou mot-cle.
                        On lit toutes les lettres consécutives, puis on
                        compare avec les mots-cles connus.               */
                     lexeme_en_cours.ligne   = numero_ligne();
                     lexeme_en_cours.colonne = numero_colonne();
                     while (est_lettre(caractere_courant())) {
                        char lower = tolower((unsigned char)caractere_courant());
                        ajouter_caractere(lexeme_en_cours.chaine, lower);
                        avancer_car();
                     }
                     /* mots-cles (conforme aux slides du cours INF404) */
                     if (strcmp(lexeme_en_cours.chaine, "lire") == 0) {
                        lexeme_en_cours.nature = LIRE;
                     } else if (strcmp(lexeme_en_cours.chaine, "ecrire") == 0) {
                        lexeme_en_cours.nature = ECRIRE;
                     } else if (strcmp(lexeme_en_cours.chaine, "si") == 0) {
                        lexeme_en_cours.nature = SI;
                     } else if (strcmp(lexeme_en_cours.chaine, "alors") == 0) {
                        lexeme_en_cours.nature = ALORS;
                     } else if (strcmp(lexeme_en_cours.chaine, "sinon") == 0) {
                        lexeme_en_cours.nature = SINON;
                     } else if (strcmp(lexeme_en_cours.chaine, "fsi") == 0) {
                        lexeme_en_cours.nature = FSI;
                     } else if (strcmp(lexeme_en_cours.chaine, "tanque") == 0) {
                        lexeme_en_cours.nature = TANQUE;
                     } else if (strcmp(lexeme_en_cours.chaine, "faire") == 0) {
                        lexeme_en_cours.nature = FAIRE;
                     } else if (strcmp(lexeme_en_cours.chaine, "fait") == 0) {
                        lexeme_en_cours.nature = FAIT;
                     } else {
                        /* ce n'est pas un mot-cle : identificateur */
                        lexeme_en_cours.nature = IDF;
                     }
                     etat = E_FIN;
                     break;

                  case SYMBOLE: {
                     char premier_car;
                     lexeme_en_cours.ligne   = numero_ligne();
                     lexeme_en_cours.colonne = numero_colonne();
                     premier_car = caractere_courant();
                     ajouter_caractere(lexeme_en_cours.chaine, premier_car);
                     avancer_car();  /* avance apres le 1er caractere */
                     switch (premier_car) {
                        case '+':
                           lexeme_en_cours.nature = PLUS;
                           break;
                        case '-':
                           lexeme_en_cours.nature = MOINS;
                           break;
                        case '*':
                           lexeme_en_cours.nature = MUL;
                           break;
                        case '/':
                           lexeme_en_cours.nature = DIVISION;
                           break;
                        case '(':
                           lexeme_en_cours.nature = PARO;
                           break;
                        case ')':
                           lexeme_en_cours.nature = PARF;
                           break;
                        case ';':
                           lexeme_en_cours.nature = SEPINST;
                           break;
                        case '=':
                           if (caractere_courant() == '=') {
                              ajouter_caractere(lexeme_en_cours.chaine, '=');
                              lexeme_en_cours.nature = EGAL;
                              avancer_car();
                           } else {
                              lexeme_en_cours.nature = AFF;
                           }
                           break;
                        case '<':
                           if (caractere_courant() == '=') {
                              ajouter_caractere(lexeme_en_cours.chaine, '=');
                              lexeme_en_cours.nature = INF_EG;
                              avancer_car();
                           } else {
                              lexeme_en_cours.nature = INF;
                           }
                           break;
                        case '>':
                           if (caractere_courant() == '=') {
                              ajouter_caractere(lexeme_en_cours.chaine, '=');
                              lexeme_en_cours.nature = SUP_EG;
                              avancer_car();
                           } else {
                              lexeme_en_cours.nature = SUP;
                           }
                           break;
                        case '!':
                           if (caractere_courant() == '=') {
                              ajouter_caractere(lexeme_en_cours.chaine, '=');
                              lexeme_en_cours.nature = DIFF;
                              avancer_car();
                           } else {
                              erreur_lexicale(lexeme_en_cours.ligne,
                                              lexeme_en_cours.colonne,
                                              "'!=' attendu apres '!'");
                           }
                           break;
                        default:
                           erreur_lexicale(lexeme_en_cours.ligne,
                                           lexeme_en_cours.colonne,
                                           "symbole invalide '%c'",
                                           premier_car);
                     }
                     etat = E_FIN;
                     break;
                  }

                  default:
                     erreur_lexicale(numero_ligne(),
                                     numero_colonne(),
                                     "caractere invalide '%c'",
                                     caractere_courant());
               }
               break;

            case E_NOMBRE:  /* reconnaissance d'un entier ou flottant */
               if (caractere_courant() == '.') {
                  if (point_present) {
                     erreur_lexicale(lexeme_en_cours.ligne,
                                     lexeme_en_cours.colonne,
                                     "nombre mal forme: plusieurs points dans \"%s\"",
                                     lexeme_en_cours.chaine);
                  }
                  point_present = 1;
                  lexeme_en_cours.nature = FLOTTANT;
                  ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                  avancer_car();
                  if (!est_chiffre(caractere_courant())) {
                     erreur_lexicale(lexeme_en_cours.ligne,
                                     lexeme_en_cours.colonne,
                                     "chiffre attendu apres le point decimal");
                  }
               }
               switch (nature_caractere(caractere_courant())) {
                  case CHIFFRE:
                     ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                     etat = E_NOMBRE;
                     avancer_car();
                     break;
                  default:
                     etat = E_FIN;
               }
               break;

            case E_FIN:    /* etat final */
               break;

         } /* fin switch(etat) */
      } /* fin while */

      if ((lexeme_en_cours.nature == ENTIER || lexeme_en_cours.nature == FLOTTANT) &&
          lexeme_en_cours.chaine[0] != '\0') {
         if (lexeme_en_cours.nature == ENTIER) {
            lexeme_en_cours.valeur = (int)strtol(lexeme_en_cours.chaine, NULL, 10);
            lexeme_en_cours.valeur_flottante = (double)lexeme_en_cours.valeur;
         } else {
            lexeme_en_cours.valeur_flottante = strtod(lexeme_en_cours.chaine, NULL);
         }
      }
   }

   /* --------------------------------------------------------------------- */

   void ajouter_caractere(char *s, char c) {
      int l = strlen(s);
      s[l]   = c;
      s[l+1] = '\0';
   }

   /* --------------------------------------------------------------------- */

   Nature_Caractere nature_caractere(char c) {
      if (fin_de_sequence_car()) return C_FIN_SEQUENCE;
      if (est_chiffre(c)) return CHIFFRE;
      if (est_symbole(c)) return SYMBOLE;
      if (est_lettre(c))  return MOT_CAR;
      return ERREUR_CAR;
   }

   /* --------------------------------------------------------------------- */

   int est_separateur(char c) {
      return c == ' ' || c == '\t' || c == '\n';
   }

   /* --------------------------------------------------------------------- */

   int est_chiffre(char c) {
      return c >= '0' && c <= '9';
   }

   /* --------------------------------------------------------------------- */

   int est_symbole(char c) {
      switch (c) {
         case '+': case '-': case '*': case '/':
         case '(': case ')': case '=': case ';':
         case '<': case '>': case '!':
            return 1;
         default:
            return 0;
      }
   }

   /* --------------------------------------------------------------------- */

   int est_lettre(char c) {
      return isalpha((unsigned char)c);
   }

   /* --------------------------------------------------------------------- */

   const char *Nature_vers_Chaine(Nature_Lexeme nature) {
      switch (nature) {
         case ENTIER:       return "ENTIER";
         case FLOTTANT:     return "FLOTTANT";
         case PLUS:         return "PLUS";
         case MOINS:        return "MOINS";
         case MUL:          return "MUL";
         case DIVISION:     return "DIVISION";
         case PARO:         return "PARO";
         case PARF:         return "PARF";
         case IDF:          return "IDF";
         case LIRE:         return "LIRE";
         case ECRIRE:       return "ECRIRE";
         case SI:           return "SI";
         case ALORS:        return "ALORS";
         case SINON:        return "SINON";
         case FSI:          return "FSI";
         case TANQUE:       return "TANQUE";
         case FAIRE:        return "FAIRE";
         case FAIT:         return "FAIT";
         case AFF:          return "AFF";
         case SEPINST:      return "SEPINST";
         case INF:          return "INF";
         case SUP:          return "SUP";
         case INF_EG:       return "INF_EG";
         case SUP_EG:       return "SUP_EG";
         case EGAL:         return "EGAL";
         case DIFF:         return "DIFF";
         case FIN_SEQUENCE: return "FIN_SEQUENCE";
         default:           return "ERREUR";
      }
   }

   /* --------------------------------------------------------------------- */

   void afficher(Lexeme l) {
      switch (l.nature) {
         case FIN_SEQUENCE:
            break;
         default:
            printf("(ligne %d, colonne %d) : ", l.ligne, l.colonne);
            printf("[nature = %s", Nature_vers_Chaine(l.nature));
            printf(", chaine = %s", l.chaine);
            switch (l.nature) {
               case ENTIER:
                  printf(", valeur = %d", l.valeur);
                  break;
               case FLOTTANT:
                  printf(", valeur = %g", l.valeur_flottante);
                  break;
               default:
                  break;
            }
            printf("]");
      }
   }

   /* --------------------------------------------------------------------- */
