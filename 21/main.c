#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 100
#define NBMV 15

typedef enum t_position t_position;
enum t_position {
    zero,
    un,
    deux,
    trois,
    quatre,
    cinq,
    six,
    sept,
    huit,
    neuf,
    gauche,
    droite,
    haut,
    bas,
    valid
};

t_position inverse(t_position pos)
{
    if (pos == haut) {
        return bas;
    }
    if (pos == bas) {
        return haut;
    }
    if (pos == gauche) {
        return droite;
    }
    return gauche;
}

typedef struct t_suitemouv t_suitemouv;
struct t_suitemouv {
    int nb;
    int nbmvs;
    t_position** suitemouv;
};

t_suitemouv mouvements(t_position pos1, t_position pos2)
{
    t_suitemouv allsuitemouv = {};
    t_position** tabmouvs = malloc(2 * sizeof(t_position*));
    t_position* suitemouv = malloc(5 * sizeof(t_position));
    t_position* suitemouv_bis = malloc(5 * sizeof(t_position));
    int nb = 1;
    int nbmvs = 0;
    tabmouvs[0] = suitemouv;
    tabmouvs[1] = suitemouv_bis;
    allsuitemouv.suitemouv = tabmouvs;
    if (pos1 != zero && pos1 <= neuf) {
        if (pos2 == valid) {
            if (pos1 % 3 == 1) {
                nbmvs += 2;
                suitemouv[0] = droite;
                suitemouv[1] = droite;
                for (unsigned int i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    nbmvs += 1;
                }
            } else if (pos1 % 3 == 2) {
                nbmvs += 1;
                suitemouv[0] = droite;
                nb += 1;
                for (unsigned int i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    suitemouv_bis[nbmvs - 1] = bas;
                    nbmvs += 1;
                }
                suitemouv_bis[nbmvs - 1] = droite;
            } else {
                for (unsigned int i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    nbmvs += 1;
                }
            }
        } else if (pos2 == zero) {
            if (pos1 % 3 == 1) {
                suitemouv[0] = droite;
                nbmvs = 1;
                for (unsigned i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    nbmvs += 1;
                }
            } else if (pos1 % 3 == 0) {
                suitemouv[0] = gauche;
                nbmvs = 1;
                for (unsigned i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    suitemouv_bis[nbmvs - 1] = bas;
                    nbmvs += 1;
                }
                suitemouv_bis[nbmvs - 1] = gauche;
                nb += 1;
            } else {
                for (unsigned i = 0; i < (pos1 + 2) / 3; i += 1) {
                    suitemouv[nbmvs] = bas;
                    nbmvs += 1;
                }
            }
        } else {
            for (unsigned i = (pos1 + 2) % 3; i < (pos2 + 2) % 3; i += 1) {
                suitemouv[nbmvs] = droite;
                nbmvs += 1;
            }
            for (unsigned i = (pos2 + 2) % 3; i < (pos1 + 2) % 3; i += 1) {
                suitemouv[nbmvs] = gauche;
                nbmvs += 1;
            }
            for (unsigned j = (pos2 + 2) / 3; j < (pos1 + 2) / 3; j += 1) {
                suitemouv[nbmvs] = bas;
                nbmvs += 1;
            }
            for (unsigned j = (pos1 + 2) / 3; j < (pos2 + 2) / 3; j += 1) {
                suitemouv[nbmvs] = haut;
                nbmvs += 1;
            }
            nbmvs = 0;
            for (unsigned j = (pos1 + 2) / 3; j < (pos2 + 2) / 3; j += 1) {
                suitemouv_bis[nbmvs] = haut;
                nbmvs += 1;
            }
            for (unsigned j = (pos2 + 2) / 3; j < (pos1 + 2) / 3; j += 1) {
                suitemouv_bis[nbmvs] = bas;
                nbmvs += 1;
            }
            for (unsigned i = (pos1 + 2) % 3; i < (pos2 + 2) % 3; i += 1) {
                suitemouv_bis[nbmvs] = droite;
                nbmvs += 1;
            }
            for (unsigned i = (pos2 + 2) % 3; i < (pos1 + 2) % 3; i += 1) {
                suitemouv_bis[nbmvs] = gauche;
                nbmvs += 1;
            }
            nb += 1;
        }
    } else if (pos1 == zero) {
        if (pos2 == valid) {
            suitemouv[0] = droite;
            nbmvs = 1;
        } else {
            t_suitemouv asmb = mouvements(pos2, zero);
            nb = asmb.nb;
            nbmvs = asmb.nbmvs;
            for (int i = 0; i < nbmvs; i += 1) {
                suitemouv[i] = inverse(asmb.suitemouv[0][nbmvs - 1 - i]);
                suitemouv_bis[i] = inverse(asmb.suitemouv[1][nbmvs - 1 - i]);
            }
            free(asmb.suitemouv[0]);
            free(asmb.suitemouv[1]);
            free(asmb.suitemouv);
        }
    } else if (pos1 < valid) {
        if (pos1 == bas) {
            if (pos2 == valid) {
                nbmvs = 2;
                suitemouv[0] = haut;
                suitemouv[1] = droite;
                suitemouv_bis[0] = droite;
                suitemouv_bis[1] = haut;
                nb += 1;
            } else {
                nbmvs = 1;
                suitemouv[0] = pos2;
            }

        } else if (pos1 == gauche) {
            suitemouv[0] = droite;
            nbmvs = 1;
            if (pos2 == valid) {
                nbmvs += 2;
                suitemouv[1] = droite;
                suitemouv[2] = haut;
            } else if (pos2 != bas) {
                nbmvs += 1;
                suitemouv[1] = pos2;
            }
        } else if (pos1 == haut) {
            if (pos2 == valid) {
                suitemouv[0] = droite;
                nbmvs = 1;
            } else if (pos2 == bas) {
                suitemouv[0] = bas;
                nbmvs = 1;
            } else if (pos2 == droite) {
                suitemouv[0] = bas;
                suitemouv[1] = droite;
                suitemouv_bis[0] = droite;
                suitemouv_bis[1] = bas;
                nbmvs = 2;
                nb += 1;
            } else {
                suitemouv[0] = bas;
                suitemouv[1] = gauche;
                nbmvs = 2;
            }
        } else {
            if (pos2 == valid) {
                suitemouv[0] = haut;
                nbmvs = 1;
            } else if (pos2 == bas) {
                suitemouv[0] = gauche;
                nbmvs = 1;
            } else if (pos2 == haut) {
                suitemouv[0] = gauche;
                suitemouv[1] = haut;
                suitemouv_bis[0] = haut;
                suitemouv_bis[1] = gauche;
                nbmvs = 2;
                nb += 1;
            } else {
                suitemouv[0] = gauche;
                suitemouv[1] = gauche;
                nbmvs = 2;
            }
        }
    } else {
        t_suitemouv asmb = mouvements(pos2, valid);
        nb = asmb.nb;
        nbmvs = asmb.nbmvs;
        for (int i = 0; i < nbmvs; i += 1) {
            suitemouv[i] = inverse(asmb.suitemouv[0][nbmvs - 1 - i]);
            suitemouv_bis[i] = inverse(asmb.suitemouv[1][nbmvs - 1 - i]);
        }
        free(asmb.suitemouv[0]);
        free(asmb.suitemouv[1]);
        free(asmb.suitemouv);
    }
    allsuitemouv.nb = nb;
    allsuitemouv.nbmvs = nbmvs;
    return allsuitemouv;
}

