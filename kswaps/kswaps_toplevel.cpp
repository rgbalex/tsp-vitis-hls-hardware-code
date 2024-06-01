#include "kswaps_toplevel.h"

#include <cmath>

int index(int row, int col, int total_cities) {
#pragma HLS INLINE
    return row + total_cities * col;
}

#pragma region Helper Functions

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[20]) {
    int cost = 0;
    path_cost_for_loop: for (int i = 0; i < num_cities; i++) {
#pragma HLS LOOP_TRIPCOUNT max=20 min=4
#pragma HLS PIPELINE
        printf("City %d to %d: %d\n", path[i], path[i+1], adjacency_matrix[index(path[i], path[i+1], num_cities)]);
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    return cost;
}

#pragma endregion

#pragma region Helper Functions for two-opt
// see https://slowandsteadybrain.medium.com/traveling-salesman-problem-ce78187cf1f3
// void two_opt_swap(int* original_path, int* out_path, int first, int second) {
//     int swapped_path[20];
//     // zero out path
//     // memset(swapped_path, -1, 20);
//     // reset out_path
//     memcpy(swapped_path, original_path, 20*sizeof(int));
//     // print initial path
//     printf("Initial path:  ");
//     for (int i = 0; i < 20; i++) {
//         printf("%d ", original_path[i]);
//     }
//     printf("\n");

//     // swap elements at positions i and j
//     int temp = swapped_path[first];
//     swapped_path[first] = swapped_path[second];
//     swapped_path[second] = temp;

//     // print swapped path
//     printf("Swapped path:  ");
//     for (int i = 0; i < 20; i++) {
//         printf("%d ", swapped_path[i]);
//     }
//     printf("\n");

//     memcpy(swapped_path, out_path, 20*sizeof(int));
    
//     // return swapped_path;
// }

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

int two_opt(uint8 adjacency_matrix[], int num_cities, int original_path[20], int path_length) {

    // put original path into present_route to be calculated
    int present_route[20];
    memcpy(present_route, original_path, sizeof(present_route));

    int new_distance = 0xFFFF;
    int new_route[20];
    int best_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, present_route);
    // memcpy route from path
    
    for (int i = 1; i < path_length-2; i++) {
#pragma HLS LOOP_FLATTEN off
        for (int j = i+1; j < path_length - 1; j++) {
#pragma HLS LOOP_FLATTEN off

            // swap elements at positions i and j
            // print new_route
            printf("\nSwapping %d and %d\n", i, j);
            printf(" Present cost: %d\n", best_distance);
            printf("Present route: ");
            for (int i = 0; i < num_cities+1; i++) {
                printf("%d ", present_route[i]+1);
            }
            printf("\n");


            two_opt_swap(original_path, new_route, i, j);

            new_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, new_route);

            // print cost and new_route
            printf("     New cost: %d\n", best_distance);
            printf("    New route: ");
            for (int i = 0; i < num_cities+1; i++) {
                printf("%d ", present_route[i]+1);
            }
            printf("\n");


            if (new_distance < best_distance) {
                best_distance = new_distance;
                // memcpy route from new_route
                // present_route = new_route;
                memcpy(present_route, new_route, sizeof(present_route));
            }
        }
    }
    // set path to best path
    memcpy(new_route, present_route, sizeof(present_route));
    return best_distance;
}
#pragma endregion

#pragma region Nearest Neighbour First

int nearest_neigbour_first(uint8 adjacency_matrix[], int num_cities, int visited_cities[20]) {
    int run = 1;
    int skip = 0;
    int distance = 0;
    int min_city = -1;
    int min_distance = 0xFFFF;
    int visited_cities_tail = 0;
    int worst_case_distance = 0;
    // start at city 0
    visited_cities[0] = 0;
    visited_cities_tail += 1;

    nearest_neighbour_run: while (run) {
    #pragma HLS LOOP_TRIPCOUNT max=20 min=4

        // get the last city visited
        nearest_neighbour_new_city: for (int visiting_city = 0; visiting_city < num_cities; visiting_city++) {
        #pragma HLS LOOP_TRIPCOUNT max=20 min=4
        #pragma HLS LOOP_FLATTEN off
            skip = 0;
            distance = adjacency_matrix[index(visited_cities[visited_cities_tail-1], visiting_city, num_cities)];

            //changing this breaks things
            check_if_visited: for (int city = 0; city < num_cities; city++) {
            #pragma HLS LOOP_TRIPCOUNT max=20 min=4
            #pragma HLS UNROLL
                if (visited_cities[city] == visiting_city) {
                    skip = 1;
                }
            }
            
            if (!skip & (distance < min_distance)) {
                min_distance = distance;
                min_city = visiting_city;
            }
        }

        if (visited_cities_tail == num_cities) {
            run = 0;
        } else {
            visited_cities[visited_cities_tail] = min_city;
            visited_cities_tail += 1;
            worst_case_distance += min_distance;
            // printf("City: %d, Distance: %d, Worst case: %d\n", min_city, min_distance, worst_case_distance);
            min_distance = 0xFFFF;
        }
    }

    // finish with a loop back to city 0
    int final_distance = adjacency_matrix[index(visited_cities[visited_cities_tail-1], 0, num_cities)];
    worst_case_distance += final_distance;

    // add this to the path
    visited_cities[visited_cities_tail] = 0;
    visited_cities_tail += 1;
    printf("Visited cities: ");
    for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");
    printf("Visited cities tail: %d\n", visited_cities_tail);
    int better_distance = two_opt(adjacency_matrix, num_cities, visited_cities, visited_cities_tail);

    // better_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, swapped_cities);
    printf("Better distance is %d\n", better_distance);
    printf("Better route is: ");
    print_loop_better: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");

    return worst_case_distance;
}

#pragma endregion

#pragma region Main Function

int solve_the_tsp(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance) {
	#pragma HLS TOP name=anneal
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=_number_of_cities bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

	int num_cities = *_number_of_cities;
	int nearest_neighbour_first_cost = 0;

    int _visited_cities[20];
    #pragma HLS ARRAY_PARTITION variable=_visited_cities dim=1 complete
    memset(_visited_cities, -1, 20);

    uint8 cache[400]; // enough space for a 20x20 matrix, initialized with zeros
    memcpy(cache, ram, 400*sizeof(uint8));

    // Print out all elements in cache
    print_loop_i: for (int i = 0; i < num_cities; i++) {
        print_loop_j: for (int j = 0; j < num_cities; j++) {
            printf("%3u ", cache[(num_cities * j) + i]);
        }
        printf("\n");
    }
    printf("\n");

    // Rest of the code...
    // print number of cities
    printf("Number of cities: %d\n", num_cities);

    nearest_neighbour_first_cost = nearest_neigbour_first(cache, num_cities, _visited_cities);
    printf("Nearest Neighbour First: %d\n", nearest_neighbour_first_cost);

    printf("Visited Cities:  ");
    for (int i = 0; i < num_cities+1; i++) {
        printf("%d ", _visited_cities[i]+1);
    }
    printf("\n");


    *_shortest_calculated_distance = nearest_neighbour_first_cost;

    return 1;
}

#pragma endregion
