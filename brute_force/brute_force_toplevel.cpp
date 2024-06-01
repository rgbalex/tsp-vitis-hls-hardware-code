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
    // print total cost
    // printf("Total cost: %d\n", cost);   
    return cost;
}

void display(int *a, unsigned int j, unsigned int i, int num_cities)            
{
   for(unsigned int x = 0; x < num_cities; x++)
      printf("%d ",a[x+1]);
   printf("   swapped(%d, %d)\n", j, i);
//    getch();  // press any key to continue...
} // display()

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

    uint8 cache[400]; // enough space for a 20x20 matrix, initialized with zeros
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

    // calculate the cost of the current best path
    
    // brute force every possible path
    int a[20];
    #pragma HLS ARRAY_PARTITION variable=a dim=1 complete
    memcpy(a, _current_best_path, 20*sizeof(int));

    unsigned int p[20];
#pragma HLS ARRAY_PARTITION variable=p dim=1 complete
    unsigned int i, j, tmp; // Upper Index i; Lower Index j

    initialise_p: for(i = 0; i < num_cities; i++)   // initialize arrays; a[N] can be any type
    {
#pragma HLS LOOP_TRIPCOUNT max=20 min=4
	#pragma HLS PIPELINE
        // a[i] = i + 1;   // a[i] value is not revealed and can be arbitrary
        p[i] = i;
    }
    p[num_cities] = num_cities; // p[N] > 0 controls iteration and the index boundary for i
    //display(a, 0, 0);   // remove comment to display array a[]
    i = 1;   // setup first swap points to be 1 and 0 respectively (i & j)

    // display(a, 0, i, num_cities-1);
    current_best_cost = path_cost_from_adjacency_matrix(cache, num_cities, _current_best_path);    
    
    run_quick_perm: while(i < num_cities-1) {
	#pragma HLS LOOP_FLATTEN off
	#pragma HLS LOOP_TRIPCOUNT max=19 min=3
        p[i]--;             // decrease index "weight" for i by one
        j = i % 2 * p[i];   // IF i is odd then j = p[i] otherwise j = 0
        tmp = a[j+1];         // swap(a[j], a[i])
        a[j+1] = a[i+1];
        a[i+1] = tmp;
    
        if (!(a[1] > a[num_cities-1])){
            // display(a, j, i, num_cities-1); // remove comment to display target array a[]
            current_cost = path_cost_from_adjacency_matrix(cache, num_cities, a);    
            if (current_cost < current_best_cost) {
                current_best_cost = current_cost;
            }
        }

        i = 1;              // reset index i to 1 (assumed)
        // while (!p[i])       // while (p[i] == 0)
        int run = 1;
        // while (!p[i])       // while (p[i] == 0)
        update_p_array: while (run)
        {
		#pragma HLS LOOP_TRIPCOUNT max=20 min=4
		#pragma HLS PIPELINE
            if (p[i]) {
                run = 0;
            } else {
                p[i] = i;        // reset p[i] zero value
                i++;             // set new index value for i (increase by one)
            }
        } // while(!p[i])
    } // while(i < N)

    *_shortest_calculated_distance = current_best_cost;

    return -1;
}

#pragma endregion
