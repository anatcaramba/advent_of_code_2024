// #include <ctype.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

typedef struct t_sommet t_sommet;
struct t_sommet {
    int x;
    int y;
    int dx;
    int dy;
    int dist;
};

int hash(t_sommet s, int hei, int wid)
{
    return s.x * 9 * wid + s.y * 9 + (s.dx + 1) * 3 + (s.dy + 1);
}

void maj_positions(t_sommet* tas, int i, int* positions, int hei, int wid)
{
    positions[hash(tas[i], hei, wid)] = i;
}

t_sommet extraire_min(t_sommet* tas, int n, int* positions, int hei, int wid)
{
    t_sommet retour = tas[0];
    tas[0] = tas[n - 1];
    maj_positions(tas, 0, positions, hei, wid);
    int i = 0;
    while (2 * i + 1 < n) {
        int fils = 2 * i + 1;
        if (2 * i + 2 < n && tas[2 * i + 2].dist < tas[2 * i + 1].dist) {
            fils = 2 * i + 2;
        }
        if (tas[i].dist > tas[fils].dist) {
            t_sommet temp = tas[i];
            tas[i] = tas[fils];
            tas[fils] = temp;
            maj_positions(tas, i, positions, hei, wid);
            maj_positions(tas, fils, positions, hei, wid);
            i = fils;
        } else {
            break;
        }
    }
    return retour;
}

int diminuer(t_sommet* tas, t_sommet adiminuer, int* positions, int hei, int wid)
{
    int i = positions[hash(adiminuer, hei, wid)];
    assert(tas[i].x == adiminuer.x);
    assert(tas[i].y == adiminuer.y);
    assert(tas[i].dx == adiminuer.dx);
    assert(tas[i].dy == adiminuer.dy);
    if (tas[i].dist <= adiminuer.dist) {
        if (tas[i].dist == adiminuer.dist) {
            return 1;
        }
        return -1;
    }
    tas[i].dist = adiminuer.dist;
    while (i >= 1 && tas[(i - 1) / 2].dist > tas[i].dist) {
        int pere = (i - 1) / 2;
        t_sommet temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions, hei, wid);
        maj_positions(tas, pere, positions, hei, wid);
        i = pere;
    }
    return 0;
}
void ajouter(t_sommet* tas, int n, t_sommet deb, int* positions, int hei, int wid)
{
    tas[n] = deb;
    maj_positions(tas, n, positions, hei, wid);
    int i = n;
    while (i >= 1 && tas[(i - 1) / 2].dist > tas[i].dist) {
        int pere = (i - 1) / 2;
        t_sommet temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions, hei, wid);
        maj_positions(tas, pere, positions, hei, wid);
        i = pere;
    }
}

int parcours(t_sommet* prevs, int* nbprevs, int xs, int ys, t_sommet fin, int hei, int wid)
{
    int lentabs = hei * wid * 9;
    bool* vis = malloc(lentabs * sizeof(bool));
    bool* vispos = malloc(hei * wid * sizeof(bool));
    for (int i = 0; i < lentabs; i += 1) {
        vis[i] = false;
    }
    for (int i = 0; i < wid * hei; i += 1) {
        vispos[i] = false;
    }
    t_sommet* to_vis = malloc(lentabs * sizeof(t_sommet));
    to_vis[0] = fin;
    vis[hash(fin, hei, wid)] = true;
    int nb_vis = 0;
    int lenpile = 1;
    while (lenpile > 0) {
        lenpile -= 1;
        t_sommet s = to_vis[lenpile];
        if (!vispos[s.x * wid + s.y]) {
            nb_vis += 1;
            vispos[s.x * wid + s.y] = true;
        }
        int h = hash(s, hei, wid);
        for (int i = 0; i < nbprevs[h]; i += 1) {
            t_sommet s0 = prevs[4 * h + i];
            int h0 = hash(s0, hei, wid);
            if (vis[h0]) {
                continue;
            }
            vis[h0] = true;
            to_vis[lenpile] = prevs[h * 4 + i];
            lenpile += 1;
        }
    }
    free(vis);
    free(to_vis);
    free(vispos);
    return nb_vis;
}

