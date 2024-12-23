# Advent of Code 2024 : mes solutions

Mes solutions sont en C, à l'exception du jour 19 en OCaml. Elles devraient fournir le bon résultat sur les entrées de tous les utilisateurs (mais pas forcément sur les exemples de taille plus petite).

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

### Jour 14

Jour plutôt cool et facile. La première partie ne demande que de calculer des modulos, je me suis quand même fait avoir par 1) l'inversion des axes ; 2) Le fait que `%` en C ne soit pas positif pour un dividende négatif.

Partie 2, je me suis rapidement rendu compte qu'il existait un pattern vertical apparaissant tous les 101 étapes (soit la largeur) depuis 11. Comme je ne trouvais pas la solution attendue de cette manière, je suis reparti de 0, et j'ai vu un pattern horizontal, cette fois, dans $89 + 103N$ (103 étant la hauteur). Il faudrait ensuite, pour être malin, utiliser le théorème chinois (101 et 103 étant premiers (donc entre eux)) mais je ne m'en souvenais pas et je n'avais pas de papier, donc j'ai résolu numériquement.

Pour cette raison je ne suis pas certain que ma solution se généralise, elle dépend de fait des valeurs initiales des patterns (pour moi 89 et 11). À la limite, il serait possible de détecter ces valeurs initiales en comptant le nombre de robots se situant dans la zone concernée.

### Jour 15

Encore un jour très agréable. La première partie se résout facilement en appliquant les règles. À chaque étape, on va regarder le plus loin possible tant qu'il y a des `O`. Si le caractère suivant est un `#`, on ne fait rien, sinon on décale tout. La présence d'un enclos de murs autour de la zone est un vrai soulagement, pas de tests aux limites (sinon, j'aurais pu de toute façon les rajouter, mais je ne le fais pas systématiquement).

La deuxième partie n'est pas si horrible qu'elle m'en a eu l'air au début. J'ai généralisé, en quelque sorte, l'approche précédente en réalisant un parcours en profondeur dans la direction du mouvement. Il suffit que l'une des caisses atteintes soit bloquée pour que tout le mouvement soit annulé. Je marque les cases à décaler, puis une dernière fonction réalise ces modifications. Je réalise cette dernière étape assez paresseusement en parcourant toute la grille dans le bon sens, il serait possible de faire mieux en faisant en fait un parcours en largeur. Ainsi il suffirait de décaler les cases visitées dans l'ordre postfixe du BFS.

### Jour 16

Neurones complètement grillés aujourd'hui. Partie 1 : Dijkstra presque standard. Partie 2 : j'ai modifié mon Dijkstra afin de construire un DAG mémorisant tous les chemins possibles. À chaque fois qu'on pourrait diminuer la distance d'un sommet ouvert mais pas encore fermé, si la distance est égale à celle déjà calculée, alors j'ajoute un ancêtre supplémentaire. Ensuite, j'ai fait un parcours du DAG à l'envers, depuis le sommet de fin, en comptant les sommets visités (sans distinguer cette fois l'orientation). Le bug qui m'a grillé les neurones est que je ne distinguais pas sommets ouverts et sommets sortis de la file de priorité, ce qui me conduisait à remodifier la distance de sommets défilés. Ce problème se produit-il dans un Dijkstra classique ? J'y penserai demain quand j'aurai récupéré mon cerveau.

### Jour 17

Mes neurones fonctionnent à nouveau. La partie 1 est triviale ; la partie 2 l'est beaucoup moins. Il ne suffit pas de tenter toutes les valeurs possibles en brute force ; j'ai essayé de faire du retro-engineering sur mon programme, mais je n'arrivais pas à grand chose. En fait, il me suffisait de remarquer la structure, puis de tenter un pseudo brute-force pour matcher à chaque étape le suffixe. Ce qui m'a pris du temps, c'est qu'il ne suffit pas de regarder les 8 valeurs possibles de `a % 8`, car la sortie peut dépendre de plus de bits de `a` que des trois derniers. Je pense que tout seul, il m'aurait fallu du temps pour arriver au résultat.

Ajout : en fait si, ça ne dépend que de 8 valeurs de `a % 8`, mais on n'est pas garanti de trouver alors le résultat de manière gloutonne. C'est pour ça que je "rattrape" des choix faits précédemment. Merci au collègue qui me l'a expliqué.

### Jour 18

