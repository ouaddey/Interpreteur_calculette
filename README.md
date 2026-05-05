# Mini-interpreteur en C

Projet universitaire realise en C autour de la construction d'un mini-interpreteur pour un langage imperatif simple. Le programme lit un fichier source, effectue une analyse lexicale et syntaxique, construit un arbre abstrait (AST), puis interprete les instructions reconnues.


## Objectif

L'objectif du projet est de mettre en pratique les bases de la compilation et de l'interpretation :

- lecture d'un fichier source ;
- decoupage en lexemes ;
- analyse syntaxique descendante recursive ;
- construction et parcours d'un AST ;
- evaluation d'expressions arithmetiques ;
- interpretation d'instructions simples avec une table des symboles.

## Technologies utilisees

- Langage C ;
- Makefile ;
- GCC ;
- structures de donnees simples pour l'AST et la table des symboles.

## Structure du projet

```text
.
├── include/        # Fichiers d'en-tete
├── src/            # Fichiers sources C
├── examples/       # Programmes d'exemple
├── tests/          # Fichiers de test valides et invalides
├── docs/           # Rapport du projet
├── Makefile        # Compilation du projet
└── README.md       # Documentation
```

Les executables et fichiers objets sont generes dans `bin/` et `build/`.

## Compilation

Depuis la racine du projet :

```bash
make
```

Executables produits :

- `bin/interpreteur` : interpreteur de programmes ;
- `bin/calculette` : evaluation d'expressions arithmetiques ;
- `bin/test_lexeme` : affichage des lexemes reconnus ;
- `bin/essai_ast` : essais manuels sur l'AST.

Pour supprimer les fichiers generes :

```bash
make clean
```

## Execution

Interpreter un programme :

```bash
./bin/interpreteur examples/ex1_arithmetique.txt
```

Afficher l'AST avant execution :

```bash
./bin/interpreteur --verbose examples/ex2_condition.txt
```

Evaluer une expression arithmetique :

```bash
./bin/calculette tests/valides/calculette/expr_plus_mult.txt
```

Afficher les lexemes d'un fichier :

```bash
./bin/test_lexeme examples/ex1_arithmetique.txt
```

## Exemple de programme accepte

```text
somme = 0 ;
i = 1 ;
tanque i <= 5 faire
    somme = somme + i ;
    i = i + 1
fait ;
ecrire(somme)
```

Execution :

```bash
./bin/interpreteur examples/ex3_boucle.txt
```

## Langage supporte

Le langage gere notamment :

- affectation : `x = expr` ;
- lecture : `lire(x)` ;
- affichage : `ecrire(expr)` ;
- condition : `si ... alors ... sinon ... fsi` ;
- boucle : `tanque ... faire ... fait` ;
- operateurs arithmetiques : `+`, `-`, `*`, `/` ;
- comparaisons : `<`, `>`, `<=`, `>=`, `==`, `!=`.

## Tests

Une cible de verification simple est fournie :

```bash
make test
```

Elle compile le projet puis execute quelques exemples representatifs. Les dossiers `tests/valides/` et `tests/erreurs/` contiennent des cas supplementaires pouvant etre lances manuellement.

## Competences mises en oeuvre

- programmation en C modulaire ;
- decomposition d'un projet en sources, en-tetes, exemples et tests ;
- analyse lexicale et syntaxique ;
- manipulation d'arbres abstraits ;
- gestion d'une table de symboles ;
- compilation avec Makefile ;
- documentation technique claire et reproductible.
# Interpreteur_calculette
