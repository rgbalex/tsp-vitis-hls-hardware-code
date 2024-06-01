#include "brute_force_toplevel.h"
#include <stdio.h>

#pragma region Helper Functions

int index(int row, int col, int total_cities) {
#pragma HLS INLINE
    return row + total_cities * col;
}

int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[20]) {
    int cost = 0;
    path_cost_for_loop: for (int i = 0; i < num_cities; i++) {
    #pragma HLS LOOP_TRIPCOUNT max=20 min=4
    #pragma HLS PIPELINE
        // printf("City %d to %d: %d\n", path[i], path[i+1], adjacency_matrix[index(path[i], path[i+1], num_cities)]);
        cost += adjacency_matrix[index(path[i], path[i+1], num_cities)];
    }
    // printf("Total cost: %d\n", cost);   
    return cost;
}

void display(int *a, unsigned int j, unsigned int i, int num_cities)            
{
   for(unsigned int x = 0; x < num_cities; x++)
      printf("%d ",a[x+1]);
   printf("   swapped(%d, %d)\n", j, i);
}

#pragma endregion


#pragma region Main Function

int solve_the_tsp(uint32 *ram, int *_number_of_cities, int *_shortest_calculated_distance) {
	#pragma HLS TOP name=solve_the_tsp
    #pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
    #pragma HLS INTERFACE s_axilite port=_number_of_cities bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=_shortest_calculated_distance bundle=AXILiteS
    #pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS

	int num_cities = *_number_of_cities;

    int current_cost = INF;
    int current_best_cost = INF;
    int _current_best_path[20];
    #pragma HLS ARRAY_PARTITION variable=_current_best_path dim=1 complete
    memset(_current_best_path, -1, 20);

    // initialize current best path to ints ascending up to number of cities
    sample_path_loop: for (int i = 0; i < num_cities; i++) {
	#pragma HLS LOOP_TRIPCOUNT max=20 min=4
	#pragma HLS PIPELINE
        _current_best_path[i] = i;
    }
    _current_best_path[num_cities] = 0;

    uint8 cache[400];
    #pragma HLS ARRAY_PARTITION variable=cache dim=1 factor=4 cyclic
    // #pragma HLS ARRAY_PARTITION variable=cache dim=1 complete

    // enough space for a 20x20 matrix, initialized with zeros
    memcpy(cache, ram, 400*sizeof(uint8));

    #pragma region Print out all elements in cache
    // ================================================================
    // Print out all elements in cache
    print_loop_i: for (int i = 0; i < num_cities; i++) {
        print_loop_j: for (int j = 0; j < num_cities; j++) {
            printf("%3u ", cache[(num_cities * j) + i]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Number of cities: %d\n", num_cities);

    // printf("Current best path:  ");
    // print_current_best_path: for (int i = 0; i < num_cities+1; i++) {
    //     printf("%d ", _current_best_path[i]);
    // }
    // printf("\n");
    // ================================================================
    #pragma endregion

    *_shortest_calculated_distance = 0x7FFFFFFF;

    // brute force every possible path
    int a[20];
    #pragma HLS ARRAY_PARTITION variable=a dim=1 complete
    memcpy(a, _current_best_path, 20*sizeof(int));

    unsigned int p[20];
	#pragma HLS ARRAY_PARTITION variable=p dim=1 complete
    unsigned int i, j, tmp;

    initialise_p: for(i = 0; i < num_cities; i++) {
	    #pragma HLS LOOP_TRIPCOUNT max=20 min=4
	    #pragma HLS PIPELINE
        p[i] = i;
    }

    p[num_cities] = num_cities;
    //display(a, 0, 0);
    i = 1;

    // display(a, 0, i, num_cities-1);

    // calculate the cost of the current best path
    current_best_cost = path_cost_from_adjacency_matrix(cache, num_cities, _current_best_path);    
    
    run_quick_perm: while(i < num_cities-1) {
	#pragma HLS LOOP_FLATTEN off
        p[i]--;
        j = i % 2 * p[i];
        tmp = a[j+1];
        a[j+1] = a[i+1];
        a[i+1] = tmp;
    
        if (!(a[1] > a[num_cities-1])){
            // display(a, j, i, num_cities-1);
            current_cost = path_cost_from_adjacency_matrix(cache, num_cities, a);    
            if (current_cost < current_best_cost) {
                current_best_cost = current_cost;
                memcpy(_current_best_path, a, 20*sizeof(int));
            }
        }

        i = 1;

        int run = 1;
        update_p_array: while (run) {
		#pragma HLS LOOP_TRIPCOUNT max=20 min=4
		#pragma HLS PIPELINE
            if (p[i]) {
                run = 0;
            } else {
                p[i] = i;
                i++;
            }
        }
    }

    *_shortest_calculated_distance = current_best_cost;
    
    // copy the best path to the first 20 bytes of mainmemory
    memcpy(ram, _current_best_path, 20*sizeof(int));
    // since we already know this start value, and the number of cities requested,
    // this information can be reconstructed by the ARM core outside of vitis

    return -1;
}

#pragma endregion
