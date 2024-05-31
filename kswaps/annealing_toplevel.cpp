#include "annealing_toplevel.h"
#include <cmath>

int anneal(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length, double temperature, double cooling_rate, double absolute_temperature, int *_shortest_calculated_distance);

#pragma region Helper Functions

int index(int row, int col, int total_cities) {
#pragma HLS INLINE
    return row + total_cities * col;
}

int rand() {
    // hls did not know how to make a random number generator so I found one on the internet
    static unsigned int seed = 14987516; // initial seed value
    seed = (1103515245 * seed + 16547) % (1 << 31); // linear congruential generator formula
    return seed;
}

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[], int path_length) {
    int cost = 0;
    path_cost_for_loop: for (int i = 0; i < path_length-1; i++) {
#pragma HLS LOOP_TRIPCOUNT max=20 min=4
#pragma HLS PIPELINE
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    return cost;
}

#pragma endregion

#pragma region Nearest Neighbour First

int nearest_neigbour_first (uint8 adjacency_matrix[], int num_cities, double temperature, double cooling_rate, double absolute_temperature, int *_return_type, int *_shortest_calculated_distance) {
    int run = 1;
    int visited_cities[20];
#pragma HLS ARRAY_PARTITION variable=visited_cities dim=1 complete
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
#pragma HLS LOOP_TRIPCOUNT max=20 min=4

        // get the last city visited
        nearest_neighbour_new_city: for (int current_city = 0; current_city < num_cities; current_city++) {
#pragma HLS LOOP_TRIPCOUNT max=20 min=4
#pragma HLS LOOP_FLATTEN off
            skip = 0;
            distance = adjacency_matrix[index(visited_cities[visited_cities_tail-1], current_city, num_cities)];

            //changing this breaks things
            check_if_visited: for (int city = 0; city < num_cities; city++) {
#pragma HLS LOOP_TRIPCOUNT max=20 min=4
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


    // perform simulated annealing
    int annealed_distance = anneal(adjacency_matrix, num_cities, visited_cities, visited_cities_tail, temperature, cooling_rate, absolute_temperature, _shortest_calculated_distance);
    printf("Your upper bound for distance is %d\n", worst_case_distance);
    printf("Annealed distance is %d\n", annealed_distance);
    printf("Annealed route is: ");
    print_loop_annealed: for (int i = 0; i < visited_cities_tail; i++) {
        printf("%d ", visited_cities[i]+1);
    }
    printf("\n");
    printf("======================================================== \n");

    if (annealed_distance <= worst_case_distance) {
        *_return_type = 0;
        return annealed_distance;
    } else {
        *_return_type = 1;
        return worst_case_distance;
    }
    
    // return annealed_distance;
}

#pragma endregion

#pragma region Simulated-Annealing

int anneal(uint8 adjacency_matrix[], int num_cities, int path[20], int path_length, double temperature, double cooling_rate, double absolute_temperature, int *_shortest_calculated_distance) {
    // print the starting path
    printf("Starting path: ");
    for (int i = 0; i < path_length; i++) {
        printf("%d ", path[i]+1);
    }
    printf("\n");

    int iteration = -1;
    // light
    // double temperature = 100.0;
    // double cooling_rate = 0.9999;
    // double absolute_temperature = 0.00001;
    // harsh
    // double temperature = 10000000.0;
    // double cooling_rate = 0.99999999;
    // double absolute_temperature = 0.000001;
    // suggested
    // double cooling_rate = 0.999;
    // temperature = 100.0;
    // cooling_rate = 0.9999;
    // absolute_temperature = 1;

    int distance = 9999;
    int delta_distance = 1;
    
    int current_path[20];
#pragma HLS ARRAY_PARTITION variable=current_path dim=1 complete
    int new_path[20];
#pragma HLS ARRAY_PARTITION variable=new_path dim=1 complete
    int new_distance;
    
    int best_distance = 9999;
    int best_path[20];
#pragma HLS ARRAY_PARTITION variable=best_path dim=1 complete
    int best_iteration = 0;

    double acceptance_probability;
    double _random = (rand() % 100);
    int first, second;
    bool a;



    // memcpy route from path
    memcpy(current_path, path, sizeof(current_path));
    // assuming the cities are already set up
    
    distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, path, path_length);
    int run = 1;
    annealing_while: while (run) {
#pragma HLS LOOP_TRIPCOUNT max=3000000 min=1000
        if (temperature < absolute_temperature) { run = 0; }
        // if (iteration > max_anneal) { run = 0; }

        // see https://www.codeproject.com/Articles/26758/Simulated-Annealing-Solving-the-Travelling-Salesma
        memcpy(new_path, current_path, sizeof(current_path));
        
        first = rand() % (path_length-2);
        second = rand() % (path_length-2);

        // valid numbers
        first += 1;
        second += 1;

        int temp = new_path[first];
        new_path[first] = new_path[second];
        new_path[second] = temp;

        new_distance = path_cost_from_adjacency_matrix(adjacency_matrix, num_cities, new_path, path_length);

        // save the best seen path
        if (new_distance < best_distance) {
        	*_shortest_calculated_distance = new_distance;
            best_distance = new_distance;
            memcpy(best_path, new_path, sizeof(new_path));
            best_iteration = iteration;
        }

        // we want this to be minimised
        delta_distance = new_distance - distance;
        acceptance_probability = exp((-delta_distance) / temperature);
        _random = (rand() % 100000) / 100000.0;

        // printf("Temperature: %f, Best Distance: %d\n", temperature, distance);
        // printf("Acceptance probability: %f, Random: %f, Accepted: %s\n\n", acceptance_probability, _random, ((delta_distance < 0) | (acceptance_probability > _random)) ? "true" : "false");

        if ((delta_distance < 0) | (acceptance_probability > _random)) {
            memcpy(current_path, new_path, sizeof(new_path));
            distance = new_distance;
        }

        temperature *= cooling_rate;
        iteration += 1;
    }


    printf("==================== Iteration: %d ==================== \n", iteration);
    printf("Acceptance probability: %f, Random: %f\n", acceptance_probability, _random);
    printf("Iteration: %d, Distance: %d, Temperature: %f\n", iteration, distance, temperature);
    printf("Current path: ");
    for (int i = 0; i < path_length; i++) {
        printf("%d ", new_path[i]+1);
    }
    printf("\n======================================================== \n");
    // show the best seen path and cost
    printf("Best path: ");
    for (int i = 0; i < path_length; i++) {
        printf("%d ", best_path[i]+1);
    }
    printf("\n");
    printf("Best seen distance: %d\n", best_distance);
    printf("Best seen iteration: %d\n", best_iteration);
    printf("======================================================== \n");
    // set path to best path
    memcpy(path, current_path, sizeof(current_path));
    return distance;
}
#pragma endregion

#pragma region Main Function

int annealing(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance, double *_temperature, double *_cooling_rate, double *_absolute_temperature) {
	#pragma HLS TOP name=anneal
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=_number_of_cities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=_shortest_calculated_distance bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=_temperature bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=_cooling_rate bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=_absolute_temperature bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS
    
    int *_return_type;
    int return_type = -1;
    _return_type = &return_type;

	int output_cost;
	int num_cities = *_number_of_cities;
    
    // temperature = 100.0;
    // cooling_rate = 0.9999;
    // absolute_temperature = 1;
    double temperature = *_temperature;
    double cooling_rate = *_cooling_rate;
    double absolute_temperature = *_absolute_temperature;

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
    output_cost = nearest_neigbour_first(cache, num_cities, temperature, cooling_rate, absolute_temperature, _return_type, _shortest_calculated_distance);

    *_shortest_calculated_distance = output_cost;

    return return_type;
}

#pragma endregion
