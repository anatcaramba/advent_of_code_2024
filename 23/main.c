#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define NBL 26

typedef struct t_arete t_arete;
struct t_arete {
    int src;
    int dest;
    t_arete* suiv;
};

int hash(char c1, char c2)
{
    return (c1 - 'a') * NBL + (c2 - 'a');
}

int bronkerbosch(t_arete** adj, int n, int* r, int nbr, int* p, int nbp, int* x, int nbx)
{
    int ret = (nbp == 0 && nbx == 0) ? nbr : 0;
    int rnbr = nbr + 1;
    int* rr = malloc(rnbr * sizeof(int));
    for (int j = 0; j < nbr; j += 1) {
        rr[j] = r[j];
    }
    int* rp = malloc(nbp * sizeof(int));
    int* rx = malloc(n * sizeof(int));
    for (int i = 0; i < nbp; i += 1) {
        rr[nbr] = p[i];
        int rnbp = 0;
        int rnbx = 0;
        t_arete* ari = adj[p[i]];
        while (ari != 0) {
            int dest = ari->dest;
            for (int j = 0; j < nbp; j += 1) {
                if (p[j] == dest) {
                    rp[rnbp] = dest;
                    rnbp += 1;
                    break;
                }
            }
            for (int j = 0; j < nbx; j += 1) {
                if (x[j] == dest) {
                    rx[rnbx] = dest;
                    rnbx += 1;
                    break;
                }
            }
            ari = ari->suiv;
        }
        int ret_rec = bronkerbosch(adj, n, rr, rnbr, rp, rnbp, rx, rnbx);
        ret = ret_rec > ret ? ret_rec : ret;
        nbx += 1;
        x[nbx - 1] = p[i];
        p[i] = -1;
    }
    free(rp);
    free(rx);
    free(rr);
    return ret;
}

void bronkerbosch_find(t_arete** adj, int n, int* r, int nbr, int* p, int nbp, int* x, int nbx, int cible)
{
    int ret = (nbp == 0 && nbx == 0) ? nbr : 0;
    if (ret == cible) {
        for (int i = 0; i < nbr - 1; i += 1) {
            int mini = NBL * NBL;
            int indmini = -1;
            for (int j = i; j < nbr; j += 1) {
                if (mini > r[j]) {
                    mini = r[j];
                    indmini = j;
                }
            }
            r[indmini] = r[i];
            r[i] = mini;
            printf("%c%c,", r[i] / NBL + 'a', r[i] % NBL + 'a');
        }
        printf("%c%c\n", r[nbr - 1] / NBL + 'a', r[nbr - 1] % NBL + 'a');
    }
    int rnbr = nbr + 1;
    int* rr = malloc(rnbr * sizeof(int));
    for (int j = 0; j < nbr; j += 1) {
        rr[j] = r[j];
    }
    int* rp = malloc(nbp * sizeof(int));
    int* rx = malloc(n * sizeof(int));
    for (int i = 0; i < nbp; i += 1) {
        rr[nbr] = p[i];
        int rnbp = 0;
        int rnbx = 0;
        t_arete* ari = adj[p[i]];
        while (ari != 0) {
            int dest = ari->dest;
            for (int j = 0; j < nbp; j += 1) {
                if (p[j] == dest) {
                    rp[rnbp] = dest;
                    rnbp += 1;
                    break;
                }
            }
            for (int j = 0; j < nbx; j += 1) {
                if (x[j] == dest) {
                    rx[rnbx] = dest;
                    rnbx += 1;
                    break;
                }
            }
            ari = ari->suiv;
        }
        bronkerbosch_find(adj, n, rr, rnbr, rp, rnbp, rx, rnbx, cible);
        nbx += 1;
        x[nbx - 1] = p[i];
        p[i] = -1;
    }
    free(rp);
    free(rx);
    free(rr);
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

    int n = NBL * NBL;
    t_arete** adj = malloc(n * sizeof(t_arete*));
    for (int i = 0; i < n; i += 1) {
        adj[i] = 0;
    }
    int i = 0;
    int m = 0;
    while (i < nbread - 1) {
        int u = hash(inp[i], inp[i + 1]);
        int v = hash(inp[i + 3], inp[i + 4]);
        t_arete* uv = malloc(sizeof(t_arete));
        t_arete* vu = malloc(sizeof(t_arete));
        uv->src = u;
        uv->dest = v;
        uv->suiv = adj[u];
        adj[u] = uv;
        vu->src = v;
        vu->dest = u;
        vu->suiv = adj[v];
        adj[v] = vu;
        m += 1;
        i += 6;
    }

    // ETOILES
    long ret1 = 0;
    int hta = hash('t', 'a');
    int htz = hash('t', 'z');
    for (int h = hta; h <= htz; h += 1) {
        t_arete* ar1 = adj[h];
        while (ar1 != 0) {
            t_arete* ar2 = ar1->suiv;
            t_arete* aru = adj[ar1->dest];
            while (ar2 != 0) {
                t_arete* aru_cp = aru;
                while (aru_cp != 0) {
                    if (aru_cp->dest == ar2->dest) {
                        if (hta <= ar1->dest && ar1->dest <= htz) {
                            if (hta <= ar2->dest && ar2->dest <= htz) {
                                ret1 += 2;
                            } else {
                                ret1 += 3;
                            }
                        } else if (hta <= ar2->dest && ar2->dest <= htz) {
                            ret1 += 3;
                        } else {
                            ret1 += 6;
                        }
                    }
                    aru_cp = aru_cp->suiv;
                }
                ar2 = ar2->suiv;
            }
            ar1 = ar1->suiv;
        }
    }
    ret1 /= 6;
    printf("%ld\n", ret1);

    int* p = malloc(n * sizeof(int));
    int* p2 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i += 1) {
        p[i] = i;
        p2[i] = i;
    }
    int* x = malloc(n * sizeof(int));
    int cible = bronkerbosch(adj, n, 0, 0, p, n, x, 0);
    bronkerbosch_find(adj, n, 0, 0, p2, n, x, 0, cible);

    // FREES
    for (int i = 0; i < n; i += 1) {
        t_arete* ar = adj[i];
        while (ar != 0) {
            t_arete* suiv = ar->suiv;
            free(ar);
            ar = suiv;
        }
    }
    free(adj);
    free(x);
    free(p);
    free(p2);
}
