#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NETAPES1 25
#define NETAPES2 75
#define MODULO 1000003

typedef struct maillon maillon;
struct maillon {
    maillon* suivant;
    maillon* prec;
    long val;
};

typedef struct cleval cleval;
struct cleval {
    cleval* suivant;
    int et;
    long val;
    long lg;
};

void liberer(cleval* entree)
{
    while (entree != 0) {
        cleval* suivant = entree->suivant;
        free(entree);
        entree = suivant;
    }
}

int hash(int et, long val)
{
    return (val * 100 + et) % MODULO;
}

cleval* trouver(cleval** hashtable, int et, long val)
{
    int h = hash(et, val);
    cleval* cv = hashtable[h];
    while (cv != 0) {
        if (cv->et == et && cv->val == val) {
            return cv;
        }
        cv = cv->suivant;
    }
    return 0;
}

void inserer(cleval** hashtable, int et, long val, long lg)
{
    int h = hash(et, val);
    cleval* cv = malloc(sizeof(cleval));
    cv->et = et;
    cv->val = val;
    cv->suivant = hashtable[h];
    cv->lg = lg;
    hashtable[h] = cv;
}

long nb_digits(long n)
{
    return n == 0 ? 0 : (nb_digits(n / 10) + 1);
}

long ten_pow(long m)
{
    if (m == 0) {
        return 1;
    } else {
        long half = ten_pow(m / 2);
        return m % 2 == 0 ? half * half : half * half * 10;
    }
}

long longueur(long val, int et, int netapesmax, cleval** hashtable)
{
    if (et == netapesmax) {
        return 1;
    }
    cleval* entree = trouver(hashtable, et, val);
    if (entree != 0) {
        return entree->lg;
    }
    long lg = -1;
    if (val == 0) {
        lg = longueur(1, et + 1, netapesmax, hashtable);
    } else {
        long m = nb_digits(val);
        if (m % 2 == 1) {
            lg = longueur(val * 2024, et + 1, netapesmax, hashtable);
        } else {
            long dixp = ten_pow(m / 2);
            lg = longueur(val / dixp, et + 1, netapesmax, hashtable) + longueur(val % dixp, et + 1, netapesmax, hashtable);
        }
    }
    inserer(hashtable, et, val, lg);
    return lg;
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

    int* pierres = malloc(nbread * sizeof(int));
    int i = 0;
    int len = 0;
    while (i < nbread - 1) {
        int s = 0;
        while (isdigit(inp[i])) {
            s = s * 10 + inp[i] - '0';
            i += 1;
        }
        pierres[len] = s;
        len += 1;
        i += 1;
    }

    long ret1 = 0;
    long ret2 = 0;

    // ETOILE 1
    cleval** hashtable = malloc(MODULO * sizeof(cleval));
    for (int i = 0; i < MODULO; i += 1) {
        hashtable[i] = 0;
    }
    for (int i = 0; i < len; i += 1) {
        ret1 += longueur(pierres[i], 0, NETAPES1, hashtable);
    }
    printf("%ld\n", ret1);
    for (int i = 0; i < MODULO; i += 1) {
        liberer(hashtable[i]);
        hashtable[i] = 0;
    }

    // ETOILE 2
    for (int i = 0; i < MODULO; i += 1) {
        liberer(hashtable[i]);
        hashtable[i] = 0;
    }
    for (int i = 0; i < len; i += 1) {
        ret2 += longueur(pierres[i], 0, NETAPES2, hashtable);
    }
    printf("%ld\n", ret2);
    for (int i = 0; i < MODULO; i += 1) {
        liberer(hashtable[i]);
        hashtable[i] = 0;
    }
    free(pierres);
    free(hashtable);
}