Encore du plus court chemin ? Très simple aujourd'hui, j'aurais même pu faire un simple BFS (mais j'ai fait un Dijkstra par habitude). Partie 2, recherche dichotomique du seuil (puis relire la consigne pour imprimer la bonne valeur). RAS, vraiment.

### Jour 19

Journée algorithmique du texte : je me suis dit que je pourrais adapter l'implémentation de Glushkov faite avec mes élèves en OCaml. Il me restait, à partir d'un automate, à écrire la fonction décidant si un mot est accepté par cet automate, en plus des fonctions de lecture de fichier. Seulement, l'automate ainsi créé (non déterministe) possède plus de 2000 états ; au-delà de la 14e lettre du premier mot, comme il faut maintenir tous les états dans lesquels on peut se trouver, la taille de cet ensemble a tellement explosé qu'on n'avance plus. Bon, je n'avais pas du tout optimisé cette implémentation, mais il faut bien se rendre compte que c'est impossible ainsi. J'ai alors entrepris, en OCaml, de simplement tenter d'appliquer tous les patterns possibles, de maintenir l'ensemble des positions dans lesquelles on peut se trouver, et de renvoyer vrai ssi. on peut atteindre la position `n` (la longueur du mot). Cela m'a demandé, un peu lourdement, d'éliminer les doublons à chaque étape.

Puis finalement, partie 2, j'ai compris qu'il existait une "simple" relation de récurrence qui m'aurait simplifié également ma partie 1 ; mais qu'importe, je garde tout tel quel. Ma partie 1 prend quelque 4s avec `ocamlc`, et 500ms avec `ocamlopt`.

### Jour 20

Encore un problème de graphe 2D. Pas de grosse difficulté, j'ai en fait résolu quelque chose de plus général que ce que donnaient les input : un chemin unique du départ à l'arrivée. J'ai donc une solution assez lourde, avec plein de BFS partout. Pour la partie 1, j'ai même cherché manuellement les raccourcis possibles, là où une simple distance de Manhattan suffisait.

### Jour 21

Le problème le plus difficile de cette année pour l'instant. Par chance, j'avais correctement anticipé la partie 2, donc j'ai pu résoudre les deux parties d'un coup, mais cela m'a pris du temps. La formule de récurrence sur le nombre de boutons sur lesquels appuyer, je l'ai trouvée en 2 min sur papier ; puis j'ai buggé pendant des heures à essayer de l'implémenter en programmation dynamique (bottom-up), juste parce que je n'ai pas fait confiance à la dite formule. Une fois tout ceci corrigé, j'ai enfin bon sur l'exemple ; puis mauvaise réponse sur mon input. Je suis alors rentré dans la deuxième phase de corrections : grâce aux collègues, je me suis rendu compte que les déplacements sur un clavier n'étaient pas tous équivalents. Je considérais seulement des déplacements "sans alternance", c'est-à-dire `>>^` ou `^>>` mais jamais `>^>`, ce qui est une hypothèse correcte à première vue, et qui aujourd'hui me paraît clairement vraie. Mais il se trouve que les deux premiers déplacements ne sont eux-mêmes pas équivalents. Pourtant, j'avais envisagé cela, mais m'étais dit que comme toutes ces séquences de mouvements sont encadrées par des `A`, alors le coût est le même. Mais si c'est vrai au rang `n`, ce n'est pas vrai au rang `n + 1`. Avec mon hypothèse, cela fait donc parfois 1, parfois 2 chemins à envisager, et il faut prendre celui qui donne le minimum de coups. La formule de récurrence n'en est pas trop modifiée. Après ça, on trouve le résultat attendu ! Partie 2, j'ai juste eu à changer la taille de mon tableau de programmation dynamique.

Il apparaîtrait, d'après un collègue, que le meilleur chemin entre deux touches soit invariablement le même (peu importe la profondeur). Cependant les raisons pour lesquelles ces chemins sont les meilleurs ne sont pas forcément claires. Il est certain que l'enchaînement de `<` et de `A` est coûteux car il maximise le déplacement ; mais après...

### Jour 22

Jour simple. Rien à dire sur la partie 1. Partie 2, il vaut mieux mémoriser les scores des séquences rencontrées que de toutes les tenter ; on y gagne un énorme facteur, mais je pensais que ça irait, donc naïvement j'avais commencé par cette seconde approche.

### Jour 23

Recherche de cliques. Pour la partie 1 (lister certaines 3-cliques) je suis assez content d'avoir codé quelque chose de correct du premier coup et assez rapidement, en tenant compte des doublons/triplets. Partie 2, j'ai utilisé l'algorithme de Bron-Kerbosch (que je ne connaissais pas). Comme la clique maximum est de taille 13, tout va bien. Au début, j'avais eu un peu peur, car il y a quand même plus de 3000 sommets.
