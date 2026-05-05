#ifndef ANALYSE_LEXICALE_H
#define ANALYSE_LEXICALE_H

   typedef enum {
      ERREUR_CAR,      /* caractere incorrect          */
      CHIFFRE,         /* chiffre                      */
      SYMBOLE,         /* symbole                      */
      MOT_CAR,         /* suite de lettres             */
      C_FIN_SEQUENCE   /* caractere de fin de sequence */
   } Nature_Caractere ;

   typedef enum {
      /* --- litteraux --- */
      ENTIER,       /* sequence de chiffres            */
      FLOTTANT,     /* nombre avec separateur decimal  */
      /* --- operateurs arithmetiques (symboles) --- */
      PLUS,         /* +                               */
      MOINS,        /* -                               */
      MUL,          /* *                               */
      DIVISION,     /* /                               */
      /* --- parentheses --- */
      PARO,         /* (                               */
      PARF,         /* )                               */
      /* --- identificateur et mots-cles --- */
      IDF,          /* identificateur                  */
      LIRE,         /* mot-cle  lire                   */
      ECRIRE,       /* mot-cle  ecrire                 */
      SI,           /* mot-cle  si    (if)             */
      ALORS,        /* mot-cle  alors (then)           */
      SINON,        /* mot-cle  sinon (else)           */
      FSI,          /* mot-cle  fsi   (fi/endif)       */
      TANQUE,       /* mot-cle  tanque (tantque/while)  */
      FAIRE,        /* mot-cle  faire  (do)             */
      FAIT,         /* mot-cle  fait   (done/end)       */
      /* --- ponctuation --- */
      AFF,          /* =  (affectation)                */
      SEPINST,      /* ;  (separateur d'instructions)  */
      /* --- operateurs de comparaison --- */
      INF,          /* <                               */
      SUP,          /* >                               */
      INF_EG,       /* <=                              */
      SUP_EG,       /* >=                              */
      EGAL,         /* == (comparaison d'egalite)      */
      DIFF,         /* !=                              */
      /* --- fin --- */
      FIN_SEQUENCE  /* pseudo-lexeme fin de sequence   */
   } Nature_Lexeme ;


   typedef struct {
      Nature_Lexeme nature;
      unsigned int ligne;
      unsigned int colonne;
      char chaine[256];
      int valeur;
      double valeur_flottante;
   } Lexeme ;

   void afficher(Lexeme l);
   const char *Nature_vers_Chaine (Nature_Lexeme nature);

   void demarrer(char *nom_fichier);
   void avancer();
   Lexeme lexeme_courant();
   int fin_de_sequence();
   void arreter();

#endif /* ANALYSE_LEXICALE_H */
