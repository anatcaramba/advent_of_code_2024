#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXNB 3000
#define MODULO 16777216
#define NETAPES 2000

int hash(int c1, int c2, int c3, int c4)
{
    return (((c1 + 9) * 19 + (c2 + 9)) * 19 + (c3 + 9)) * 19 + (c4 + 9);
}

long etape(long a)
{
    a = ((a * 64) ^ a) % MODULO;
    a = ((a / 32) ^ a) % MODULO;
    a = ((a * 2048) ^ a) % MODULO;
    return a;
}

int main(void)
{
    // LECTURE
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        fprintf(stderr, "Erreur : impossible de lire le fichier\n");
        return 1;
    }

    int nb = -1;
    long* nombres = malloc(MAXNB * sizeof(long));
    int ret = 1;
    while (ret == 1) {
        nb += 1;
        ret = fscanf(f, "%ld", nombres + nb);
    }

    // ETOILES
    long ret1 = 0;
    int nbetats = 19 * 19 * 19 * 19;
    bool* vues = malloc(sizeof(bool) * nbetats * nb);
    for (int i = 0; i < nbetats * nb; i += 1) {
        vues[i] = false;
    }
    long* prix = malloc(sizeof(long) * nbetats);
    for (int i = 0; i < nbetats; i += 1) {
        prix[i] = 0;
    }

    for (int i = 0; i < nb; i += 1) {
        int b4 = -10;
        int b3 = -10;
        int b2 = -10;
        int b1 = -10;
        long a = nombres[i];
        for (int j = 0; j < NETAPES; j += 1) {
            long a_new = etape(a);
            b4 = b3;
            b3 = b2;
            b2 = b1;
            b1 = a_new % 10 - a % 10;
            if (b4 != -10) {
                int h = hash(b1, b2, b3, b4);
                if (!vues[h * nb + i]) {
                    vues[h * nb + i] = true;
                    prix[h] += a_new % 10;
                }
            }
            a = a_new;
        }
        ret1 += a;
    }
    printf("%ld\n", ret1);

    long ret2 = 0;
    for (int i = 0; i < nbetats; i += 1) {
        if (ret2 < prix[i]) {
            ret2 = prix[i];
        }
    }
    printf("%ld\n", ret2);
    free(prix);
    free(vues);
    free(nombres);
}
