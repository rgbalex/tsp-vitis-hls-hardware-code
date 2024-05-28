#include "toplevel.h"

#include <stdio.h>

#include <cstdint>
#include <cstring> // Include the <cstring> header for memcpy

#include <bits/stdc++.h>
using namespace std;
#define V 4

bool isValidEdge(int u, int v, vector<bool> inMST)
{
   if (u == v)
       return false;
   if (inMST[u] == false && inMST[v] == false)
        return false;
   else if (inMST[u] == true && inMST[v] == true)
        return false;
   return true;
}

void primMST(uint8 cost[][V])
{  
    vector<bool> inMST(V, false);
 
    // Include first vertex in MST
    inMST[0] = true;
 
    // Keep adding edges while number of included
    // edges does not become V-1.
    int edge_count = 0, mincost = 0;
    while (edge_count < V - 1) {
 
        // Find minimum weight valid edge.  
        int min = INT_MAX, a = -1, b = -1;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {               
                if (cost[i][j] < min) {
                    if (isValidEdge(i, j, inMST)) {
                        min = cost[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
        }
        if (a != -1 && b != -1) {
            printf("Edge %d:(%d, %d) cost: %d \n", 
                         edge_count++, a, b, min);
            mincost = mincost + min;
            inMST[b] = inMST[a] = true;
        }
    }
    printf("\n Minimum cost= %d \n", mincost);
}

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
	primMST(*cache);

	// Finish up - add the shortest path
	params.shortest_path = shortest_path;
	return shortest_path;
}
