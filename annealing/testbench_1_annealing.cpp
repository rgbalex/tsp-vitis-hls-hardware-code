#include "annealing_toplevel.h"

#include <stdio.h>
#include <cassert>
#include <cstdlib> // Include the <cstdlib> header

uint32* _mainmemory; // Declare _mainmemory as a global variable

int _max_anneal = 1000;

void test_1(){
    int _numberOfCities = 4;
    int _shortestCalculatedDistance = 9999;

    _mainmemory[0] = 0x12FCB100;
    _mainmemory[1] = 0xFA4900B1;
    _mainmemory[2] = 0XBD0049FC;
    _mainmemory[3] = 0x00BDFA12;

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance, &_max_anneal);

    assert(result == 99);
    assert(_shortestCalculatedDistance == 457);
}

int test_2() {
    int _numberOfCities = 5;
    int _shortestCalculatedDistance = 9999;

    //minimum path is 77
    int matrix[5][5] = {    { 0, 30, 33, 10, 25},
                            {30,  0, 19, 15, 18},
                            {33, 19,  0, 25, 15},
                            {10, 15, 25,  0,  3},
                            {25, 18, 15,  3,  0} };

    printf("\nTest 2\n\n");

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {

            // left shift the char out of the int
            temp = (char)(matrix[i][j]);
            temp = (char)(matrix[i][j]);

            // left shift temp into correct position w.r.t counter
            building = temp << (8 * counter);

            // or into output
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index ++;
            } else {
                counter ++;
            }

        }
    }
    _mainmemory[index] = output;

    assert(_mainmemory[0] == 0x0A211E00);
    assert(_mainmemory[1] == 0x13001E19);
    assert(_mainmemory[2] == 0x1321120F);
    assert(_mainmemory[3] == 0x0A0F1900);
    assert(_mainmemory[4] == 0x0300190F);
    assert(_mainmemory[5] == 0x030F1219);
    assert(_mainmemory[6] == 0); // the missing value

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance, &_max_anneal);
    assert(result == 99);
    assert(_shortestCalculatedDistance == 77);
    return 0;
}

int test_3() {
    int _numberOfCities = 8;
    int _shortestCalculatedDistance = 9999;

    //minimum path is 77
    int matrix[8][8] = {    { 0 , 10, 15, 20, 25, 30, 35,  40  },
                            { 10, 0 , 35, 25, 30, 35, 40,  45  },
                            { 15, 35, 0 , 30, 35, 40, 45,  50  },
                            { 20, 25, 30, 0 , 45, 50, 55,  60  },
                            { 25, 30, 35, 45, 0 , 65, 70,  75  },
                            { 30, 35, 40, 50, 65, 0 , 85,  90  },
                            { 35, 40, 45, 55, 70, 85, 0 ,  105 },
                            { 40, 45, 50, 60, 75, 90, 105, 0   }    };
    printf("\nTest 3\n\n");

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {

            // left shift the char out of the int
            temp = (char)(matrix[i][j]);
            temp = (char)(matrix[i][j]);

            // left shift temp into correct position w.r.t counter
            building = temp << (8 * counter);

            // or into output
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index ++;
            } else {
                counter ++;
            }

        }
    }
    _mainmemory[index] = output;

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance, &_max_anneal);
    assert(result == 99);
    // sanity check
    assert(_shortestCalculatedDistance <= 400);
    // past here, nnf is working
    assert(_shortestCalculatedDistance <= 350);
    // past here, some annealing is working
    assert(_shortestCalculatedDistance == 330);
    // correct solution
    return 0;
}

