#ifndef __TOPLEVEL_H_
#define __TOPLEVEL_H_

// Imports
#include <stdlib.h>

// Macros
#define CEIL_DIV(x, y) (((x) + (y) - 1) / (y))

//Typedefs
typedef unsigned int uint32;
typedef int int32;
typedef unsigned char uint8;

#pragma pack(push, 1)
// Structs
struct Request {
	uint8 message_id;
	uint8 number_of_cities;
	uint32 scenario_id;
};

struct Response {
	uint8  message_id;
	uint8  number_of_cities;
	uint32 scenario_id;
	uint8* adjacency_matrix;
	// The adjacency matrix is encoded as a list of unsigned bytes, 
	// encoding the distance between cities. 
	// A city is always 0 from itself and all distances are symmetric 
	// (distance(X,Y) == distance(Y,X)). 
};

struct Solve {
	uint8  message_id;
	uint8  number_of_cities;
	uint32 scenario_id;
	uint32 shortest_path; // big endian integer
};

struct Solve_Reply
{
	uint8  message_id;
	uint8  number_of_cities;
	uint32 scenario_id;
	uint8  answer; // 1 = correct, 2 = incorrect, 3 = unknown
};
#pragma pack(pop)

// for windows
// #define bzero(b,len) (memset((b), '\0', (len)), (void) 0)


// Prototypes
int toplevel(uint32 *ram, uint32 *message_id, uint32 *number_of_cities, uint32 *scenario_id);
int path_cost_from_adjacency_matrix(uint8 adjacency_matrix[], int num_cities, int path[], int path_length);

#endif
