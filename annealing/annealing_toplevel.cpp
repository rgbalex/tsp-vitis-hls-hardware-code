#include "annealing_toplevel.h"


#pragma region Helper Functions

int index(int row, int col, int total_cities) {
#pragma HLS INLINE
    return row + total_cities * col;
}

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[], int path_length) {
    int cost = 0;
    for (int i = 0; i < path_length-1; i++) {
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    return cost;
}

#pragma endregion

#pragma region Nearest Neighbour First

int nearest_neigbour_first (uint8 adjacency_matrix[], int num_cities) {
    int run = 1;
    int visited_cities[20];
// #pragma HLS ARRAY_PARTITION variable=visited_cities dim=1 complete
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

//    printf("Your upper bound for distance is %d\n", worst_case_distance);

    return worst_case_distance;
}

#pragma endregion

int annealing(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance) {
	#pragma HLS TOP name=anneal
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=_number_of_cities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=_shortest_calculated_distance bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

	int output_cost;
	int num_cities = *_number_of_cities;

    uint8 cache[400]; // enough space for a 20x20 matrix
    memcpy(cache, ram, 400*sizeof(uint8));

    // Print out all elements in cache
    print_loop_i: for (int i = 0; i < num_cities; i++) {
        print_loop_j: for (int j = 0; j < num_cities; j++) {
            printf("%d ", cache[index(i, j, num_cities)]);
        }
        printf("\n");
    }
    printf("\n");

    // Rest of the code...
    output_cost = nearest_neigbour_first(cache, num_cities);
    printf("Nearest Neighbour First: %d\n", output_cost);

    *_shortest_calculated_distance = output_cost;

    return 99;
}
