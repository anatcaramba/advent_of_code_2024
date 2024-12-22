#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

int* bfs(char* grid, const int hei, const int wid, int source)
{
    int* fp = malloc(hei * wid * sizeof(int));
    bool* visites = malloc(hei * wid * sizeof(bool));
    int* dists = malloc(hei * wid * sizeof(int));
    for (int i = 0; i < hei * wid; i += 1) {
        visites[i] = false;
        dists[i] = INT_MAX;
    }
    visites[source] = true;
    dists[source] = 0;
    fp[0] = source;
    int q0 = 0;
    int q1 = 1;
    while (q0 < q1) {
        int sommet = fp[q0];
        q0 += 1;
        int x = sommet / wid;
        int y = sommet % wid;
        for (int dx = -1; dx <= 1; dx += 1) {
            for (int dy = -1; dy <= 1; dy += 1) {
                if ((dx == 0) == (dy == 0)) {
                    continue;
                }
                int voisin = (x + dx) * wid + (y + dy);
                if (visites[voisin] || grid[voisin] == '#') {
                    continue;
                }
                visites[voisin] = true;
                dists[voisin] = dists[sommet] + 1;
                fp[q1] = voisin;
                q1 += 1;
            }
        }
    }
    free(visites);
    free(fp);
    return dists;
}
int* bfs_cheat(const int hei, const int wid, int source)
{
    int* fp = malloc(hei * wid * sizeof(int));
    bool* visites = malloc(hei * wid * sizeof(bool));
    int* dists = malloc(hei * wid * sizeof(int));
    for (int i = 0; i < hei * wid; i += 1) {
        visites[i] = false;
        dists[i] = INT_MAX;
    }
    visites[source] = true;
    dists[source] = 0;
    fp[0] = source;
    int q0 = 0;
    int q1 = 1;
    while (q0 < q1) {
        int sommet = fp[q0];
        q0 += 1;
        int x = sommet / wid;
        int y = sommet % wid;
        for (int dx = -1; dx <= 1; dx += 1) {
            for (int dy = -1; dy <= 1; dy += 1) {
                if ((dx == 0) == (dy == 0)) {
                    continue;
                }
                if (x + dx < 0 || x + dx >= hei || y + dy < 0 || y + dy >= wid) {
                    continue;
                }
                int voisin = (x + dx) * wid + (y + dy);
                if (visites[voisin]) {
                    continue;
                }
                visites[voisin] = true;
                dists[voisin] = dists[sommet] + 1;
                if (dists[voisin] < 20) {
                    fp[q1] = voisin;
                    q1 += 1;
                }
            }
        }
    }
    free(visites);
    free(fp);
    return dists;
}
void remplir_cheats(char* grid, int* cheats, int hei, int wid, int* dists_d, int* dists_a, int i, int j)
{
    if (grid[i * wid + j] == '#') {
        return;
    }
    int ind_cheat = 8 * (i * wid + j);
    if (grid[i * wid + j + 1] == '#') {
        if (j + 2 < wid && grid[i * wid + j + 2] != '#') {
            int new_d = dists_a[i * wid + j + 2] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat] = new_d;
        }
        if (grid[(i + 1) * wid + (j + 1)] != '#') {
            int new_d = dists_a[(i + 1) * wid + j + 1] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 1] = new_d;
        }
        if (grid[(i - 1) * wid + (j + 1)] != '#') {
            int new_d = dists_a[(i - 1) * wid + j + 1] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 7] = new_d;
        }
    }
    if (grid[i * wid + j - 1] == '#') {
        if (j - 2 >= 0 && grid[i * wid + j - 2] != '#') {
            int new_d = dists_a[i * wid + j - 2] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 2] = new_d;
        }
        if (grid[(i + 1) * wid + (j - 1)] != '#') {
            int new_d = dists_a[(i + 1) * wid + j - 1] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 3] = new_d;
        }
        if (grid[(i - 1) * wid + (j - 1)] != '#') {
            int new_d = dists_a[(i - 1) * wid + j - 1] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 5] = new_d;
        }
    }
    if (grid[(i + 1) * wid + j] == '#') {
        if (i + 2 < hei && grid[(i + 2) * wid + j] != '#') {
            int new_d = dists_a[(i + 2) * wid + j] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 4] = new_d;
        }
        if (grid[(i + 1) * wid + (j + 1)] != '#') {
            int new_d = dists_a[(i + 1) * wid + j + 1] + dists_d[i * wid + j] + 2;
            if (new_d < cheats[ind_cheat + 1]) {
                cheats[ind_cheat + 1] = new_d;
            }
        }
        if (grid[(i + 1) * wid + (j - 1)] != '#') {
            int new_d = dists_a[(i + 1) * wid + j - 1] + dists_d[i * wid + j] + 2;
            if (new_d < cheats[ind_cheat + 3]) {
                cheats[ind_cheat + 3] = new_d;
            }
        }
    }
    if (grid[(i - 1) * wid + j] == '#') {
        if (i - 2 >= 0 && grid[(i - 2) * wid + j] != '#') {
            int new_d = dists_a[(i - 2) * wid + j] + dists_d[i * wid + j] + 2;
            cheats[ind_cheat + 6] = new_d;
        }
        if (grid[(i - 1) * wid + (j + 1)] != '#') {
            int new_d = dists_a[(i - 1) * wid + j + 1] + dists_d[i * wid + j] + 2;
            if (new_d < cheats[ind_cheat + 7]) {
                cheats[ind_cheat + 7] = new_d;
            }
        }
        if (grid[(i - 1) * wid + (j - 1)] != '#') {
            int new_d = dists_a[(i - 1) * wid + j - 1] + dists_d[i * wid + j] + 2;
            if (new_d < cheats[ind_cheat + 5]) {
                cheats[ind_cheat + 5] = new_d;
            }
        }
    }
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

    char* grid = malloc(MAXLEN * sizeof(int));
    int wid = 0;
    while (inp[wid] != '\n') {
        wid += 1;
    }
    int hei = 0;
    int i = 0;
    int depart = -1;
    int arrivee = -1;
    while (i < nbread - 1) {
        for (int j = 0; j < wid; j += 1) {
            grid[wid * hei + j] = inp[i];
            if (inp[i] == 'S') {
                depart = wid * hei + j;
            } else if (inp[i] == 'E') {
                arrivee = wid * hei + j;
            }
            i += 1;
        }
        i += 1;
        hei += 1;
    }

    // ETOILE 1
    long ret1 = 0;
    int* dists_d = bfs(grid, hei, wid, depart);
    int* dists_a = bfs(grid, hei, wid, arrivee);

    int* cheats = malloc(8 * wid * hei * sizeof(int));
    for (int i = 0; i < 8 * wid * hei; i += 1) {
        cheats[i] = INT_MAX;
    }
    for (int i = 1; i < hei - 1; i += 1) {
        for (int j = 1; j < wid - 1; j += 1) {
            remplir_cheats(grid, cheats, hei, wid, dists_d, dists_a, i, j);
        }
    }
    int dist_vanille = dists_d[arrivee];
    for (int i = 0; i < 8 * wid * hei; i += 1) {
        if (cheats[i] <= dist_vanille - 100) {
            ret1 += 1;
        }
    }
    printf("%ld\n", ret1);
    // ETOILE 2
    long ret2 = 0;
    for (int i = 0; i < hei * wid; i += 1) {
        if (grid[i] == '#') {
            continue;
        }
        int* dists = bfs_cheat(hei, wid, i);
        for (int j = 0; j < hei * wid; j += 1) {
            if (grid[j] == '#' || dists[j] > 20 || dists_d[i] > wid * hei || dists_a[j] > wid * hei) {
                continue;
            }
            int new_d = dists_d[i] + dists_a[j] + dists[j];
            if (new_d + 100 <= dist_vanille) {
                ret2 += 1;
            }
        }
        free(dists);
    }
    printf("%ld\n", ret2);
    free(dists_d);
    free(dists_a);
    free(cheats);
    free(grid);
}
