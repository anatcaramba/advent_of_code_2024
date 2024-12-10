#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

int main(void)
{
    // LIRE
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        fprintf(stderr, "Erreur ouverture fichier\n");
        return 0;
    }
    char inp[MAXLEN];
    int nbread = fread(inp, sizeof(char), MAXLEN, stdin);
    if (nbread == 0) {
        fprintf(stderr, "Erreur lecture fichier\n");
        return 1;
    }

    // ETOILE 1
    int i = 0;
    long ret1 = 0;
    while (i < nbread - 1) {
        while (i < nbread - 1 && inp[i] != 'm') {
            i += 1;
        }
        if (i + 7 >= nbread - 1) { // sinon pas la place pour une instruction
            break;
        }
        if (inp[i] == 'm') {
            i += 1;
            if (inp[i] != 'u' || inp[i + 1] != 'l' || inp[i + 2] != '(' || !isdigit(inp[i + 3])) {
                i += 1;
                continue;
            }
            int s = 0;
            i += 3;
            while (isdigit(inp[i])) {
                s = 10 * s + inp[i] - '0';
                i += 1;
            }
            if (inp[i] != ',') {
                continue;
            }
            i += 1;
            if (!isdigit(inp[i])) {
                i += 1;
                continue;
            }
            int p = 0;
            while (isdigit(inp[i])) {
                p = 10 * p + inp[i] - '0';
                i += 1;
            }
            if (inp[i] != ')') {
                i += 1;
                continue;
            }
            i += 1;
            ret1 += s * p;
        }
    }

    // ETOILE 2
    long ret2 = 0;
    bool does = true;
    i = 0;
    while (i < nbread - 1) {
        while (i < nbread - 1 && inp[i] != 'm' && inp[i] != 'd') {
            i += 1;
        }
        if (i + 7 >= nbread - 1) {
            break;
        }
        if (inp[i] == 'm') {
            i += 1;
            if (!does) {
                continue;
            }
            if (inp[i] != 'u' || inp[i + 1] != 'l' || inp[i + 2] != '(' || !isdigit(inp[i + 3])) {
                i += 1;
                continue;
            }
            int s = 0;
            i += 3;
            while (isdigit(inp[i])) {
                s = 10 * s + inp[i] - '0';
                i += 1;
            }
            if (inp[i] != ',') {
                continue;
            }
            i += 1;
            if (!isdigit(inp[i])) {
                i += 1;
                continue;
            }
            int p = 0;
            while (isdigit(inp[i])) {
                p = 10 * p + inp[i] - '0';
                i += 1;
            }
            if (inp[i] != ')') {
                i += 1;
                continue;
            }
            i += 1;
            ret2 += s * p;
        } else if (inp[i] == 'd' && inp[i + 2] == '(') {
            i += 1;
            if (inp[i] != 'o' || inp[i + 2] != ')') {
                continue;
            }
            does = true;
        } else if (inp[i] == 'd') {
            i += 1;
            if (inp[i] != 'o' || inp[i + 1] != 'n' || inp[i + 2] != '\'' || inp[i + 3] != 't' || inp[i + 4] != '(' || inp[i + 5] != ')') {
                continue;
            }
            does = false;
        }
    }

    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
}
