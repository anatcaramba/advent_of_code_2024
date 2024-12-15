// #include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define MAXMV 30000

bool parcours(char* grid2, int x, int y, int dx, int dy, int wid, bool* abouger)
{
    abouger[x * wid + y] = true;
    char c = grid2[(x + dx) * wid + y + dy];
    if (c == '#') {
        return false;
    }
    if (c == '.') {
        return true;
    }
    if (c == '[') {
        if (!parcours(grid2, x + dx, y + dy, dx, dy, wid, abouger)) {
            return false;
        }
        if (!abouger[(x + dx) * wid + (y + dy + 1)] && !parcours(grid2, x + dx, y + dy + 1, dx, dy, wid, abouger)) {
            // a droite
            return false;
        }
    } else if (c == ']') {
        if (!parcours(grid2, x + dx, y + dy, dx, dy, wid, abouger)) {
            return false;
        }
        if (!abouger[(x + dx) * wid + (y + dy - 1)] && !parcours(grid2, x + dx, y + dy - 1, dx, dy, wid, abouger)) {
            // a gauche
            return false;
        }
    }
    return true;
}

void decaler(char* grid2, int dx, int dy, int hei, int wid, bool* abouger)
{
    if (dx == 0 && dy == -1) {
        for (int y = 1; y < wid - 1; y += 1) {
            for (int x = 1; x < hei - 1; x += 1) {
                if (abouger[x * wid + y]) {
                    grid2[(x + dx) * wid + (y + dy)] = grid2[x * wid + y];
                    grid2[x * wid + y] = '.';
                }
            }
        }
    } else if (dx == 0 && dy == 1) {
        for (int y = wid - 2; y > 0; y -= 1) {
            for (int x = 1; x < hei - 1; x += 1) {
                if (abouger[x * wid + y]) {
                    grid2[(x + dx) * wid + (y + dy)] = grid2[x * wid + y];
                    grid2[x * wid + y] = '.';
                }
            }
        }
    } else if (dx == -1 && dy == 0) {
        for (int x = 1; x < hei - 1; x += 1) {
            for (int y = wid - 2; y > 0; y -= 1) {
                if (abouger[x * wid + y]) {
                    grid2[(x + dx) * wid + (y + dy)] = grid2[x * wid + y];
                    grid2[x * wid + y] = '.';
                }
            }
        }
    } else {
        for (int x = hei - 2; x > 0; x -= 1) {
            for (int y = wid - 2; y > 0; y -= 1) {
                if (abouger[x * wid + y]) {
                    grid2[(x + dx) * wid + (y + dy)] = grid2[x * wid + y];
                    grid2[x * wid + y] = '.';
                }
            }
        }
    }
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
    int xrob = -1;
    int yrob = -1;
    int xrob2 = -1;
    int yrob2 = -1;
    while (inp[i] != '\n') {
        for (int j = 0; j < wid; j += 1) {
            grid[wid * hei + j] = inp[i];
            if (inp[i] == '@') {
                xrob = hei;
                yrob = j;
                xrob2 = hei;
                yrob2 = 2 * j;
            }
            i += 1;
        }
        i += 1;
        hei += 1;
    }
    i += 1;
    char* mouvements = malloc(MAXMV * sizeof(char));
    int nb_mouv = 0;
    while (i < nbread - 1) {
        if (inp[i] != '\n') {
            mouvements[nb_mouv] = inp[i];
            nb_mouv += 1;
        }
        i += 1;
    }

    char* grid2 = malloc(wid * hei * 2 * sizeof(char));
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            char c = grid[i * wid + j];
            grid2[i * 2 * wid + 2 * j] = c == 'O' ? '[' : c;
            grid2[i * 2 * wid + 2 * j + 1] = c == 'O' ? ']' : (c == '#' ? '#' : '.');
        }
    }

    long ret1 = 0;
    long ret2 = 0;

    // ETOILE1

    for (int j = 0; j < nb_mouv; j += 1) {
        char mv = mouvements[j];
        int dx = mv == '<' || mv == '>' ? 0 : (mv == 'v' ? 1 : -1);
        int dy = mv == 'v' || mv == '^' ? 0 : (mv == '>' ? 1 : -1);
        int x = xrob;
        int y = yrob;
        while (grid[(x + dx) * wid + y + dy] == 'O') {
            x += dx;
            y += dy;
        }
        if (grid[(x + dx) * wid + y + dy] != '#') {
            while (x + dx != xrob || y + dy != yrob) {
                grid[(x + dx) * wid + y + dy] = grid[x * wid + y];
                x -= dx;
                y -= dy;
            }
            grid[xrob * wid + yrob] = '.';
            xrob += dx;
            yrob += dy;
        }
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < wid; j += 1) {
            if (grid[i * wid + j] == 'O') {
                ret1 += 100 * i + j;
            }
        }
    }
    printf("%ld\n", ret1);

    // ETOILE 2

    bool* abouger = malloc(2 * wid * hei * sizeof(bool));
    for (int j = 0; j < nb_mouv; j += 1) {
        for (int i = 0; i < hei; i += 1) {
            for (int j = 0; j < 2 * wid; j += 1) {
                abouger[i * 2 * wid + j] = false;
            }
        }
        char mv = mouvements[j];
        int dx = mv == '<' || mv == '>' ? 0 : (mv == 'v' ? 1 : -1);
        int dy = mv == 'v' || mv == '^' ? 0 : (mv == '>' ? 1 : -1);
        bool bouger = parcours(grid2, xrob2, yrob2, dx, dy, 2 * wid, abouger);
        if (!bouger) {
            continue;
        }
        decaler(grid2, dx, dy, hei, 2 * wid, abouger);
        xrob2 += dx;
        yrob2 += dy;
    }
    for (int i = 0; i < hei; i += 1) {
        for (int j = 0; j < 2 * wid; j += 1) {
            if (grid2[i * 2 * wid + j] == '[') {
                ret2 += 100 * i + j;
            }
        }
    }

    printf("%ld\n", ret2);
    free(grid);
    free(mouvements);
    free(abouger);
    free(grid2);
}
