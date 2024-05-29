#include "toplevel.h"

#ifdef __linux__
#include "hls_math.h"
#else
#include <cmath>
#endif


#include <stdio.h>

#include <cstring> // Include for memcpy

#define INF 0xFFFF

int index(int row, int col, int total_cities) {
#pragma HLS INLINE
    return row + total_cities * col;
}

int rand() {
    // hls did not know how to make a random number generator so I found one on the internet
    static unsigned int seed = 12345; // initial seed value
    seed = (1103515245 * seed + 12345) % (1 << 31); // linear congruential generator formula
    return seed;
}

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[], int path_length) {
    int cost = 0;
    for (int i = 0; i < path_length-1; i++) {
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    return cost;
}

#pragma region Helper Functions for two-opt
// see https://slowandsteadybrain.medium.com/traveling-salesman-problem-ce78187cf1f3
int* two_opt_swap(int in_path[20], int out_path[20], int first, int second) {
    // zero out path
    memset(out_path, -1, 20);
    // copy elements from in path
    memcpy(out_path, in_path, 20*sizeof(int));
    // swap elements at positions first and second
    int temp = out_path[first];
    out_path[first] = out_path[second];
    out_path[second] = temp;
    return out_path;
}

int two_opt(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length) {
    int new_distance = INF;
    int new_route[20];
    int best_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, path, path_length);
    int present_route[20];
    // memcpy route from path
    memcpy(present_route, path, sizeof(present_route));
    
    for (int i = 1; i < path_length-2; i++) {
#pragma HLS LOOP_FLATTEN off
        for (int j = 1; j < path_length - 1; j++) {
#pragma HLS LOOP_FLATTEN off
            two_opt_swap(present_route, new_route, i, j);
            // printf("Swapping %d and %d\n", i, j);
            new_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, new_route, path_length);

            if (new_distance < best_distance) {
                best_distance = new_distance;
                // memcpy route from new_route
                // present_route = new_route;
                memcpy(present_route, new_route, sizeof(present_route));
            }
        }
    }
    // set path to best path
    memcpy(path, present_route, sizeof(present_route));
    return best_distance;
}
#pragma endregion

#pragma region Helper Functions for three-opt
int* three_opt_swap(int in_path[20], int out_path[20], int first, int second, int third) {
    // zero out path
    memset(out_path, -1, 20);
    // copy elements from in path
    memcpy(out_path, in_path, 20*sizeof(int));
    // swap elements at positions first and second
    int temp = out_path[first];
    out_path[first] = out_path[second];
    out_path[second] = temp;
    // swap elements at positions second and third
    temp = out_path[second];
    out_path[second] = out_path[third];
    out_path[third] = temp;
    return out_path;
}

int three_opt(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length) {
    int new_distance = INF;
    int new_route[20];
    int best_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, path, path_length);
    int present_route[20];
    // memcpy route from path
    memcpy(present_route, path, sizeof(present_route));
    
    for (int i = 1; i < path_length-3; i++) {
        for (int j = 1; j < path_length - 2; j++) {
            for (int k = 1; k < path_length - 1; k++) {
                three_opt_swap(present_route, new_route, i, j, k);
                // printf("Swapping %d and %d\n", i, j);
                new_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, new_route, path_length);

                if (new_distance < best_distance) {
                    best_distance = new_distance;
                    // memcpy route from new_route
                    // present_route = new_route;
                    memcpy(present_route, new_route, sizeof(present_route));
                }
            }
        }
    }
    // set path to best path
    memcpy(path, present_route, sizeof(present_route));
    return best_distance;
}

#pragma endregion

#pragma region Helper Functions for simulated-annealing
int anneal(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length) {
    int iteration = -1;
    double temperature = 10000.0;
    double cooling_rate = 0.99999;
    double absolute_temperature = 0.00001;
    int distance = 9999;
    int current_path[20];
    int new_path[20];
    double acceptance_probability;
    // memcpy route from path
    memcpy(current_path, path, sizeof(current_path));
    // assuming the cities are already set up
    
    distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, path, path_length);
    int run = 1;
    annealing_while: while (run) {
        if (temperature > absolute_temperature) { run = 0; }

        memcpy(new_path, current_path, sizeof(new_path));
        int first = rand() % path_length;
        int second = rand() % path_length;
        bool a = (first == second);
        bool b = (first == 0 || second == 0);
        bool c = (first == path_length-1 || second == path_length-1);

        if (!(a || b || c)) {
        	// printf("Swapping %d and %d\n", first, second);

        	        int temp = new_path[first];
        	        new_path[first] = new_path[second];
        	        new_path[second] = temp;

        	        int new_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, new_path, path_length);

					acceptance_probability = exp((distance - new_distance) / temperature);

        	        if (acceptance_probability > (rand() % 100) / 100) {
        	            memcpy(current_path, new_path, sizeof(new_path));
        	            distance = new_distance;
        	        }

        	        temperature *= cooling_rate;
        	        iteration += 1;
        }
    }
    // set path to best path
    memcpy(path, current_path, sizeof(current_path));
    return distance;
}
#pragma endregion

