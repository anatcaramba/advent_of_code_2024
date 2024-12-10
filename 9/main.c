#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

long checksum(long d, long le, int id)
{
    return id * (d + d + le - 1) * le / 2;
}

void extraire_min(long* tas, int n)
{
    tas[0] = tas[n - 1];
    int i = 0;
    while (2 * i + 1 < n) {
        int fils = 2 * i + 1;
        if (2 * i + 2 < n && tas[2 * i + 2] < tas[2 * i + 1]) {
            fils = 2 * i + 2;
        }
        if (tas[i] > tas[fils]) {
            long temp = tas[i];
            tas[i] = tas[fils];
            tas[fils] = temp;
            i = fils;
        } else {
            break;
        }
    }
}

void ajouter(long* tas, int n, long deb)
{
    tas[n] = deb;
    int i = n;
    while (i >= 1 && tas[(i - 1) / 2] > tas[i]) {
        int pere = (i - 1) / 2;
        long temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        i = pere;
    }
}

int main(void)
{
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
    int len = nbread - 1;
    long ret1 = 0;
    long ret2 = 0;

    // ETOILE 1
    int j = len - 1;
    long curr = inp[j] - '0';
    long d = 0;
    int i = 0;
    long le = inp[i] - '0';
    while (i < j) {
        if (i % 2 == 0) {
            ret1 += checksum(d, le, i / 2);
            d += le;
            i += 1;
            le = inp[i] - '0';
        } else if (le - curr > 0) {
            ret1 += checksum(d, curr, j / 2);
            d += curr;
            le -= curr;
            j -= 2;
            curr = inp[j] - '0';
        } else if (le - curr < 0) {
            ret1 += checksum(d, le, j / 2);
            i += 1;
            d += le;
            curr -= le;
            le = inp[i] - '0';
        } else {
            ret1 += checksum(d, le, j / 2);
            i += 1;
            j -= 2;
            d += le;
            curr = inp[j] - '0';
            le = inp[i] - '0';
        }
    }
    if (i == j) {
        ret1 += checksum(d, curr, j / 2);
    }
    printf("%ld\n", ret1);

    // ETOILE 2
    long* positions = malloc((len / 2 + 1) * sizeof(long));
    long** trous = malloc(10 * sizeof(long*));
    for (int k = 0; k < 10; k += 1) {
        trous[k] = malloc(len * sizeof(long));
    }
    long lengths[10] = {};
    d = 0;
    for (int i = 0; i < len; i += 1) {
        positions[i / 2] = d;
        d += inp[i] - '0';
        i += 1;
        if (i == len) {
            break;
        }
        int lg = inp[i] - '0';
        ajouter(trous[lg], lengths[lg], d);
        lengths[lg] += 1;
        d += lg;
    }
    for (int j = len - 1; j >= 0; j -= 2) {
        long mininser = LONG_MAX;
        long posmin = -1;
        for (int k = inp[j] - '0'; k < 10; k += 1)
            if (lengths[k] > 0 && trous[k][0] < mininser && trous[k][0] < positions[j / 2]) {
                mininser = trous[k][0];
                posmin = k;
            }
        if (posmin >= 0) {
            extraire_min(trous[posmin], lengths[posmin]);
            lengths[posmin] -= 1;
            positions[j / 2] = mininser;
            int reste = posmin - (inp[j] - '0');
            if (reste > 0) {
                ajouter(trous[reste], lengths[reste], mininser + inp[j] - '0');
                lengths[reste] += 1;
            }
        }
        ret2 += checksum(positions[j / 2], inp[j] - '0', j / 2);
    }
    printf("%ld\n", ret2);
    for (int k = 0; k < 10; k += 1) {
        free(trous[k]);
    }
    free(trous);
    free(positions);
}
