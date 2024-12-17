#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 100000
#define MAXLENPROG 30

enum opcode {
    adv,
    bxl,
    bst,
    jnz,
    bxc,
    out,
    bdv,
    cdv
};

enum literal {
    zero,
    un,
    deux,
    trois,
    reA,
    reB,
    reC
};

char* execution(unsigned long* programme, unsigned long lenprog, unsigned long regA, unsigned long regB, unsigned long regC)
{
    char* sortie = malloc(2 * MAXLENPROG * sizeof(char));
    for (int i = 0; i < 2 * MAXLENPROG; i += 1) {
        sortie[i] = 9;
    }
    int lenout = 0;
    unsigned long pc = 0;
    while (pc < lenprog) {
        unsigned long opc = programme[pc];
        if (opc != jnz || regA != 0) {
            if (pc + 1 >= lenprog) {
                break;
            }
            unsigned long oper = programme[pc + 1];
            unsigned long combo = oper;
            if (oper == reA) {
                combo = regA;
            } else if (oper == reB) {
                combo = regB;
            } else if (oper == reC) {
                combo = regC;
            }
            switch (opc) {
            case adv:
                regA = regA / (1 << combo);
                break;
            case bdv:
                regB = regA / (1 << combo);
                break;
            case cdv:
                regC = regA / (1 << combo);
                break;
            case bxl:
                regB = regB ^ oper;
                break;
            case bxc:
                regB = regB ^ regC;
                break;
            case bst:
                regB = combo % 8;
                break;
            case out:
                sortie[lenout] = combo % 8 + '0';
                sortie[lenout + 1] = ',';
                lenout += 2;
                break;
            case jnz:
                pc = oper;
                break;
            }
            if (opc != jnz) {
                pc += 2;
            }
        } else {
            pc += 2;
        }
    }
    sortie[lenout - 1] = 0;
    return sortie;
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
    unsigned long regA = 0;
    unsigned long regB = 0;
    unsigned long regC = 0;
    int i = 0;
    while (!isdigit(inp[i])) {
        i += 1;
    }
    while (isdigit(inp[i])) {
        regA = regA * 10 + inp[i] - '0';
        i += 1;
    }
    while (!isdigit(inp[i])) {
        i += 1;
    }
    while (isdigit(inp[i])) {
        regB = regB * 10 + inp[i] - '0';
        i += 1;
    }
    while (!isdigit(inp[i])) {
        i += 1;
    }
    while (isdigit(inp[i])) {
        regC = regC * 10 + inp[i] - '0';
        i += 1;
    }
    while (!isdigit(inp[i])) {
        i += 1;
    }
    unsigned long* programme = malloc(MAXLENPROG * sizeof(unsigned long));
    int lenprog = 0;
    while (i < nbread - 1) {
        programme[lenprog] = inp[i] - '0';
        lenprog += 1;
        i += 2;
    }

    // ETOILE 1
    char* ret1 = execution(programme, lenprog, regA, regB, regC);
    printf("%s\n", ret1);
    free(ret1);

    // ETOILE 2
    unsigned long ret2 = 0;
    for (int i = lenprog - 1; i >= 0; i -= 1) {
        unsigned amod = 0;
        for (;; amod += 1) {
            char* sortie = execution(programme, lenprog, ret2 * 8 + amod, regB, regC);
            int j = 0;
            for (; j < lenprog - i; j += 1) {
                unsigned long c = sortie[2 * j] - '0';
                if (c != programme[i + j]) {
                    break;
                }
            }
            free(sortie);
            if (j != lenprog - i) {
                continue;
            }
            ret2 = ret2 * 8 + amod;
            break;
        }
    }
    printf("%ld\n", ret2);
    free(programme);
}
