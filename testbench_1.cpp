#include "toplevel.h"
#include <stdio.h>
// for windows
// #include <strings.h>
// for linux
#include <string.h>
#include <cassert>
#include <cstdio>

#define INF 0xFF

uint32 _mainmemory[8000];

int test_1() {
    uint32 _messageId = 0x01;
    uint32 _numberOfCities = 4;
    // unsure which, will test when in uni labs
    // uint32 _scenarioId = 0xc0000000; // big endian representation of 3
    uint32 _scenarioId = 0x00000003; // big endian representation of 3

    // An example scenario from the server is:
    // 
    // Size 4, ID 3:
    //
    // [  0  177  252   18]
    // [177    0   73  250]
    // [252   73    0  189]
    // [ 18  250  189    0]
    // Answer: 457

    printf("Test 1\n\n");

    //minimum path is 457
    int matrix[4][4] = { {   0, 177, 252,  18 },
                         { 177,   0,  73, 250 },
                         { 252,  73,   0, 189 },
                         {  18, 250, 189,   0 }   };

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

    // _mainmemory[0] = 0x00B1FC12;
    // _mainmemory[1] = 0xB10049FA;
    // _mainmemory[2] = 0xFC4900BD;
    // _mainmemory[3] = 0x12FABD00;

    _mainmemory[0] = 0x12FCB100;
    _mainmemory[1] = 0xFA4900B1;
    _mainmemory[2] = 0XBD0049FC;
    _mainmemory[3] = 0x00BDFA12;

    uint32 shortest_distance = 9999;
    int val = do_tsp(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId, &shortest_distance);
    assert(val == 457);
    assert(shortest_distance == 457);

    return 0;
}


int test_2() {
    uint32 _messageId = 0x01;
    uint32 _numberOfCities = 5;
    // unsure which, will test when in uni labs
    // uint32 _scenarioId = 0xc0000000; // big endian representation of 3

    uint32 _scenarioId = 0x00000015; // big endian representation of 3
    // n.b. scenario id is made up

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

    uint32 shortest_distance = 9999;
    int val = do_tsp(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId, &shortest_distance);
    assert(val == 77);
    assert(shortest_distance == 77);
    return 0;
}

int test_3() {
    uint32 _messageId = 0x01;
    uint32 _numberOfCities = 8;
    // unsure which, will test when in uni labs
    // uint32 _scenarioId = 0xc0000000; // big endian representation of 3

    uint32 _scenarioId = 0x00000025; // big endian representation of 3
    // n.b. scenario id is made up

    //minimum path is 330
    //nnf path is 395
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
            building = temp << (8 * (counter));

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

    uint32 shortest_distance = 9999;
    int val = do_tsp(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId, &shortest_distance);
    assert(val <= 400);
    assert(shortest_distance <= 360);

    return 0;
}

int test_4() {
    uint32 _messageId = 0x01;
    uint32 _numberOfCities = 12;
    // unsure which, will test when in uni labs
    // uint32 _scenarioId = 0xc0000000;
    uint32 _scenarioId = 0x00000021;
    // n.b. scenario id is made up

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
            temp = (char)(matrix[i][j]);

            // left shift temp into correct position w.r.t counter
            building = temp << (8 * (counter));

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

    uint32 shortest_distance = 9999;
    int val = do_tsp(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId, &shortest_distance);
    assert(val == 134);
    assert(shortest_distance == 134);


    return 0;
}

int main() {
    assert(test_1() == 0);
    assert(test_2() == 0);
    // note: the above two tests can be solved by nnf
    // larger solutions would need to be solved properly. 
    assert(test_3() == 0);
    // greedy algorithm works but not optimal solution
    assert(test_4() == 0);

    // printf("Tests completed successfuaslly (hopefully)\n\r");
    return 0;
}

