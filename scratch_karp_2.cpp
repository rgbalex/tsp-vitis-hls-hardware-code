#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF 10000000
#define MAX_NODES 12

int min(int x, int y) {
    return (x < y) ? x : y;
}

int tsp_held_karp(int dist[][MAX_NODES], int set[], int n, int pos, int path[], int *path_len) {
    int i;

    if (set[pos] == -1) {
        if (pos == n - 1) {
            set[pos] = 0;
            path[(*path_len)++] = pos;
        } else {
            set[pos] = INF;
            for (i = pos + 1; i < n; i++) {
                int new_set[MAX_NODES] = {0};
                int new_path_len = *path_len;

                for (int j = 0; j < n; j++)
                    new_set[j] = set[j];

                new_set[pos] = -2; // mark as visited

                int new_cost = dist[pos][i] + tsp_held_karp(dist, new_set, n, i, path, &new_path_len);

                if (new_cost < set[pos]) {
                    set[pos] = new_cost;
                    path[(*path_len)++] = pos;
                }
            }
        }
    }

    return set[pos];
}

int main() {

    int dist[12][12] = {
        {0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60},
        {10, 0, 12, 18, 22, 28, 32, 38, 42, 48, 52, 58},
        {15, 12, 0, 8, 14, 20, 24, 30, 34, 40, 44, 50},
        {20, 18, 8, 0, 6, 12, 16, 22, 26, 32, 36, 42},
        {25, 22, 14, 6, 0, 8, 12, 18, 22, 28, 32, 38},
        {30, 28, 20, 12, 8, 0, 4, 10, 14, 20, 24, 30},
        {35, 32, 24, 16, 12, 4, 0, 6, 10, 16, 20, 26},
        {40, 38, 30, 22, 18, 10, 6, 0, 4, 10, 14, 20},
        {45, 42, 34, 26, 22, 14, 10, 4, 0, 6, 10, 16},
        {50, 48, 40, 32, 28, 20, 16, 10, 6, 0, 4, 10},
        {55, 52, 44, 36, 32, 24, 20, 14, 10, 4, 0, 6},
        {60, 58, 50, 42, 38, 30, 26, 20, 16, 10, 6, 0}
    };

    // int i, n = 4, set[MAX_NODES] = {-1, -1, -1, -1};
    int i, n = 12, set[12] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int path[MAX_NODES];
    int path_len = 0;

    int min_cost = tsp_held_karp(dist, set, n, 0, path, &path_len);

    printf("AAAA\nMinimum cost = %d\n", min_cost);
    printf("Path length: %d\n", path_len);
    printf("Path: ");
    for (i = 0; i < path_len; i++)
        printf("%d ", path[i]);
    printf("%d\n", 0); // add the starting node to complete the tour

    return 0;
}
