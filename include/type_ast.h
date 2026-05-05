
#ifndef __TYPE_AST__
#define __TYPE_AST__

/* Types de noeuds de l'AST (conforme aux slides INF404) */
typedef enum {
    /* --- expressions arithmetiques --- */
    OPERATION,      /* operation binaire : gauche OP droite    */
    VALEUR,         /* valeur numerique constante              */
    N_IDF,          /* identificateur (variable)               */
    /* --- instructions --- */
    N_AFF,          /* affectation  : N_IDF = eag              */
    N_LIRE,         /* lecture      : lire(N_IDF)              */
    N_ECRIRE,       /* ecriture     : ecrire(eag)              */
    N_SEPINST,      /* sequence d'instructions (fils g et d)   */
    N_IF,           /* instruction conditionnelle              */
    N_WHILE,        /* instruction iterative (tanque/faire/fait)*/
    /* --- conditions (operateurs de comparaison) --- */
    N_INF,          /* <   */
    N_SUP,          /* >   */
    N_INF_EG,       /* <=  */
    N_SUP_EG,       /* >=  */
    N_EGAL,         /* ==  */
    N_DIFF          /* !=  */
} TypeAst ;

typedef enum {N_PLUS, N_MUL, N_MOINS, N_DIV} TypeOperateur ;

typedef struct noeud {
    TypeAst nature;
    TypeOperateur operateur;
    struct noeud *gauche, *droite;
    double valeur;
    char nom[256];  /* pour N_IDF, N_AFF, N_LIRE */
} NoeudAst;

typedef NoeudAst *Ast ;

#endif
