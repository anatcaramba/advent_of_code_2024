#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000

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
    char* grid = malloc(MAXLEN * sizeof(char));
    int i = 0;
    int row = 0;
    int col = 0;
    int wid = 0;
    int rowd = -1;
    int cold = -1;
    while (i < nbread - 1) {
        if (inp[i] <= '\n') {
            row += 1;
            wid = col;
            col = 0;
        } else {
            if (inp[i] == '<' || inp[i] == '>' || inp[i] == '^' || inp[i] == 'v') {
                rowd = row;
                cold = col;
            }
            grid[row * wid + col] = inp[i];
            col += 1;
        }
        i += 1;
    }
    int hei = row;

    // ETOILE 1
    long ret1 = 0;
    long ret2 = 0;
    int* visited = malloc(wid * hei * sizeof(int));
    for (int i = 0; i < hei * wid; i += 1) {
        visited[i] = 0;
    }
    int rowdin = rowd;
    int coldin = cold;
    visited[rowd * wid + cold] = 1;
    int drin = grid[rowd * wid + cold] == '>' || grid[rowd * wid + cold] == '<' ? 0 : (grid[rowd * wid + cold] == '^' ? -1 : 1);
    int dcin = grid[rowd * wid + cold] == '^' || grid[rowd * wid + cold] == 'v' ? 0 : (grid[rowd * wid + cold] == '<' ? -1 : 1);
    int dr = drin;
    int dc = dcin;
    while (0 <= rowd + dr && rowd + dr < hei && 0 <= cold + dc && cold + dc < wid) {
        if (grid[(rowd + dr) * wid + cold + dc] == '#') {
            int ndr = dr == 0 ? dc : 0;
            dc = dc == 0 ? -dr : 0;
            dr = ndr;
        }
        if (!(0 <= rowd + dr && rowd + dr < hei && 0 <= cold + dc && cold + dc < wid)) {
            break;
        }
        if (grid[(rowd + dr) * wid + cold + dc] == '#') {
            continue;
        }
        rowd += dr;
        cold += dc;
        visited[rowd * wid + cold] += 1;
    }
    for (int i = 0; i < wid * hei; i += 1) {
        if (visited[i] > 0) {
            ret1 += 1;
        }
    }
    printf("%ld\n", ret1);

    // ETOILE 2
    for (int i = 0; i < wid * hei; i += 1) {
        if (visited[i] == 0) {
            continue;
        }
        grid[i] = '#';
        int dr = drin;
        int dc = dcin;
        rowd = rowdin;
        cold = coldin;
        int nbet = 0;
        while (nbet < wid * hei / 2 && 0 <= rowd + dr && rowd + dr < hei && 0 <= cold + dc && cold + dc < wid) {
            if (grid[(rowd + dr) * wid + cold + dc] == '#') {
                int ndr = dr == 0 ? dc : 0;
                dc = dc == 0 ? -dr : 0;
                dr = ndr;
            }
            if (!(0 <= rowd + dr && rowd + dr < hei && 0 <= cold + dc && cold + dc < wid)) {
                break;
            }
            rowd += dr;
            cold += dc;
            nbet += 1;
        }
        if (nbet == wid * hei / 2) {
            ret2 += 1;
        }
        grid[i] = '.';
    }
    printf("%ld\n", ret2);
    free(grid);
    free(visited);
}
