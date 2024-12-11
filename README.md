# Advent of Code 2024 : mes solutions

Mes solutions sont pour l'instant toutes en C. Elles devraient fournir le bon résultat sur les entrées de tous les utilisateurs (mais pas forcément sur les exemples de taille plus petite).

## Remarques

### Jour 11

J'ai commencé par réaliser la partie 1 avec des listes doublement chaînées. Pour 25 étapes, c'est tout à fait gérable en temps mais cela devient affreux en temps ET en mémoire pour un nombre d'étapes dépassant 35. Donc pas possible de faire la partie 2 ainsi...

Heureusement, l'ordre des pierres ne compte absolument pas, malgré un petit signal contradictoire. Donc finalement, une version récursive avec mémoïsation dans une table de hachage customisée fait l'affaire (pour les deux parties).
