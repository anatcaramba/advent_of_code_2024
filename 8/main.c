#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NLINES 50
#define LENLINES 50
#define NBCHAR 128
#define MAXCHAR 30

struct pair {
    int x;
    int y;
};

/*v2 - v1 */
struct pair vect(struct pair v1, struct pair v2)
{
    return (struct pair) { .x = v2.x - v1.x, .y = v2.y - v1.y };
}

/* v1 + v2*/
struct pair add(struct pair v1, struct pair v2)
{
    return (struct pair) { .x = v2.x + v1.x, .y = v2.y + v1.y };
}

bool isbounds(struct pair v, int hei, int wid)
{
    int i = v.x;
    int j = v.y;
    return i >= 0 && i < hei && j >= 0 && j < wid;
}
int main(void)
{
    // LECTURE ET PRETRAITEMENT
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
    char* grid = malloc(NLINES * LENLINES * sizeof(char));
    struct pair positions[NBCHAR][MAXCHAR] = {};
    int lengths[NBCHAR] = {};
    int i = 0;
    while (inp[i] != '\n') {
        grid[i] = inp[i];
        if (isalnum(inp[i])) {
            int cha = inp[i];
            positions[cha][lengths[cha]] = (struct pair) { .x = 0, .y = i };
            lengths[cha] += 1;
        }
        i += 1;
    }
    int wid = i;
    int r = 1;
    while (i + 1 < nbread) {
        i += 1;
        for (int col = 0; col < wid; col += 1) {
            grid[r * wid + col] = inp[i];
            if (isalnum(inp[i])) {
                int cha = inp[i];
                positions[cha][lengths[cha]] = (struct pair) { .x = r, .y = col };
                lengths[cha] += 1;
            }
            i += 1;
        }
        r += 1;
    }
    int hei = r;

    int* nbfound = malloc(sizeof(int) * wid * hei);
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            nbfound[i * wid + j] = 0;
        }
    }

    long ret1 = 0;
    long ret2 = 0;

    // ETOILE 1
    for (int i = 0; i < NBCHAR; i += 1) {
        if (isalnum(i)) {
            int cha = i;
            for (int j = 0; j < lengths[cha]; j += 1) {
                for (int k = 0; k < lengths[cha]; k += 1) {
                    if (k == j) {
                        continue;
                    }
                    struct pair v1 = positions[cha][j];
                    struct pair v2 = positions[cha][k];
                    struct pair wplus = add(vect(v1, v2), v2);
                    struct pair wmoins = vect(vect(v1, v2), v1);
                    if (isbounds(wplus, hei, wid)) {
                        nbfound[wplus.x * wid + wplus.y] += 1;
                    }
                    if (isbounds(wmoins, hei, wid)) {
                        nbfound[wmoins.x * wid + wmoins.y] += 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            if (nbfound[i * wid + j] > 0) {
                ret1 += 1;
            }
        }
    }

    // ETOILE 2
    for (int i = 0; i < NBCHAR; i += 1) {
        if (isalnum(i)) {
            int cha = i;
            for (int j = 0; j < lengths[cha]; j += 1) {
                for (int k = 0; k < lengths[cha]; k += 1) {
                    if (k == j) {
                        continue;
                    }
                    struct pair v1 = positions[cha][j];
                    struct pair v2 = positions[cha][k];
                    struct pair v12 = vect(v1, v2);
                    struct pair wplus = v2;
                    struct pair wmoins = v1;
                    while (isbounds(wplus, hei, wid)) {
                        nbfound[wplus.x * wid + wplus.y] += 1;
                        wplus = add(v12, wplus);
                    }
                    while (isbounds(wmoins, hei, wid)) {
                        nbfound[wmoins.x * wid + wmoins.y] += 1;
                        wmoins = vect(v12, wmoins);
                    }
                }
            }
        }
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            if (nbfound[i * wid + j] > 0) {
                ret2 += 1;
            }
        }
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(grid);
    free(nbfound);
}
