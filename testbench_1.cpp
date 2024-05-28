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

    // _mainmemory[0] = 0x00B1FC12;
    // _mainmemory[1] = 0xB10049FA;
    // _mainmemory[2] = 0xFC4900BD;
    // _mainmemory[3] = 0x12FABD00;

    _mainmemory[0] = 0x12FCB100;
    _mainmemory[1] = 0xFA4900B1;
    _mainmemory[2] = 0XBD0049FC;
    _mainmemory[3] = 0x00BDFA12;

    int val = toplevel(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId);
    assert(val == 457);

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
    uint32 output;
    uint32 building;
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

    assert(_mainmemory[0] == 0x0A211E00);
    assert(_mainmemory[1] == 0x13001E19);
    assert(_mainmemory[2] == 0x1321120F);
    assert(_mainmemory[3] == 0x0A0F1900);
    assert(_mainmemory[4] == 0x0300190F);
    assert(_mainmemory[5] == 0x030F1219);
    assert(_mainmemory[6] == 0); // the missing value

    int val = toplevel(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId);
    assert(val == 77);

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
    int matrix[8][8] = {   { 0 , 10, 15, 20, 25, 30, 35, 40  },
                                    { 10, 0 , 35, 25, 30, 35, 40, 45  },
                                    { 15, 35, 0 , 30, 35, 40, 45, 50  },
                                    { 20, 25, 30, 0 , 45, 50, 55, 60  },
                                    { 25, 30, 35, 45, 0 , 65, 70, 75  },
                                    { 30, 35, 40, 50, 65, 0 , 85, 90  },
                                    { 35, 40, 45, 55, 70, 85, 0 , 105 },
                                    { 40, 45, 50, 60, 75, 90, 105, 0  } };

    printf("\nTest 3\n\n");

    int counter = 0;
    int index = 0;
    uint32 output;
    uint32 building;
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

    int val = toplevel(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId);
    // assert(val == 330);
    assert(val == 395);

    

    return 0;
}

int main() {
    assert(test_1() == 0);
    assert(test_2() == 0);
    assert(test_3() == 0);
    // note: the above two tests can be solved by nnf
    // larger solutions would need to be solved properly. 

    // printf("Tests completed successfuaslly (hopefully)\n\r");
    return 0;
}

