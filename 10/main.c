#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NLINES 60
#define LENLINES 60

bool isbounds(int x, int y, int hei, int wid)
{
    return x >= 0 && x < hei && y >= 0 && y < wid;
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
    int* grid = malloc(nbread * sizeof(int));
    int wid = -1;
    int i = 0;

    while (inp[i] != '\n') {
        grid[i] = inp[i] - '0';
        i += 1;
    }
    wid = i;
    int hei = 1;
    int j = 0;
    i += 1;
    while (inp[i] != 0) {
        if (inp[i] == '\n') {
            hei += 1;
            j = 0;
        } else {
            grid[hei * wid + j] = inp[i] - '0';
            j += 1;
        }
        i += 1;
    }

    // PRECALCUL
    int* positions[10];
    int lengths[10] = {};
    for (int i = 0; i < 10; i += 1) {
        positions[i] = malloc(wid * hei / 2 * sizeof(int));
    }
    for (int i = 0; i < hei * wid; i += 1) {
        positions[grid[i]][lengths[grid[i]]] = i;
        lengths[grid[i]] += 1;
    }

    // ETOILES
    long ret1 = 0;
    long ret2 = 0;
    int* endpoints = malloc(wid * hei * wid * hei * sizeof(int));
    int* nb_endpts = malloc(wid * hei * sizeof(int));
    int* nbpaths = malloc(wid * hei * sizeof(int));
    for (int p = 0; p < lengths[9]; p += 1) {
        int q = positions[9][p];
        endpoints[q * wid * hei] = q;
        nb_endpts[q] = 1;
        nbpaths[q] = 1;
    }
    for (int k = 8; k >= 0; k -= 1) {
        for (int p = 0; p < lengths[k]; p += 1) {
            int q = positions[k][p];
            int row = q / wid;
            int col = q % wid;
            nbpaths[q] = 0;
            nb_endpts[q] = 0;
            if (isbounds(row + 1, col, hei, wid) && grid[(row + 1) * wid + col] == k + 1) {
                nbpaths[q] += nbpaths[(row + 1) * wid + col];
                for (int i = 0; i < nb_endpts[(row + 1) * wid + col]; i += 1) {
                    endpoints[q * wid * hei + i] = endpoints[((row + 1) * wid + col) * wid * hei + i];
                    nb_endpts[q] += 1;
                }
            }
            if (isbounds(row - 1, col, hei, wid) && grid[(row - 1) * wid + col] == k + 1) {
                nbpaths[q] += nbpaths[(row - 1) * wid + col];
                for (int i = 0; i < nb_endpts[(row - 1) * wid + col]; i += 1) {
                    int nouv = endpoints[((row - 1) * wid + col) * wid * hei + i];
                    int j = 0;
                    for (; j < nb_endpts[q]; j += 1) {
                        if (nouv == endpoints[q * wid * hei + j]) {
                            break;
                        }
                    }
                    if (j == nb_endpts[q]) {
                        endpoints[q * wid * hei + nb_endpts[q]] = nouv;
                        nb_endpts[q] += 1;
                    }
                }
            }
            if (isbounds(row, col + 1, hei, wid) && grid[row * wid + col + 1] == k + 1) {
                nbpaths[q] += nbpaths[row * wid + col + 1];
                for (int i = 0; i < nb_endpts[row * wid + col + 1]; i += 1) {
                    int nouv = endpoints[(row * wid + col + 1) * wid * hei + i];
                    int j = 0;
                    for (; j < nb_endpts[q]; j += 1) {
                        if (nouv == endpoints[q * wid * hei + j]) {
                            break;
                        }
                    }
                    if (j == nb_endpts[q]) {
                        endpoints[q * wid * hei + nb_endpts[q]] = nouv;
                        nb_endpts[q] += 1;
                    }
                }
            }
            if (isbounds(row, col - 1, hei, wid) && grid[row * wid + col - 1] == k + 1) {
                nbpaths[q] += nbpaths[row * wid + col - 1];
                for (int i = 0; i < nb_endpts[row * wid + col - 1]; i += 1) {
                    int nouv = endpoints[(row * wid + col - 1) * wid * hei + i];
                    int j = 0;
                    for (; j < nb_endpts[q]; j += 1) {
                        if (nouv == endpoints[q * wid * hei + j]) {
                            break;
                        }
                    }
                    if (j == nb_endpts[q]) {
                        endpoints[q * wid * hei + nb_endpts[q]] = nouv;
                        nb_endpts[q] += 1;
                    }
                }
            }
        }
    }
    for (int p = 0; p < lengths[0]; p += 1) {
        int q = positions[0][p];
        ret1 += nb_endpts[q];
        ret2 += nbpaths[q];
    }

    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    for (int i = 0; i < 10; i += 1) {
        free(positions[i]);
    }
    free(endpoints);
    free(nbpaths);
    free(nb_endpts);
    free(grid);
}
