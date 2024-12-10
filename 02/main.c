#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100000
#define WID 100
#define HEI 1000

int main(void)
{
    // PARSING
    FILE* f = freopen("./input", "r", stdin);
    if (f == 0) {
        return 0;
    }
    char inp[MAXLEN];
    int r = fread(inp, sizeof(char), MAXLEN, stdin);
    if (r == 0) {
        return 0;
    }
    int grid[HEI][WID];
    long ret2 = 0;
    long ret1 = 0;
    int i = 0;
    int k = 0;
    int j = 0;
    while (j < HEI && inp[i]) {
        while (inp[i] && !isdigit(inp[i])) {
            if (inp[i] == '\n') {
                grid[j][k] = -1;
                grid[j][k + 1] = -1;
                grid[j][k + 2] = -1;

                j += 1;
                if (j == HEI) {
                    break;
                }
                k = 0;
            }
            i += 1;
        }
        if (j == HEI) {
            break;
        }
        int s = 0;
        while (inp[i] && isdigit(inp[i])) {
            s = 10 * s + inp[i] - '0';
            i += 1;
        }
        grid[j][k] = s;
        k += 1;
    }

    // ETOILE 1
    for (int j = 0; j < HEI; j += 1) {
        bool isdecr = true;
        bool ok = true;
        if (grid[j][0] < grid[j][1]) {
            isdecr = false;
        }
        for (int k = 0; grid[j][k + 1] != -1; k += 1) {
            if (isdecr && grid[j][k] - grid[j][k + 1] > 3) {
                ok = false;
                break;
            }
            if (isdecr && grid[j][k] - grid[j][k + 1] < 1) {
                ok = false;
                break;
            }
            if (!isdecr && grid[j][k + 1] - grid[j][k] > 3) {
                ok = false;
                break;
            }
            if (!isdecr && grid[j][k + 1] - grid[j][k] < 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            ret1 += 1;
        }
    }

    // ETOILE 2
    for (int j = 0; j < HEI; j += 1) {
        for (int deleted = 0; grid[j][deleted] != -1; deleted += 1) {
            int first = 0;
            int sec = 1;
            if (deleted == 0) {
                first = 1;
                sec = 2;
            }
            if (deleted == 1) {
                sec = 2;
            }
            bool isdecr = true;
            bool ok = true;
            if (grid[j][first] < grid[j][sec]) {
                isdecr = false;
            }
            if (isdecr && grid[j][first] - grid[j][sec] > 3) {
                continue;
            }
            if (isdecr && grid[j][first] - grid[j][sec] < 1) {
                continue;
            }
            if (!isdecr && grid[j][sec] - grid[j][first] > 3) {
                continue;
            }
            if (!isdecr && grid[j][sec] - grid[j][first] < 1) {
                continue;
            }
            int next = sec + 1;
            for (int k = sec; grid[j][next] != -1; k += 1) {
                if (k == deleted) {
                    continue;
                }
                if (next == deleted) {
                    next += 1;
                    if (grid[j][next] == -1) {
                        continue;
                    }
                }
                if (isdecr && grid[j][k] - grid[j][next] > 3) {
                    ok = false;
                    break;
                }
                if (isdecr && grid[j][k] - grid[j][next] < 1) {
                    ok = false;
                    break;
                }
                if (!isdecr && grid[j][next] - grid[j][k] > 3) {
                    ok = false;
                    break;
                }
                if (!isdecr && grid[j][next] - grid[j][k] < 1) {
                    ok = false;
                    break;
                }
                next += 1;
            }
            if (ok) {
                ret2 += 1;
                break;
            }
        }
    }
    printf("%ld\n", ret1);
    printf("%ld\n", ret2);
}
