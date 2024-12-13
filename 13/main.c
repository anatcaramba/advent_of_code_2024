#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NBMAX 2000
#define LENTAS 10000000
#define OFFSET 10000000000000

typedef struct t_machine t_machine;
struct t_machine {
    long xprize;
    long yprize;
    long dxa;
    long dya;
    long dxb;
    long dyb;
};

int lecture_entree(t_machine* machines, char* inp, int nbread)
{
    int nb_machines = 0;
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
        machines[nb_machines].dxa = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        machines[nb_machines].dya = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        machines[nb_machines].dxb = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        machines[nb_machines].dyb = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        machines[nb_machines].xprize = s;
        while (!isdigit(inp[i])) {
            i += 1;
        }
        s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        machines[nb_machines].yprize = s;
        nb_machines += 1;
    }
    return nb_machines;
}

typedef struct t_sommet t_sommet;
struct t_sommet {
    long x;
    long y;
    long prize;
};

long hash(long x, long y, long xprize)
{
    return (x + y * (xprize + 1));
}
void maj_positions(t_sommet* tas, int i, int* positions, long xprize)
{
    positions[hash(tas[i].x, tas[i].y, xprize)] = i;
}

t_sommet extraire_min(t_sommet* tas, int n, int* positions, long xprize)
{
    t_sommet retour = tas[0];
    tas[0] = tas[n - 1];
    maj_positions(tas, 0, positions, xprize);
    int i = 0;
    while (2 * i + 1 < n) {
        int fils = 2 * i + 1;
        if (2 * i + 2 < n && tas[2 * i + 2].prize < tas[2 * i + 1].prize) {
            fils = 2 * i + 2;
        }
        if (tas[i].prize > tas[fils].prize) {
            t_sommet temp = tas[i];
            tas[i] = tas[fils];
            tas[fils] = temp;
            maj_positions(tas, i, positions, xprize);
            maj_positions(tas, fils, positions, xprize);
            i = fils;
        } else {
            break;
        }
    }
    return retour;
}

void diminuer(t_sommet* tas, long x, long y, long prize, long xprize, int* positions)
{
    int i = positions[hash(x, y, xprize)];
    if (tas[i].prize <= prize) {
        return;
    }
    tas[i].prize = prize;
    while (i >= 1 && tas[(i - 1) / 2].prize > tas[i].prize) {
        int pere = (i - 1) / 2;
        t_sommet temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions, xprize);
        maj_positions(tas, pere, positions, xprize);
        i = pere;
    }
}
void ajouter(t_sommet* tas, int n, t_sommet deb, int* positions, long xprize)
{
    tas[n] = deb;
    maj_positions(tas, n, positions, xprize);
    int i = n;
    while (i >= 1 && tas[(i - 1) / 2].prize > tas[i].prize) {
        int pere = (i - 1) / 2;
        t_sommet temp = tas[i];
        tas[i] = tas[pere];
        tas[pere] = temp;
        maj_positions(tas, i, positions, xprize);
        maj_positions(tas, pere, positions, xprize);
        i = pere;
    }
}

long dijkstra(t_machine mach)
{
    long xprize = mach.xprize;
    long yprize = mach.yprize;
    long ret = LONG_MAX;
    t_sommet* tas = malloc(LENTAS * sizeof(t_sommet));
    long lentabs = (xprize + 1) * (yprize + 1);
    int* positions = malloc(lentabs * sizeof(int));
    int* visites = malloc(lentabs * sizeof(int));
    for (int i = 0; i < lentabs; i += 1) {
        visites[i] = 0;
    }
    tas[0] = (t_sommet) { .prize = 0, .x = 0, .y = 0 };
    maj_positions(tas, 0, positions, xprize);
    visites[hash(0, 0, xprize)] = 1;
    int lentas = 1;
    while (lentas > 0) {
        t_sommet sommet = extraire_min(tas, lentas, positions, xprize);
        lentas -= 1;
        long x = sommet.x;
        long y = sommet.y;
        long prize = sommet.prize;
        if (x == xprize && y == yprize) {
            ret = prize;
            goto fin;
        }
        if (x != 0 && x * yprize == y * xprize) { // shortcut
            long ratio = xprize / x;
            ret = prize * ratio;
            goto fin;
        }
        long xa = x + mach.dxa;
        long ya = y + mach.dya;
        long xb = x + mach.dxb;
        long yb = y + mach.dyb;
        if (xa <= xprize % OFFSET && ya <= yprize % OFFSET) {
            if (visites[hash(xa, ya, xprize)] == 0) {
                visites[hash(xa, ya, xprize)] = 1;
                t_sommet suivant = (t_sommet) { .prize = prize + 3, .x = xa, .y = ya };
                ajouter(tas, lentas, suivant, positions, xprize);
                lentas += 1;
            } else {
                diminuer(tas, xa, ya, prize + 3, xprize, positions);
            }
        }
        if (xb <= xprize % OFFSET && yb <= yprize % OFFSET) {
            if (visites[hash(xb, yb, xprize)] == 0) {
                visites[hash(xb, yb, xprize)] = 1;
                t_sommet suivant = (t_sommet) {
                    .prize = prize + 1,
                    .x = xb,
                    .y = yb,
                };
                ajouter(tas, lentas, suivant, positions, xprize);
                lentas += 1;
            } else {
                diminuer(tas, xb, yb, prize + 1, xprize, positions);
            }
        }
    }
fin:
    free(tas);
    free(positions);
    free(visites);
    return ret;
}

long reso(t_machine mach)
{
    long dxa = mach.dxa;
    long dya = mach.dya;
    long dxb = mach.dxb;
    long dyb = mach.dyb;
    long xprize = mach.xprize;
    long yprize = mach.yprize;
    if (dxa * dyb == dxb * dya) {
        // Ne se produit apparemment jamais
        return 0;
    }
    long nb = (dxa * yprize - dya * xprize) / (dxa * dyb - dya * dxb);
    long na = (xprize - nb * dxb) / dxa;
    if (na * dxa + nb * dxb == xprize && na * dya + nb * dyb == yprize) {
        return 3 * na + nb;
    } else {
        return 0;
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

    t_machine* machines = malloc(sizeof(t_machine) * NBMAX);
    int nb_machines = lecture_entree(machines, inp, nbread);

    // ETOILES

    long ret1 = 0;
    long ret2 = 0;

    for (int i = 0; i < nb_machines; i += 1) {
        t_machine mach = machines[i];
        // long m = dijkstra(mach);
        // if (m == LONG_MAX) {
        //     continue;
        // }
        // printf("m: %ld\n", m);
        // ret1 += m;
        ret1 += reso(mach);
    }
    printf("%ld\n", ret1);
    for (int i = 0; i < nb_machines; i += 1) {
        t_machine mach = machines[i];
        mach.xprize += OFFSET;
        mach.yprize += OFFSET;
        ret2 += reso(mach);
    }
    printf("%ld\n", ret2);
    free(machines);
}
