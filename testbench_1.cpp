#include "toplevel.h"
#include <stdio.h>
// for windows
// #include <strings.h>
// for linux
#include <string.h>
#include <cassert>

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

    printf("Hello, world!\n");

    _mainmemory[0] = 0x00B1FC12;
    _mainmemory[1] = 0xB10049FA;
    _mainmemory[2] = 0xFC4900BD;
    _mainmemory[3] = 0x12FABD00;

    toplevel(_mainmemory,&_messageId, &_numberOfCities, &_scenarioId);

    return 0;
}


int main() {
    assert(test_1() == 0);

    // printf("Tests completed successfuaslly (hopefully)\n\r");
    return 0;
}

