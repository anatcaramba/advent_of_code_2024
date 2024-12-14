#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLEN 10000
#define WID 101
#define HEI 103
#define NBMAX 500

typedef struct t_robot t_robot;
struct t_robot {
    long x;
    long y;
    long vx;
    long vy;
};

int lecture_entree(t_robot* robots, char* inp, int nbread)
{
    long nb_robots = 0;
    int i = 0;
    while (i < nbread - 1) {
        while (i < nbread - 1 && !isdigit(inp[i])) {
            i += 1;
        }
        int s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        robots[nb_robots].x = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        robots[nb_robots].y = s;
        while (inp[i] != '-' && !isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        int neg = 1;
        if (inp[i] == '-') {
            neg = -1;
            i += 1;
        }
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        robots[nb_robots].vx = neg * s;
        while (inp[i] != '-' && !isdigit(inp[i])) {
            i += 1;
        }
        neg = 1;
        if (inp[i] == '-') {
            neg = -1;
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        robots[nb_robots].vy = neg * s;
        while (i < nbread - 1 && !isdigit(inp[i])) {
            i += 1;
        }
        nb_robots += 1;
    }
    return nb_robots;
}

long reso(t_robot rob, long nb_et)
{
    long x = rob.x;
    long y = rob.y;
    long vx = rob.vx;
    long vy = rob.vy;
    long fx = (x + vx * nb_et) % WID;
    long fy = (y + vy * nb_et) % HEI;
    if (fx < 0) {
        fx += WID;
    }
    if (fy < 0) {
        fy += HEI;
    }
    return fx * HEI + fy;
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

    t_robot* robots = malloc(NBMAX * sizeof(t_robot));
    int nb_robots = lecture_entree(robots, inp, nbread);

    // ETOILES

    long ret1 = 0;
    long ret2 = 0;

    long nb_et1 = 100;
    long qhg = 0;
    long qhd = 0;
    long qbg = 0;
    long qbd = 0;
    for (int i = 0; i < nb_robots; i += 1) {
        t_robot rob = robots[i];
        long xyf = reso(rob, nb_et1);
        long xf = xyf / HEI;
        long yf = xyf % HEI;
        if (xf < WID / 2 && yf < HEI / 2) {
            qhg += 1;
        }
        if (xf > WID / 2 && yf < HEI / 2) {
            qhd += 1;
        }
        if (xf < WID / 2 && yf > HEI / 2) {
            qbg += 1;
        }
        if (xf > WID / 2 && yf > HEI / 2) {
            qbd += 1;
        }
    }
    ret1 = qhg * qbd * qhd * qbg;

    // ETOILE 2
    char pos[HEI][WID];
    for (int step = 89; step <= 10011; step += HEI) {
        if ((step - 112) % WID != 0) {
            continue;
        }
        for (int y = 0; y < HEI; y++) {
            for (int x = 0; x < WID; x++) {
                pos[y][x] = ' ';
            }
        }
        for (int i = 0; i < nb_robots; i += 1) {
            t_robot rob = robots[i];
            long xyf = reso(rob, step);
            long xf = xyf / HEI;
            long yf = xyf % HEI;
            pos[yf][xf] = '#';
        }
        for (int y = HEI / 3; y < 5 * HEI / 7; y++) {
            for (int x = 2 * WID / 5; x < 5 * WID / 6; x++) {
                printf("%c", pos[y][x]);
            }
            printf("\n");
        }
        printf("\n\n");
        ret2 = step;
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(robots);
}