int test_4() {
    int _numberOfCities = 12;
    int _shortestCalculatedDistance = 9999;

    // minimum path is 310 according to copilot
    // after running, i think it is 134-5
    int matrix[12][12] = {
        {0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60},
        {10, 0, 12, 18, 22, 28, 32, 38, 42, 48, 52, 58},
        {15, 12, 0, 8, 14, 20, 24, 30, 34, 40, 44, 50},
        {20, 18, 8, 0, 6, 12, 16, 22, 26, 32, 36, 42},
        {25, 22, 14, 6, 0, 8, 12, 18, 22, 28, 32, 38},
        {30, 28, 20, 12, 8, 0, 4, 10, 14, 20, 24, 30},
        {35, 32, 24, 16, 12, 4, 0, 6, 10, 16, 20, 26},
        {40, 38, 30, 22, 18, 10, 6, 0, 4, 10, 14, 20},
        {45, 42, 34, 26, 22, 14, 10, 4, 0, 6, 10, 16},
        {50, 48, 40, 32, 28, 20, 16, 10, 6, 0, 4, 10},
        {55, 52, 44, 36, 32, 24, 20, 14, 10, 4, 0, 6},
        {60, 58, 50, 42, 38, 30, 26, 20, 16, 10, 6, 0}
    };
    printf("\nTest 4\n\n");

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {

            // left shift the char out of the int
            temp = (char)(matrix[i][j]);

            // left shift temp into correct position w.r.t counter
            building = temp << (8 * counter);

            // or into output
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index ++;
            } else {
                counter ++;
            }

        }
    }
    _mainmemory[index] = output;

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance, &_max_anneal);
    assert(result == 99);
    assert(_shortestCalculatedDistance <= 400);
    assert(_shortestCalculatedDistance <= 300);
    assert(_shortestCalculatedDistance <= 200);
    // past here, some annealing is working
    // assert(_shortestCalculatedDistance == 134);
    // correct solution?
    return 0;
}

int test_5() {
    int _numberOfCities = 6;
    int _shortestCalculatedDistance = 9999;
    int _local_max_anneal = 10000;

    int matrix[6][6] = {
        {0,   10,  104,  34,   26,  139},
        {10,   0,   140,  116,  159,  180},
        {104, 140,   0,   164,   24,  100},
        {34,  116,  164,   0,   126,  117},
        {26,  159,   24,  126,   0,    14},
        {139, 180,  100,  117,  14,    0}
    };
    printf("\nTest 5\n\n");

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    uint8 temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            // printf("Index: %d\n", index);
            // left shift the char out of the int   
            temp = matrix[i][j];
            // print the binary of temp
            // printf("Temp: %x\n", temp);

            // left shift temp into correct position w.r.t counter
            building = temp << (unsigned) (8 * counter);
            // printf("Building: %x\n", building);


            // or into output
            output = (unsigned) output | (unsigned) building;
            // printf("Output: %x\n", output);

            if (counter == 3) {
                // print output before writing
                // printf("        Output before writing: %x\n", output);
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index ++;
            } else {
                counter ++;
            }

        }
    }
    _mainmemory[index] = output;

    char *main_mem_ptr = (char *)_mainmemory;
    // Print out the mainmemory as integers
    // printf("Main Memory:\n");
    // for (int i = 0; i < _numberOfCities; i++) {
    //     for (int j = 0; j < _numberOfCities; j++) {
    //         printf("%d ", main_mem_ptr[i * _numberOfCities + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // printf("Main Memory:\n");
    // for (int i = 0; i < _numberOfCities; i++) {
    //     for (int j = 0; j < _numberOfCities; j++) {
    //         printf("%x ", (unsigned) main_mem_ptr[i * _numberOfCities + j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // // print out mainmemory as hex without pointer
    // printf("Main Memory:\n");
    // for (int i = 0; i < _numberOfCities; i++) {
    //     for (int j = 0; j < _numberOfCities; j++) {
    //         printf("%x ", (unsigned) _mainmemory[i * _numberOfCities + j]);
    //     }
    //     printf("\n");
    // }

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance, &_local_max_anneal);
    assert(result == 99);
    printf("\n");

    // assert(_shortestCalculatedDistance <= 400);
    // assert(_shortestCalculatedDistance <= 385);
    // assert(_shortestCalculatedDistance <= 200);
    // past here, some annealing is working
    // assert(_shortestCalculatedDistance == 134);
    // correct solution?
    return _shortestCalculatedDistance;
}

int main() {
    // calloc _mainmemory
    _mainmemory = (uint32 *)calloc(8000, sizeof(uint32));
	// test_1();
    // test_2();
    // test_3();
    // test_4();
    uint32 out[10];
    // run test_5 10 times, 
    for (int i = 0; i < 10; i++) {
        // collect and print results
        
        out[i] = test_5();;
    }

    // print out results
    printf("Results: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", out[i]);
    }

    return 0;
}
