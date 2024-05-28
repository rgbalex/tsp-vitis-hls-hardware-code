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

    //minimum path is 129
    int CostGraphMatrix[5][5] = {   {17, 30, 33, 10, 25},
                                    {66, 22, 19, 15, 18},
                                    {89, 13, 8, 25, 15},
                                    {33, 25, 16, 30, 3},
                                    {25, 33, 35, 24, 37}};

    printf("\nTest 2\n\n");

    printf("Addresss:%d\n", &*&CostGraphMatrix[0][0]);
    int * temp = &*&CostGraphMatrix[0][0];
    printf("CostGraphMatrix[0][0]:%d\n", CostGraphMatrix[0][0]);
    *temp = 18;
    printf("CostGraphMatrix[0][0]:%d\n", CostGraphMatrix[0][0]);
    printf("CostGraphMatrix[0][0]:%d\n", CostGraphMatrix[0][1]);
    *(temp+1) = 18;
    printf("CostGraphMatrix[0][0]:%d\n", CostGraphMatrix[0][1]);

    int val = toplevel(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId);

    return 0;
}

int main() {
    assert(test_1() == 0);
    assert(test_2() == 0);

    // printf("Tests completed successfuaslly (hopefully)\n\r");
    return 0;
}

