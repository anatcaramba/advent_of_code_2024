#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LENINP 100000
#define NLINES 1000

int compare(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int main(void)
{
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        return 1;
    }
    int* vals1 = malloc(NLINES * sizeof(int));
    int* vals2 = malloc(NLINES * sizeof(int));
    int j = 0;
    for (int i = 0; i < NLINES; i += 1) {
        int x = 0;
        int y = 0;
        int ret = scanf("%d %d", &x, &y);
        if (ret != 2) {
            return 1;
        }
        vals1[i] = x;
        vals2[i] = y;
    }
    qsort(vals1, NLINES, sizeof(int), compare);
    qsort(vals2, NLINES, sizeof(int), compare);

    long s = 0;
    for (int i = 0; i < NLINES; i += 1) {
        int y = (-vals1[i] + vals2[i]);
        s += y < 0 ? -y : y;
    }

    printf("%ld\n", s);
    j = 0;
    int s2 = 0;
    for (int i = 0; i < NLINES; i += 1) {
        while (j < NLINES - 1 && vals2[j] < vals1[i]) {
            j += 1;
        }
        while (j < NLINES - 1 && vals2[j] == vals1[i]) {
            s2 += vals1[i];
            j += 1;
        }
    }
    printf("%d\n", s2);

    free(vals1);
    free(vals2);
}
