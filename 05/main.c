#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

int main(void)
{
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

    int* orderl = malloc(sizeof(int) * 1200);
    int* orderr = malloc(sizeof(int) * 1200);
    int j = 0;
    while (inp[j * 6] != '\n') {
        orderl[j] = (inp[j * 6] - '0') * 10 + inp[j * 6 + 1] - '0';
        orderr[j] = (inp[j * 6 + 3] - '0') * 10 + inp[j * 6 + 4] - '0';
        j += 1;
    }
    int nbpairs = j;
    int* updates = malloc(sizeof(int) * 10000);
    int i = j * 6 + 1;
    j = 0;
    while (inp[i] != 'f') {
        int s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        updates[j] = s;
        if (inp[i] != ',') {
            j += 1;
            updates[j] = -1;
        }
        j += 1;
        i += 1;
    }
    updates[j] = -2;

    long ret1 = 0;
    long ret2 = 0;

    int mustbebel[100][100] = {};
    int nmbb[100] = {};
    for (int j = 0; j < nbpairs; j += 1) {
        mustbebel[orderl[j]][nmbb[orderl[j]]] = orderr[j];
        nmbb[orderl[j]] += 1;
    }
    i = 0;
    bool bon = true;
    int b = 0;
    int a = 0;
    int bloublou[50];
    for (; updates[i] != -1; i += 1) {
        bloublou[i - a] = updates[i];
        b += 1;
        for (int k = a + 1; k < i; k += 1) {
            for (int j = 0; j < nmbb[updates[i]]; j += 1) {
                if (updates[k] == mustbebel[updates[i]][j]) {
                    bon = false;
                    break;
                }
            }
            if (!bon) {
                break;
            }
        }
        if (updates[i + 1] != -1) {
            continue;
        }
        if (bon) {
            // Etoile 1
            ret1 += updates[(a + b) / 2];
        } else {
            // Etoile 2
            // On trouve la médiane en temps quadratique (les listes sont de petite taille)
            for (int p = 0; p < b - a; p += 1) {
                int c = 0;
                for (int q = 0; q < b - a; q += 1) {
                    for (int j = 0; j < nmbb[bloublou[p]]; j += 1) {
                        if (mustbebel[bloublou[p]][j] == bloublou[q]) {
                            c += 1;
                            break;
                        }
                    }
                }
                if (c == (b - a) / 2) {
                    ret2 += bloublou[p];
                }
            }
        }
        if (updates[i + 2] == -2) {
            break;
        }
        i += 1;
        b = i + 1;
        a = i + 1;
        bon = true;
    }

    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(orderl);
    free(orderr);
    free(updates);
}
