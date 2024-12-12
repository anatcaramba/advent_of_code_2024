#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

typedef struct zone zone;
struct zone {
    int nbvis;
    int nbperi;
    int coinsext;
};

bool inbounds(int i, int j, int hei, int wid)
{
    return i >= 0 && i < hei && j >= 0 && j < wid;
}

void coins_interieurs(char* grid, int hei, int wid, int i, int j, int* cint, int* representants)
{
    char c = grid[i * wid + j];
    if (inbounds(i + 1, j + 1, hei, wid)) {
        char d = grid[(i + 1) * wid + j + 1];
        if (c != d) {
            if (grid[(i + 1) * wid + j] == d && grid[i * wid + j + 1] == d) {
                cint[representants[(i + 1) * wid + j + 1]] += 1;
            }
        }
    }
    if (inbounds(i + 1, j - 1, hei, wid)) {
        char d = grid[(i + 1) * wid + j - 1];
        if (c != d && inbounds(i + 1, j, hei, wid) && inbounds(i, j - 1, hei, wid)) {
            if (grid[(i + 1) * wid + j] == d && grid[i * wid + j - 1] == d) {
                cint[representants[(i + 1) * wid + j - 1]] += 1;
            }
        }
    }
    if (inbounds(i - 1, j - 1, hei, wid)) {
        char d = grid[(i - 1) * wid + j - 1];
        if (c != d && inbounds(i - 1, j, hei, wid) && inbounds(i, j - 1, hei, wid)) {
            if (grid[(i - 1) * wid + j] == d && grid[i * wid + j - 1] == d) {
                cint[representants[(i - 1) * wid + j - 1]] += 1;
            }
        }
    }
    if (inbounds(i - 1, j + 1, hei, wid)) {
        char d = grid[(i - 1) * wid + j + 1];
        if (c != d && inbounds(i, j + 1, hei, wid) && inbounds(i - 1, j, hei, wid)) {
            if (grid[i * wid + j + 1] == d && grid[(i - 1) * wid + j] == d) {
                cint[representants[(i - 1) * wid + j + 1]] += 1;
            }
        }
    }
}

zone parcours(char* grid, int hei, int wid, int* visited, int i, int j, int repr, int* representants)
{
    int nbvis = 1;
    int nb_peri = 0;
    int coinsext = 0;
    int pdx = INT_MAX;
    int pdy = INT_MAX;
    int maybe_coinsext = 0;
    bool en_face = true;
    representants[i * wid + j] = repr;
    for (int dx = -1; dx <= 1; dx += 1) {
        for (int dy = -1; dy <= 1; dy += 1) {
            if (((dx == 0) != (dy == 0))) {
                if (!inbounds(i + dx, j + dy, hei, wid) || grid[(i + dx) * wid + j + dy] != grid[i * wid + j]) {
                    maybe_coinsext += 1;
                    if (maybe_coinsext == 1) {
                        pdx = dx;
                        pdy = dy;
                    } else if (maybe_coinsext == 2) {
                        if (dx != pdx && dy != pdy) {
                            en_face = false;
                        }
                    }
                    nb_peri += 1;
                    continue;
                }
                if (visited[(i + dx) * wid + j + dy] != 0) {
                    continue;
                }
                visited[(i + dx) * wid + j + dy] = 1;
                zone zonerec = parcours(grid, hei, wid, visited, i + dx, j + dy, repr, representants);
                nbvis += zonerec.nbvis;
                nb_peri += zonerec.nbperi;
                coinsext += zonerec.coinsext;
            }
        }
    }
    coinsext += maybe_coinsext == 4 ? 4 : (maybe_coinsext == 3 ? 2 : (maybe_coinsext == 2 && !en_face ? 1 : 0));
    return (zone) { .nbvis = nbvis, .nbperi = nb_peri, .coinsext = coinsext };
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
    int i = 0;
    while (inp[i] != '\n') {
        grid[i] = inp[i];
        i += 1;
    }
    int wid = i;
    i += 1;
    int hei = 1;
    while (i < nbread - 1) {
        for (int j = 0; j < wid; j += 1) {
            grid[wid * hei + j] = inp[i];
            i += 1;
        }
        i += 1;
        hei += 1;
    }

    int* visited = malloc(wid * hei * sizeof(int));
    int* aires = malloc(wid * hei * sizeof(int));
    int* representants = malloc(wid * hei * sizeof(int));
    int* cint = malloc(wid * hei * sizeof(int));
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            visited[i * wid + j] = 0;
            representants[i * wid + j] = 0;
            aires[i * wid + j] = 0;
            cint[i * wid + j] = 0;
        }
    }

    // ETOILES

    long ret1 = 0;
    long ret2 = 0;
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            if (visited[i * wid + j] > 0) {
                continue;
            }
            visited[i * wid + j] = 1;
            zone z = parcours(grid, hei, wid, visited, i, j, i * wid + j, representants);
            aires[i * wid + j] = z.nbvis;
            ret1 += z.nbvis * z.nbperi;
            cint[representants[i * wid + j]] = z.coinsext;
        }
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            coins_interieurs(grid, hei, wid, i, j, cint, representants);
        }
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            ret2 += aires[representants[i * wid + j]] * cint[representants[i * wid + j]];

            if (cint[representants[i * wid + j]] != 0) {
                // printf("%c %d\n", grid[i * wid + j], cint[representants[i * wid + j]]);
            }
            cint[representants[i * wid + j]] = 0;
        }
    }

    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(grid);
    free(visited);
    free(representants);
    free(aires);
    free(cint);
}