void dijkstra(char* grid, int xs, int ys, int xe, int ye, int hei, int wid)
{
    t_sommet fin = {};
    int sizegrid = hei * wid;
    long lentabs = sizegrid * 3 * 3;
    t_sommet* tas = malloc(lentabs * sizeof(t_sommet));
    long ret1 = LONG_MAX;
    int* positions = malloc(lentabs * sizeof(int));
    int* visites = malloc(lentabs * sizeof(int));
    t_sommet* prevs = malloc(4 * lentabs * sizeof(t_sommet));
    int* nbprevs = malloc(lentabs * sizeof(int));
    for (int i = 0; i < lentabs; i += 1) {
        visites[i] = 0;
        nbprevs[i] = 0;
    }
    tas[0] = (t_sommet) { .dist = 0, .x = xs, .y = ys, .dx = 0, .dy = 1 };
    maj_positions(tas, 0, positions, hei, wid);
    int lentas = 1;
    bool endfound = false;
    visites[hash(tas[0], hei, wid)] = 1;
    while (lentas > 0) {
        t_sommet sommet = extraire_min(tas, lentas, positions, hei, wid);
        visites[hash(sommet, hei, wid)] = 2;
        lentas -= 1;
        int x = sommet.x;
        int y = sommet.y;
        int dx = sommet.dx;
        int dy = sommet.dy;
        int dist = sommet.dist;
        if (!endfound && x == xe && y == ye) {
            endfound = true;
            ret1 = dist;
            fin = sommet;
        }
        for (int ndx = -1; ndx <= 1; ndx += 1) {
            for (int ndy = -1; ndy <= 1; ndy += 1) {
                if (((ndx == 0) == (ndy == 0)) || (ndx == dx && ndy == dy) || (ndx == -dx && ndy == -dy)) {
                    continue;
                }
                if (grid[(x + ndx) * wid + (y + ndy)] == '#') {
                    continue;
                }
                int nouvdist = dist + 1000;
                t_sommet nouveau = (t_sommet) { .x = x, .y = y, .dx = ndx, .dy = ndy, .dist = nouvdist };
                int h = hash(nouveau, hei, wid);
                if (visites[h] == 2) {
                    continue;
                }
                if (visites[h] == 1) {
                    int chemin = diminuer(tas, nouveau, positions, hei, wid);
                    if (chemin > 0) {
                        prevs[h * 4 + nbprevs[h]] = sommet;
                        nbprevs[h] += 1;
                    } else if (chemin == 0) {
                        prevs[h * 4] = sommet;
                        nbprevs[h] = 1;
                    }
                    continue;
                }
                visites[h] = 1;
                prevs[h * 4] = sommet;
                nbprevs[h] = 1;
                ajouter(tas, lentas, nouveau, positions, hei, wid);
                lentas += 1;
            }
        }
        if (grid[(x + dx) * wid + y + dy] == '#') {
            continue;
        }
        int nouvdist = dist + 1;
        t_sommet nouveau = (t_sommet) { .x = x + dx, .y = y + dy, .dx = dx, .dy = dy, .dist = nouvdist };
        int h = hash(nouveau, hei, wid);
        if (visites[h] == 2) {
            continue;
        }
        if (visites[h] == 1) {
            int chemin = diminuer(tas, nouveau, positions, hei, wid);
            if (chemin > 0) {
                prevs[h * 4 + nbprevs[h]] = sommet;
                nbprevs[h] += 1;
            } else if (chemin == 0) {
                prevs[h * 4] = sommet;
                nbprevs[h] = 1;
            }
            continue;
        }
        visites[h] = 1;
        prevs[h * 4] = sommet;
        nbprevs[h] = 1;
        ajouter(tas, lentas, nouveau, positions, hei, wid);
        lentas += 1;
    }
    long ret2 = parcours(prevs, nbprevs, xs, ys, fin, hei, wid);
    printf("%ld\n%ld\n", ret1, ret2);
    free(tas);
    free(positions);
    free(visites);
    free(prevs);
    free(nbprevs);
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

    char* grid = malloc(MAXLEN * sizeof(char));
    int wid = 0;
    while (inp[wid] != '\n') {
        wid += 1;
    }
    int i = 0;
    int hei = 0;
    int xs = -1;
    int ys = -1;
    int xe = -1;
    int ye = -1;
    while (i < nbread - 1) {
        for (int j = 0; j < wid; j += 1) {
            grid[wid * hei + j] = inp[i];
            if (inp[i] == 'S') {
                xs = hei;
                ys = j;
            }
            if (inp[i] == 'E') {
                xe = hei;
                ye = j;
            }
            i += 1;
        }
        i += 1;
        hei += 1;
    }
    i += 1;

    // ETOILES
    dijkstra(grid, xs, ys, xe, ye, hei, wid);

    free(grid);
}
