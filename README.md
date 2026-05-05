# Mini-interpréteur en C

Projet universitaire réalisé en C autour de la construction d’un mini-interpréteur pour un langage impératif simple. Le programme lit un fichier source, effectue une analyse lexicale et syntaxique, construit un arbre abstrait (AST), puis interprète les instructions reconnues.

> Note : ce dépôt contient un mini-interpréteur. Il ne contient pas d’implémentation SAT, CNF/DIMACS ou Akari.

## Objectif

L’objectif du projet est de mettre en pratique les bases de la compilation et de l’interprétation :

- lecture d’un fichier source ;
- découpage en lexèmes ;
- analyse syntaxique descendante récursive ;
- construction et parcours d’un AST ;
- évaluation d’expressions arithmétiques ;
- interprétation d’instructions simples avec une table des symboles.

## Technologies utilisées

- Langage C ;
- Makefile ;
- GCC ;
- structures de données simples pour l’AST et la table des symboles.

## Structure du projet

```text
.
├── include/        # Fichiers d’en-tête
├── src/            # Fichiers sources C
├── examples/       # Programmes d’exemple
├── tests/          # Fichiers de test valides et invalides
├── docs/           # Rapport du projet
├── Makefile        # Compilation du projet
└── README.md       # Documentation
```

Les exécutables et fichiers objets sont générés dans `bin/` et `build/`.

## Compilation

Depuis la racine du projet :

```bash
make
```

Exécutables produits :

- `bin/interpreteur` : interpréteur de programmes ;
- `bin/calculette` : évaluation d’expressions arithmétiques ;
- `bin/test_lexeme` : affichage des lexèmes reconnus ;
- `bin/essai_ast` : essais manuels sur l’AST.

Pour supprimer les fichiers générés :

```bash
make clean
```

## Exécution

Interpréter un programme :

```bash
./bin/interpreteur examples/ex1_arithmetique.txt
```

Afficher l’AST avant exécution :

```bash
./bin/interpreteur --verbose examples/ex2_condition.txt
```

Évaluer une expression arithmétique :

```bash
./bin/calculette tests/valides/calculette/expr_plus_mult.txt
```

Afficher les lexèmes d’un fichier :

```bash
./bin/test_lexeme examples/ex1_arithmetique.txt
```

## Exemple de programme accepté

```text
somme = 0 ;
i = 1 ;
tanque i <= 5 faire
    somme = somme + i ;
    i = i + 1
fait ;
ecrire(somme)
```

Exécution :

```bash
./bin/interpreteur examples/ex3_boucle.txt
```

## Langage supporté

Le langage gère notamment :

- affectation : `x = expr` ;
- lecture : `lire(x)` ;
- affichage : `ecrire(expr)` ;
- condition : `si ... alors ... sinon ... fsi` ;
- boucle : `tanque ... faire ... fait` ;
- opérateurs arithmétiques : `+`, `-`, `*`, `/` ;
- comparaisons : `<`, `>`, `<=`, `>=`, `==`, `!=`.

## Tests

Une cible de vérification simple est fournie :

```bash
make test
```

Elle compile le projet puis exécute quelques exemples représentatifs. Les dossiers `tests/valides/` et `tests/erreurs/` contiennent des cas supplémentaires pouvant être lancés manuellement.

## Compétences mises en œuvre

- programmation en C modulaire ;
- décomposition d’un projet en sources, en-têtes, exemples et tests ;
- analyse lexicale et syntaxique ;
- manipulation d’arbres abstraits ;
- gestion d’une table de symboles ;
- compilation avec Makefile ;
- documentation technique claire et reproductible.
# Interpréteur_calculette
