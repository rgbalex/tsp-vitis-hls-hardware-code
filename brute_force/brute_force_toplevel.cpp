#include "brute_force_toplevel.h"
#include <stdio.h>
#include <string.h>

#pragma region Helper Functions

int index(int row, int col, int total_cities) {
    return row + total_cities * col;
}

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[20]) {
    int cost = 0;
    for (int i = 0; i < num_cities; i++) {
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    return cost;
}

#pragma endregion

#pragma region Main Function

int solve_the_tsp(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance) {
    int num_cities = *_number_of_cities;

    int current_cost = INF;
    int current_best_cost = INF;
    int _current_best_path[20];
    memset(_current_best_path, -1, 20);

    // initialize current best path to ints ascending up to number of cities
    for (int i = 0; i < num_cities; i++) {
        _current_best_path[i] = i;
    }
    _current_best_path[num_cities] = 0;

    uint8 cache[400];
    memcpy(cache, ram, 400*sizeof(uint8));

    *_shortest_calculated_distance = 0x7FFFFFFF;

    // calculate the cost of the current best path
    current_best_cost = path_cost_from_adjacency_matrix(cache, num_cities, _current_best_path);

    int a[20];
    memcpy(a, _current_best_path, 20*sizeof(int));

    unsigned int p[20];
    unsigned int i, j, tmp;
    for(i = 0; i < num_cities; i++) {
        p[i] = i;
    }
    p[num_cities] = num_cities;
    i = 1;

    while(i < num_cities-1) {
        p[i]--;
        j = i % 2 * p[i];
        tmp = a[j+1];
        a[j+1] = a[i+1];
        a[i+1] = tmp;

        if (!(a[1] > a[num_cities-1])){
            current_cost = path_cost_from_adjacency_matrix(cache, num_cities, a);
            if (current_cost < current_best_cost) {
                current_best_cost = current_cost;
                memcpy(_current_best_path, a, 20*sizeof(int));
            }
        }

        i = 1;
        int run = 1;
        while (run) {
            if (p[i]) {
                run = 0;
            } else {
                p[i] = i;
                i++;
            }
        }
    }

    *_shortest_calculated_distance = current_best_cost;

    memcpy(ram, _current_best_path, 20*sizeof(int));

    return -1;
}

#pragma endregion
