# Advent of Code 2024 : mes solutions

Mes solutions sont pour l'instant toutes en C. Elles devraient fournir le bon résultat sur les entrées de tous les utilisateurs (mais pas forcément sur les exemples de taille plus petite).

## Remarques

### Jour 1

Utilisation d'un tri. R.A.S.

### Jour 2

Pas de difficulté partie 1. Partie 2, essais infructueux de résoudre en temps linéaire par ligne (trop de cas particuliers), mais il doit exister une manière. Donc résolu en temps quadratique en supprimant les éléments individuellement.

### Jour 3

En C sans expression régulière, donc un exercice de parsing sans difficulté algorithmique, avec un code un peu lourd mais fonctionnel.

### Jour 4

Résolu rapidement. Pour ce genre de problèmes où il faut considérer plusieurs directions sur une grille, il est pratique d'utiliser une double boucle pour tous `dx = -1, 0, 1` et `dy = -1, 0, 1` en éliminant potentiellement les directions interdites.

### Jour 5

Code très laid, avec une première partie assez simple, puis une deuxième où j'ai opté pour une recherche de médiane en temps quadratique (sans trier, donc).

### Jour 6

Pas de grosse difficulté ; les cycles se détectent au bout d'un certain nombre d'itérations, sans donc garder en mémoire les états (position + orientation) visités. En prenant le nombre de cases fois 4 comme nombre d'itérations, on est en tout cas sûr de ne pas s'arrêter trop tôt !

### Jour 7

La première partie met à profit les bitmasks pour symboliser les $2^p$ choix de suite d'opérations possibles ; puis pour la partie 2, on implémente des faux bitmasks en base `3` pour les $3^p$ choix.

### Jour 8

Pas trop compliqué en calculant les vecteurs entre paires de points ; je n'ai même pas eu besoin d'arithmétique pour la partie 2, car additionner la distance en boucle donne tous les points souhaités en un temps immédiat.

### Jour 9

La partie 1 est implémentée à l'aide de deux indices, l'un (`i`) parcourant la liste de gauche à droite, et `j` de droite à gauche. Ainsi, on calcule les checksums à la volée en calculant au passage la position `d` réelle des fichiers.

La partie 2 est un peu plus subtile. Il semblerait qu'un passage quadratique ait été possible, mais j'ai ici fait le choix de 10 tas min (en réalité 9, pour chaque taille de trou), où l'on stocke le trou de taille `k` le plus à gauche. Pour chaque fichier à insérer (considérés de droite à gauche), on trouve le trou en temps constant (plus suppression en $\log(\frac{n}{10})$ et on insère potentiellement un trou plus petit dans un autre tas.

### Jour 10

Confusion initiale : j'ai implémenté la partie 2 en 1, la 1 étant en fait plus difficile. Mais elle ne l'est pas beaucoup ; en précalculant la position de tous les `i` pour `i = 0 .. 9` je fais un semblant de parcours en largeur. La subtilité est, dans la partie 1, de ne pas compter de doublons, ce que je fais de manière un peu lourde avec un parcours linéaire à chaque fois.

### Jour 11

J'ai commencé par réaliser la partie 1 avec des listes doublement chaînées. Pour 25 étapes, c'est tout à fait gérable en temps mais cela devient affreux en temps ET en mémoire pour un nombre d'étapes dépassant 35. Donc pas possible de faire la partie 2 ainsi...

Heureusement, l'ordre des pierres ne compte absolument pas, malgré un petit signal contradictoire. Donc finalement, une version récursive avec mémoïsation dans une table de hachage customisée fait l'affaire (pour les deux parties).

### Jour 12

La première partie utilise un parcours en profondeur pour calculer les aires des zones (composantes connexes). De plus, à chaque sommet voisin (en comptant l'extérieur de la grille), si ce voisin est dans une autre zone, on ajoute 1 au périmètre.

La deuxième partie ne présente pas de grande difficulté algorithmique supplémentaire, mais est assez fastidieuse. J'ai calculé, pour trouver le nombre de côtés des zones, le nombre de coins. Les coins "aigüs" (convexes) sont facilement détectables. Les coins que j'appelle "obtus" (on pourrait dire plutôt dire concaves) sont plus difficiles à détecter à partir du parcours, je les ai donc détectés depuis le parcours de la zone "complémentaire" à ce coin, puis j'ai rattaché les coins concaves à leur zone via un précalcul des représentants de chaque composante... En bref, beaucoup de tests dans les 4 directions, beaucoup de structures additionnelles, il y aurait finalement de la place pour quelques simplifications.

### Jour 13

Beaucoup trop de temps à essayer de programmer une solution récursive à la partie 1.

Beaucoup trop de temps à faire fonctionner Dijkstra pour la partie 1.

Beaucoup, beaucoup trop de temps à essayer d'adapter ce Dijkstra à la partie 2... Je me suis fait avoir par deux choses. D'abord, l'énoncé qui parle de trouver un plus court chemin. Ensuite, le fait qu'après avoir fait mon Dijkstra, je me suis rendu compte qu'un raccourci était possible ; on tombe en effet rapidement sur un vecteur colinéaire à la cible. Il suffit alors de multiplier ce vecteur pour trouver la solution attendue. Or, les entrées sont programmées de sorte qu'on ne trouve *pas* un tel vecteur colinéaire une fois la cible agrandie, en tout cas, pas dans les limites de ma mémoire.

J'ai finalement compris qu'il fallait résoudre le système, ce qui rend le problème du jour quasiment risible. La leçon est de ne pas toujours penser en informaticien, et de réactiver ses maths du collège...

En plus, mon tout premier réflexe a été de penser au théorème chinois... Puis de me dire que non, décidément, la solution ne serait pas des maths...