typedef struct t_etat t_etat;
struct t_etat {
    int p;
    int bouton;
    int posrob;
};

int lenhtb(int nbt)
{
    return nbt * NBMV * NBMV;
}

int hash(t_etat etat)
{
    int p = etat.p;
    int bouton = etat.bouton;
    int posrob = etat.posrob;
    return (p * NBMV + bouton) * NBMV + posrob;
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

    int wid = 0;
    while (inp[wid] != '\n') {
        wid += 1;
    }
    int nb_seq = 0;
    char* sequences = malloc(30 * sizeof(char));
    int i = 0;
    while (i < nbread - 1) {
        for (int j = 0; j < wid; j += 1) {
            sequences[wid * nb_seq + j] = inp[i];
            i += 1;
        }
        i += 1;
        nb_seq += 1;
    }

    int nbt1 = 4;
    int nbt2 = 27;
    int len1 = lenhtb(nbt2);
    long* hshtb = malloc(len1 * sizeof(long));
    t_suitemouv* suitemouvs = malloc(NBMV * NBMV * sizeof(t_suitemouv));
    for (int pos1 = 0; pos1 < NBMV; pos1++) {
        for (int pos2 = 0; pos2 < NBMV; pos2++) {
            if (pos1 == pos2) {
                suitemouvs[pos1 * NBMV + pos2] = (t_suitemouv) {};
                continue;
            }
            if (pos1 <= neuf && pos2 != valid && pos2 > neuf) {
                suitemouvs[pos1 * NBMV + pos2] = (t_suitemouv) {};
                continue;
            }
            if (pos2 <= neuf && pos1 != valid && pos1 > neuf) {
                suitemouvs[pos1 * NBMV + pos2] = (t_suitemouv) {};
                continue;
            }
            suitemouvs[pos1 * NBMV + pos2] = mouvements(pos1, pos2);
        }
    }
    for (int i = 0; i < len1; i += 1) {
        hshtb[i] = 0;
    }
    for (int posrob = 0; posrob < NBMV; posrob++) {
        for (int bouton = 0; bouton < NBMV; bouton++) {
            t_etat etat = (t_etat) { .posrob = posrob, .p = nbt2 - 1, .bouton = bouton };
            hshtb[hash(etat)] = 1;
        }
    }
    for (int p = nbt2 - 2; p >= 0; p -= 1) {
        for (int posrob = 0; posrob < NBMV; posrob++) {
            for (int bouton = 0; bouton < NBMV; bouton++) {
                if (posrob <= neuf && bouton != valid && bouton > neuf) {
                    continue;
                }
                if (bouton <= neuf && posrob != valid && posrob > neuf) {
                    continue;
                }
                t_etat etat = (t_etat) { .posrob = posrob, .p = p, .bouton = bouton };
                t_suitemouv suitemouv1 = suitemouvs[posrob * NBMV + bouton];
                int h = hash(etat);
                long option1 = 0;
                int pos1 = valid;
                for (int i = 0; i < suitemouv1.nbmvs; i += 1) {
                    int pos = suitemouv1.suitemouv[0][i];
                    t_etat etat0 = (t_etat) { .p = p + 1, .posrob = pos1, .bouton = pos };
                    option1 += hshtb[hash(etat0)];
                    pos1 = pos;
                }
                t_etat etatA = (t_etat) { .p = p + 1, .posrob = pos1, .bouton = valid };
                option1 += hshtb[hash(etatA)];
                if (suitemouv1.nb == 2) {
                    long option2 = 0;
                    int pos1 = valid;
                    for (int i = 0; i < suitemouv1.nbmvs; i += 1) {
                        int pos = suitemouv1.suitemouv[1][i];
                        t_etat etat0 = (t_etat) { .p = p + 1, .posrob = pos1, .bouton = pos };
                        option2 += hshtb[hash(etat0)];
                        pos1 = pos;
                    }
                    t_etat etatA = (t_etat) { .p = p + 1, .posrob = pos1, .bouton = valid };
                    option2 += hshtb[hash(etatA)];
                    if (option1 > option2) {
                        option1 = option2;
                    }
                }
                hshtb[h] += option1;
            }
        }
    }

    long ret1 = 0;
    long ret2 = 0;
    for (int i = 0; i < nb_seq; i += 1) {
        int posrob = valid;
        long lenmv1 = 0;
        long lenmv2 = 0;
        for (int j = 0; j < wid; j += 1) {
            int bt = sequences[i * wid + j];
            int bouton = isdigit(bt) ? bt - '0' : valid;
            t_etat etat1 = (t_etat) { .p = nbt2 - nbt1, .posrob = posrob, .bouton = bouton };
            lenmv1 += hshtb[hash(etat1)];
            t_etat etat2 = (t_etat) { .p = nbt2 - nbt2, .posrob = posrob, .bouton = bouton };
            lenmv2 += hshtb[hash(etat2)];
            posrob = bouton;
        }
        int numer = 0;
        for (int k = 0; k < wid - 1; k += 1) {
            numer = numer * 10 + (sequences[i * wid + k] - '0');
        }
        ret1 += numer * lenmv1;
        ret2 += numer * lenmv2;
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
    free(sequences);
    free(hshtb);
    for (int i = 0; i < NBMV * NBMV; i += 1) {
        if (suitemouvs[i].suitemouv != 0) {
            free(suitemouvs[i].suitemouv[0]);
            free(suitemouvs[i].suitemouv[1]);
            free(suitemouvs[i].suitemouv);
        }
    }
    free(suitemouvs);
}