int nearest_neigbour_first (uint8 adjacency_matrix[], int num_cities) {
    int run = 1;
    int visited_cities[20];
#pragma HLS ARRAY_PARTITION variable=visited_cities dim=1 complete
    memset(visited_cities, -1, 20);
    int visited_cities_tail = 0;

    int distance = 0;
    int min_distance = INF;
    int min_city = -1;

    int skip = 0;
    int worst_case_distance = 0;

    // start at city 0

    visited_cities[0] = 0;
    visited_cities_tail += 1;

    nearest_neighbour_run: while (run) {
#pragma HLS LOOP_TRIPCOUNT avg=4 max=20 min=4

        // get the last city visited
        nearest_neighbour_new_city: for (int current_city = 0; current_city < num_cities; current_city++) {
#pragma HLS LOOP_TRIPCOUNT avg=4 max=20 min=4
#pragma HLS LOOP_FLATTEN off
            skip = 0;
            distance = adjacency_matrix[index(visited_cities[visited_cities_tail-1], current_city, num_cities)];

            //changing this breaks things
            check_if_visited: for (int city = 0; city < num_cities; city++) {
#pragma HLS LOOP_TRIPCOUNT avg=4 max=20 min=4
                if (visited_cities[city] == current_city) {
                    skip = 1;
                }
            }

            if (skip) { continue; }

            if (distance < min_distance) {
                min_distance = distance;
                min_city = current_city;
            }
        }


        if (visited_cities_tail == num_cities) {
            run = 0;
        } else {
            visited_cities[visited_cities_tail] = min_city;
            visited_cities_tail += 1;
            worst_case_distance += min_distance;
            // printf("City: %d, Distance: %d, Worst case: %d\n", min_city, min_distance, worst_case_distance);
            min_distance = INF;
        }
    }

    // finish with a loop back to city 0
    int final_distance = adjacency_matrix[index(visited_cities[visited_cities_tail-1], 0, num_cities)];
    worst_case_distance += final_distance;
    // add this to the path
    visited_cities[visited_cities_tail] = 0;
    visited_cities_tail += 1;

    // printf("Adding route from last visited city back to city 0...\n");
    // printf("City: %d, Distance: %d, Worst case: %d\n\n", 0, final_distance, worst_case_distance);


    // print out the visited_cities vector as this contains your nearest neighbour tour
    // print_loop: for (int i = 0; i < visited_cities_tail; i++) {
    //     printf("%d ", visited_cities[i]+1);
    // }
    // printf("\n\n");
   
    printf("Your upper bound for distance is %d\n", worst_case_distance);

    int cost = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);
    printf("Path cost recalculated from path is %d\n", cost);

    // perform two opt swap
    int better_distance = two_opt(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);
    printf("Better distance is %d\n", better_distance);
    printf("Better route is: ");
    print_loop_better: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");

    // perform three opt swap
    int best_distance = three_opt(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);
    printf("Best distance is %d\n", best_distance);
    printf("Best route is: ");
    print_loop_best: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");

    // perform simulated annealing
    int annealed_distance = anneal(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);
    printf("Annealed distance is %d\n", annealed_distance);
    printf("Annealed route is: ");
    print_loop_annealed: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");
    
    printf("Double checking the calculated annealed distance");
    int annealed_cost = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);
    printf("Path cost recalculated from path is %d\n", annealed_cost);

    int best_case_distance = worst_case_distance;
    if (better_distance < best_case_distance) {
        best_case_distance = better_distance;
    }
    if (best_distance < best_case_distance) {
        best_case_distance = best_distance;
    }
    if (annealed_distance < best_case_distance) {
        best_case_distance = annealed_distance;
    }
    printf("Minimum distance is %d\n", best_case_distance);

    return best_case_distance;
}

// Main Function - solver
int toplevel(uint32 *ram, uint32 *message_id, uint32 *number_of_cities, uint32 *scenario_id, uint32 *shortest_calculated_distance) {
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=message_id bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=number_of_cities bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=scenario_id bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=shortest_calculated_distance bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

    uint8 cache[400]; // enough space for a 20x20 matrix

    uint8 char_number_of_cities = (char)(*number_of_cities >> 24);
    int int_number_of_cities = *number_of_cities;
    uint32 shortest_path = -1;

    // Build the problem
    memcpy(cache, ram, sizeof(cache));

    // Print out all elements in cache
    print_loop_i: for (int i = 0; i < int_number_of_cities; i++) {
        print_loop_j: for (int j = 0; j < int_number_of_cities; j++) {
            printf("%d ", cache[index(i, j, int_number_of_cities)]);
        }
        printf("\n");
    }
   printf("\n");

    // Rest of the code...
    int nnf = nearest_neigbour_first(cache, int_number_of_cities);
    // Write the shortest distance to the variable
    *shortest_calculated_distance = nnf;
    // Finish up - add the shortest path
    return nnf;
}
