#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define COTE 140

int main(void)
{
    // LECTURE
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        fprintf(stderr, "Erreur : impossible de lire le fichier\n");
        return 1;
    }
    char inp[MAXLEN];
    int nbread = fread(inp, sizeof(char), MAXLEN, stdin);
    if (nbread == 0) {
        return 2;
    }
    long ret1 = 0;
    long ret2 = 0;

    // P1
    for (int x = 0; x < COTE; x += 1) {
        for (int y = 0; y < COTE; y += 1) {
            for (int dx = -1; dx <= 1; dx += 1) {
                for (int dy = -1; dy <= 1; dy += 1) {
                    if (dy != 0 || dx != 0) {
                        int limx = (x + 3 * dx);
                        int limy = (y + 3 * dy);
                        if (limx >= COTE || limx < 0) {
                            continue;
                        } else if (limy >= COTE || limy < 0) {
                            continue;
                        } else {
                            if (inp[x * COTE + y + x] == 'X' && inp[(x + dx) * COTE + x + dx + y + dy] == 'M' && inp[(x + 2 * dx) * COTE + y + 2 * dy + x + 2 * dx] == 'A' && inp[limx * COTE + limy + limx] == 'S') {
                                ret1 += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    // P2
    for (int x = 0; x < COTE; x += 1) {
        for (int y = 0; y < COTE; y += 1) {
            for (int dx = -1; dx <= 1; dx += 2) {
                for (int dy = -1; dy <= 1; dy += 2) {
                    int limx = (x + 2 * dx);
                    int limy = (y + 2 * dy);
                    if (limx >= COTE || limx < 0) {
                        continue;
                    } else if (limy >= COTE || limy < 0) {
                        continue;
                    } else {
                        if (inp[x * COTE + y + x] == 'M' && inp[(x + dx) * COTE + x + dx + y + dy] == 'A' && inp[limx * COTE + limy + limx] == 'S') {
                            if (inp[(x + 2 * dx) * COTE + y + x + 2 * dx] == 'M' && inp[x * COTE + limy + x] == 'S') {
                                ret2 += 1;
                            }
                            if (inp[x * COTE + limy + x] == 'M' && inp[(x + 2 * dx) * COTE + y + (x + 2 * dx)] == 'S') {
                                ret2 += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2 / 2);
}
