#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NLINES 1000
#define LENLINES 20

int main(void)
{
    // LECTURE, PARSING
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    char inp[MAXLEN] = {};
    int nbread = fread(inp, sizeof(char), MAXLEN, stdin);
    if (nbread == 0) {
        fprintf(stderr, "Erreur : fichier non lu\n");
        return 2;
    }
    long* equations = malloc(NLINES * LENLINES * sizeof(long));
    int* lengths = malloc(NLINES * sizeof(int));
    int row = 0;
    int i = 0;
    while (i + 1 < nbread) {
        while (!isdigit(inp[i])) {
            i += 1;
        }
        long v = 0;
        while (inp[i] != ':') {
            v = 10 * v + inp[i] - '0';
            i += 1;
        }
        equations[row * LENLINES] = v;
        i += 1;
        int col = 1;
        while (inp[i] == ' ') {
            i += 1;
            long v = 0;
            while (isdigit(inp[i])) {
                v = v * 10 + inp[i] - '0';
                i += 1;
            }
            equations[row * LENLINES + col] = v;
            col += 1;
        }
        lengths[row] = col;
        row += 1;
    }
    int nbrows = row;
    long ret1 = 0;
    long ret2 = 0;

    // ETOILE 1
    for (int r = 0; r < nbrows; r += 1) {
        for (long k = 0; k < (1 << (lengths[r] - 2)); k += 1) {
            long s = equations[r * LENLINES + 1];
            for (int c = 2; c < lengths[r]; c += 1) {
                if ((k & 1 << (c - 2)) == 0) {
                    s += equations[r * LENLINES + c];
                } else {
                    s *= equations[r * LENLINES + c];
                }
            }
            if (s == equations[r * LENLINES]) {
                ret1 += s;
                break;
            }
        }
    }

    // ETOILE 2
    for (int r = 0; r < nbrows; r += 1) {
        for (long k = 0; k < pow(3, (lengths[r] - 2)); k += 1) {
            long s = equations[r * LENLINES + 1];
            long k0 = k;
            for (int c = 2; c < lengths[r]; c += 1) {
                long q = k0 % 3;
                if (q == 0) {
                    s += equations[r * LENLINES + c];

                } else if (q == 1) {
                    s *= equations[r * LENLINES + c];
                } else {
                    int m = equations[r * LENLINES + c];
                    while (m > 0) {
                        s *= 10;
                        m /= 10;
                    }
                    s += equations[r * LENLINES + c];
                }
                k0 /= 3;
            }

            if (s == equations[r * LENLINES]) {
                ret2 += s;
                break;
            }
        }
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(equations);
    free(lengths);
}
