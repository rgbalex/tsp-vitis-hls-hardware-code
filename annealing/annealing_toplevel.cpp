#include "annealing_toplevel.h"

int anneal(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length, int max_anneal);


#pragma region Helper Functions

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

#pragma endregion

#pragma region Nearest Neighbour First

int nearest_neigbour_first (uint8 adjacency_matrix[], int num_cities, int max_anneal) {
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

    // perform simulated annealing
    int annealed_distance = anneal(adjacency_matrix, num_cities, visited_cities, visited_cities_tail, max_anneal);
    printf("Annealed distance is %d\n", annealed_distance);
    printf("Annealed route is: ");
    print_loop_annealed: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");

    return annealed_distance;
}

#pragma endregion

#pragma region Simulated-Annealing

int anneal(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length, int max_anneal) {
    int iteration = -1;
    // light
    // double temperature = 10000.0;
    // double cooling_rate = 0.99999;
    // harsh
    double temperature = 100000.0;
    double cooling_rate = 0.999999;
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
        if (temperature < absolute_temperature) { run = 0; }
        if (iteration > max_anneal) { run = 0; }

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

                    int acceptance_probability = 100;
                    if (new_distance > distance) {
                        acceptance_probability = 0;
                    } else {
                        int exponent = (distance - new_distance) / temperature;
                        acceptance_probability = 100;
                        if (exponent > 0) {
                            double decay = 0.9;
                            int i = 0;
                            int inner_run = 1;
                            while (inner_run) {
//                            for (int i = 0; i < exponent; i++) {
                            	if (i > exponent) { inner_run = 0; }
                                acceptance_probability *= decay;
                            }
                        }
                    }

                    if (acceptance_probability > (rand() % 100)) {
                        memcpy(current_path, new_path, sizeof(new_path));
                        distance = new_distance;
                    }

        	        temperature *= cooling_rate;
        	        iteration += 1;
        }
    }
    printf("Iteration: %d\n", iteration);
    // set path to best path
    memcpy(path, current_path, sizeof(current_path));
    return distance;
}
#pragma endregion

#pragma region Main Function

int annealing(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance, int *_max_anneal) {
	#pragma HLS TOP name=anneal
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=_number_of_cities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=_shortest_calculated_distance bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=_max_anneal bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

	int output_cost;
	int num_cities = *_number_of_cities;
    int max_anneal = *_max_anneal;

    // check max_annneal is not zero
    if (max_anneal == 0) {
        max_anneal = 1000;
    } 
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
    output_cost = nearest_neigbour_first(cache, num_cities, max_anneal);

    *_shortest_calculated_distance = output_cost;

    return 99;
}

#pragma endregion
