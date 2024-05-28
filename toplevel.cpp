#include "toplevel.h"

#include <stdio.h>

#include <cstdint>
#include <cstring> // Include the <cstring> header for memcpy

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

	// Rest of the code...
    // Nearest neigbour first soln

	// Finish up - add the shortest path
	params.shortest_path = shortest_path;
	return shortest_path;
}
