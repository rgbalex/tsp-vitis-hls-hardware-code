#include "toplevel.h"

#include <stdio.h>

#include <cstdint>
#include <cstring> // Include the <cstring> header for memcpy

#include <vector>
#define INF 0xFF

// Main Function
int listener(uint32 *ram, uint32 *message_id, uint32 *number_of_cities, uint32 *scenario_id) {
	#pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
	#pragma HLS INTERFACE s_axilite port=messageId bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=numberOfCities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=scenarioId bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

	uint8 char_number_of_cities = (char)(*number_of_cities >> 24);

	// Initialize struct with given values
    Request params = {
        .message_id = 0x01,
        .number_of_cities = char_number_of_cities, // or any value between 4 and 20
        .scenario_id = *scenario_id, // big endian representation of 3
    };
	
	return 0;
}

int index(int row, int col, int total_cities) {
	return row + total_cities * col;
}

int nearest_neigbour_first (uint8 adjacency_matrix[], int num_cities) {
    int run = 1;
    std::vector<int> visited_cities;
    
    int distance = INF;
    int city_pointer = 0;
    int min_distance = INF;
    int min_city = -1;
    
    int skip = 0;
    int worst_case_distance = INF;
    
    // start at city 0
    visited_cities.push_back(0);

    while (run) {

        // get the last city visited
        for (int i = 0; i < num_cities; i++) {
            skip = 0;
            city_pointer = visited_cities.back();
            distance = adjacency_matrix[index(city_pointer, i, num_cities)];
            
            if (distance == 0) { continue; }

            for (int j = 0; j < visited_cities.size(); j++) {
                if (visited_cities[j] == i) {
                    skip = 1;
                }
            }

            if (skip) { continue; }

            if (distance < min_distance) {
                min_distance = distance;
                min_city = i;
            }
        }
        

        if (visited_cities.size() == num_cities) {
            run = 0;
        } else {
            // printf("City: %d, Distance: %d\n", min_city, min_distance); 
            visited_cities.push_back(min_city);
            worst_case_distance += min_distance;
            min_distance = INF;
        }
    }

    // finish with a loop back to city 0
    worst_case_distance += adjacency_matrix[index(visited_cities.back(), 0, num_cities)];

    // print out the visited_cities vector as this contains your nearest neigbour tour
    for (int i = 0; i < visited_cities.size(); i++) { printf("%d ", visited_cities[i]); }

    printf("\n");
    printf("Your upper bound for distance is %d", worst_case_distance);
    
    return 0;
}

// Main Function - solver
int toplevel(uint32 *ram, uint32 *message_id, uint32 *number_of_cities, uint32 *scenario_id) {
	#pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
	#pragma HLS INTERFACE s_axilite port=message_id bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=number_of_cities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=scenario_id bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS
		
	uint8 cache[400]; // enough space for a 20x20 matrix

	uint8 char_number_of_cities = (char)(*number_of_cities >> 24);
	int int_number_of_cities = *number_of_cities;
	uint32 shortest_path = -1;

	// Initialize struct with given values
    Solve params = {
        .message_id = 0x01,
        .number_of_cities = char_number_of_cities, // or any value between 4 and 20
        .scenario_id = *scenario_id, // big endian representation of 3
    };

	// Build the problem
	memcpy(cache, ram, sizeof(cache));

	// Print out all elements in cache
	for (int i = 0; i < int_number_of_cities; i++) {
		for (int j = 0; j < int_number_of_cities; j++) {
			printf("%d ", cache[index(i, j, int_number_of_cities)]);
		}
		printf("\n");
	}
    printf("\n");

	// Rest of the code...
    // Nearest neigbour first soln
    nearest_neigbour_first(cache, int_number_of_cities);


	// Finish up - add the shortest path
	params.shortest_path = shortest_path;
	return shortest_path;
}
