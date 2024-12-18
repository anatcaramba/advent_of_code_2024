#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 100000

void maj_positions(int* tas, int i, int* positions)
{
    positions[tas[i]] = i;
}

int extraire_min(int* tas, int n, int* positions, int* dists)
{
    int retour = tas[0];
    tas[0] = tas[n - 1];
    maj_positions(tas, 0, positions);
    int i = 0;
    while (2 * i + 1 < n) {
        int fils = 2 * i + 1;
        if (2 * i + 2 < n && dists[tas[2 * i + 2]] < dists[tas[2 * i + 1]]) {
            fils = 2 * i + 2;
        }
        if (dists[tas[i]] > dists[tas[fils]]) {
            int temp = tas[i];
            tas[i] = tas[fils];
            tas[fils] = temp;
            maj_positions(tas, i, positions);
            maj_positions(tas, fils, positions);
            i = fils;
        } else {
            break;
        }
    }
    return retour;
}

void diminuer(int* tas, int sommet, int* positions, int* dists)
{
    int i = positions[sommet];
    while (i >= 1 && dists[tas[(i - 1) / 2]] > dists[tas[i]]) {
        int pere = (i - 1) / 2;
        int temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions);
        maj_positions(tas, pere, positions);
        i = pere;
    }
}
void ajouter(int* tas, int n, int deb, int* positions, int* dists)
{
    tas[n] = deb;
    maj_positions(tas, n, positions);
    int i = n;
    while (i >= 1 && dists[tas[(i - 1) / 2]] > dists[tas[i]]) {
        int pere = (i - 1) / 2;
        int temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions);
        maj_positions(tas, pere, positions);
        i = pere;
    }
}
long dijkstra(int* grid, const int cote, int lim)
{
    int depart = 1 * cote + 1;
    int arrivee = (cote - 2) * cote + cote - 2;
    int* fp = malloc(cote * cote * sizeof(int));
    bool* visites = malloc(cote * cote * sizeof(bool));
    int* dists = malloc(cote * cote * sizeof(int));
    int* positions = malloc(cote * cote * sizeof(int));
    for (int i = 0; i < cote * cote; i += 1) {
        visites[i] = false;
        dists[i] = INT_MAX;
    }
    visites[depart] = true;
    dists[depart] = 0;
    fp[0] = depart;
    maj_positions(fp, 0, positions);
    int lentas = 1;
    while (lentas > 0) {
        int suivant = extraire_min(fp, lentas, positions, dists);
        int x = suivant / cote;
        int y = suivant % cote;
        lentas -= 1;
        for (int dx = -1; dx <= 1; dx += 1) {
            for (int dy = -1; dy <= 1; dy += 1) {
                if ((dx == 0) == (dy == 0)) {
                    continue;
                }
                int voisin = (x + dx) * cote + (y + dy);
                if (grid[voisin] != 0 && grid[voisin] <= lim) {
                    continue;
                }
                if (!visites[voisin]) {
                    visites[voisin] = true;
                    dists[voisin] = dists[suivant] + 1;
                    ajouter(fp, lentas, voisin, positions, dists);
                    lentas += 1;
                } else if (dists[suivant] + 1 < dists[voisin]) {
                    dists[voisin] = dists[suivant] + 1;
                    diminuer(fp, voisin, positions, dists);
                }
            }
        }
    }
    long ret = dists[arrivee];
    free(visites);
    free(fp);
    free(positions);
    free(dists);
    return ret;
}

int main(void)
{
    // LECTURE
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        fprintf(stderr, "Erreur : impossible de lire le fichier\n");
        return 1;
    }
    char inp[MAXLEN] = {};
    int nbread = fread(inp, sizeof(char), MAXLEN, stdin);
    if (nbread == 0) {
        fprintf(stderr, "Erreur : fichier non lu\n");
        return 2;
    }

    const int cote = 70;
    // const int cote = 6;
    const int cotel = cote + 3;
    int nb_bytes = 0;
    int* bytes = malloc(cotel * cotel * sizeof(int));
    int* grid = malloc(cotel * cotel * sizeof(int));
    for (int i = 0; i < cotel; i += 1) {
        for (int j = 0; j < cotel; j += 1) {
            grid[i * cotel + j] = 0;
        }
    }
    for (int i = 0; i < cotel; i += 1) {
        grid[i * cotel] = -1;
        grid[i * cotel + cotel - 1] = -1;
        grid[0 * cotel + i] = -1;
        grid[(cotel - 1) * cotel + i] = -1;
    }
    int i = 0;
    while (i < nbread - 1) {
        nb_bytes += 1;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        int sx = 0;
        while (isdigit(inp[i])) {
            sx = sx * 10 + inp[i] - '0';
            i += 1;
        }
        i += 1;
        int sy = 0;
        while (isdigit(inp[i])) {
            sy = sy * 10 + inp[i] - '0';
            i += 1;
        }
        i += 1;
        grid[(sx + 1) * cotel + sy + 1] = nb_bytes;
        bytes[nb_bytes - 1] = (sx + 1) * cotel + (sy + 1);
    }

    // ETOILE 1
    long ret1 = dijkstra(grid, cotel, 1024);
    int a = 1024;
    int b = nb_bytes;
    while (a <= b) {
        int m = (a + b) / 2;
        long ret = dijkstra(grid, cotel, m);
        if (ret < INT_MAX) {
            a = m + 1;
        } else {
            b = m - 1;
        }
    }

    // ETOILE 2
    printf("%ld\n", ret1);

    int byte = bytes[a - 1];
    printf("%d,%d\n", byte / cotel - 1, byte % cotel - 1);
    free(grid);
    free(bytes);
}
