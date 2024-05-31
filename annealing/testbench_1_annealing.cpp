#include "annealing_toplevel.h"

#include <stdio.h>
#include <cassert>

uint32 _mainmemory[8000];

void test_1(){
    int _messageId = 0x01;
    int _numberOfCities = 4;
    int _scenarioId = 0x00000003;
    int _shortestCalculatedDistance = 9999;

    _mainmemory[0] = 0x12FCB100;
    _mainmemory[1] = 0xFA4900B1;
    _mainmemory[2] = 0XBD0049FC;
    _mainmemory[3] = 0x00BDFA12;

    int result = annealing(_mainmemory, &_numberOfCities,  &_shortestCalculatedDistance);

    assert(result == 99);
    assert(_shortestCalculatedDistance == 457);
}
int main() {
	test_1();

    return 0;
}
