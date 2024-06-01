#include "kswaps_toplevel.h"

#include <stdio.h>
#include <cassert>
#include <cstdlib>

uint32* _mainmemory;

double _temperature = 100000.0;
double _cooling_rate = 0.999999;
double _absolute_temperature = 0.00001;

void test_1() {
    int _numberOfCities = 4;
    int _shortestCalculatedDistance = 9999;

    _mainmemory[0] = 0x12FCB100;
    _mainmemory[1] = 0xFA4900B1;
    _mainmemory[2] = 0XBD0049FC;
    _mainmemory[3] = 0x00BDFA12;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);

    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

void test_2() {
    int _numberOfCities = 5;
    int _shortestCalculatedDistance = 9999;

    int matrix[5][5] = {
        { 0, 30, 33, 10, 25 },
        { 30, 0, 19, 15, 18 },
        { 33, 19, 0, 25, 15 },
        { 10, 15, 25, 0, 3 },
        { 25, 18, 15, 3, 0 }
    };

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            temp = (char)(matrix[i][j]);
            building = temp << (8 * counter);
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index++;
            } else {
                counter++;
            }
        }
    }
    _mainmemory[index] = output;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);
    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

void test_3() {
    int _numberOfCities = 8;
    int _shortestCalculatedDistance = 9999;

    int matrix[8][8] = {
        { 0, 10, 15, 20, 25, 30, 35, 40 },
        { 10, 0, 35, 25, 30, 35, 40, 45 },
        { 15, 35, 0, 30, 35, 40, 45, 50 },
        { 20, 25, 30, 0, 45, 50, 55, 60 },
        { 25, 30, 35, 45, 0, 65, 70, 75 },
        { 30, 35, 40, 50, 65, 0, 85, 90 },
        { 35, 40, 45, 55, 70, 85, 0, 105 },
        { 40, 45, 50, 60, 75, 90, 105, 0 }
    };

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            temp = (char)(matrix[i][j]);
            building = temp << (8 * counter);
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index++;
            } else {
                counter++;
            }
        }
    }
    _mainmemory[index] = output;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);
    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

void test_4() {
    int _numberOfCities = 12;
    int _shortestCalculatedDistance = 9999;

    int matrix[12][12] = {
        { 0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60 },
        { 10, 0, 12, 18, 22, 28, 32, 38, 42, 48, 52, 58 },
        { 15, 12, 0, 8, 14, 20, 24, 30, 34, 40, 44, 50 },
        { 20, 18, 8, 0, 6, 12, 16, 22, 26, 32, 36, 42 },
        { 25, 22, 14, 6, 0, 8, 12, 18, 22, 28, 32, 38 },
        { 30, 28, 20, 12, 8, 0, 4, 10, 14, 20, 24, 30 },
        { 35, 32, 24, 16, 12, 4, 0, 6, 10, 16, 20, 26 },
        { 40, 38, 30, 22, 18, 10, 6, 0, 4, 10, 14, 20 },
        { 45, 42, 34, 26, 22, 14, 10, 4, 0, 6, 10, 16 },
        { 50, 48, 40, 32, 28, 20, 16, 10, 6, 0, 4, 10 },
        { 55, 52, 44, 36, 32, 24, 20, 14, 10, 4, 0, 6 },
        { 60, 58, 50, 42, 38, 30, 26, 20, 16, 10, 6, 0 }
    };

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    char temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            temp = (char)(matrix[i][j]);
            building = temp << (8 * counter);
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index++;
            } else {
                counter++;
            }
        }
    }
    _mainmemory[index] = output;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);
    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

void test_5() {
    int _numberOfCities = 6;
    int _shortestCalculatedDistance = 9999;

    int matrix[6][6] = {
        { 0, 10, 104, 34, 26, 139 },
        { 10, 0, 140, 116, 159, 180 },
        { 104, 140, 0, 164, 24, 100 },
        { 34, 116, 164, 0, 126, 117 },
        { 26, 159, 24, 126, 0, 14 },
        { 139, 180, 100, 117, 14, 0 }
    };

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    uint8 temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            temp = matrix[i][j];
            building = temp << (8 * counter);
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index++;
            } else {
                counter++;
            }
        }
    }
    _mainmemory[index] = output;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);
    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

void test_6() {
    int _numberOfCities = 6;
    int _shortestCalculatedDistance = 9999;

    int matrix[10][10] = {
        { 0, 76, 228, 155, 196, 208, 161, 145, 163, 69 },
        { 76, 0, 221, 120, 12, 60, 195, 96, 23, 215 },
        { 228, 221, 0, 164, 215, 90, 4, 14, 213, 203 },
        { 155, 120, 164, 0, 30, 109, 148, 243, 10, 254 },
        { 196, 12, 215, 30, 0, 40, 225, 137, 250, 135 },
        { 208, 60, 90, 109, 40, 0, 112, 170, 210, 58 },
        { 161, 195, 4, 148, 225, 112, 0, 21, 42, 61 },
        { 145, 96, 14, 243, 137, 170, 21, 0, 56, 168 },
        { 163, 23, 213, 10, 250, 210, 42, 56, 0, 20 },
        { 69, 215, 203, 254, 135, 58, 61, 168, 20, 0 }
    };

    int counter = 0;
    int index = 0;
    uint32 output = 0;
    uint32 building = 0;
    uint8 temp;
    for (int i = 0; i < _numberOfCities; i++) {
        for (int j = 0; j < _numberOfCities; j++) {
            temp = matrix[i][j];
            building = temp << (8 * counter);
            output = output | building;

            if (counter == 3) {
                _mainmemory[index] = output;
                counter = 0;
                building = 0;
                output = 0;
                index++;
            } else {
                counter++;
            }
        }
    }
    _mainmemory[index] = output;

    int result = solve_the_tsp(_mainmemory, &_numberOfCities, &_shortestCalculatedDistance);
    printf("\nDistance: %d, Return Type: %s\n", _shortestCalculatedDistance, result == 1 ? "Greedy" : "Hardware");
}

int main() {
    _mainmemory = (uint32*)calloc(8000, sizeof(uint32));
    // printf("Test 1\n");
    // test_1();
    // // should be 457
    // printf("Should be 457\n\n");
    // printf("Test 2\n");
    // test_2();
    // // should be 77
    // printf("Should be 77\n\n");
    printf("Test 3\n");
    test_3();
    // should be 330
    printf("Should be 330\n\n");
    // printf("Test 4\n");
    // test_4();
    // // should be 134
    // printf("Should be 134\n\n");
    // printf("Test 5\n");
    // test_5();
    // // i think its 339
    // printf("Should be 339\n\n");
    // printf("Test 6\n");
    // test_6();
    // // i think its 420
    // printf("Should be 420\n\n");
    // printf("\n");

    return 0;
}
