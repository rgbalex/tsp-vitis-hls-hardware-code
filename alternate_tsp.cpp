// C++ code to implement the approach
#include "toplevel.h"
#include <bits/stdc++.h>
#include <vector>
using namespace std;

// N is the number of cities/Node given
#define N 8
#define INF INT_MAX

// Structure to store all the necessary information 
// to form state space tree
struct Node {
    
    // Helps in tracing the path when the answer is found
    // stores the edges of the path 
    // completed till current visited node
    vector<pair<int, int> > path;

    // Stores the reduced matrix
    int reducedMatrix[N][N];

    // Stores the lower bound
    int cost;

    // Stores the current city number
    int vertex;
  
    // Stores the total number of cities visited
    int level;
};

// Formation of edges and assigning 
// all the necessary information for new node
Node* newNode(int parentMatrix[N][N],
              vector<pair<int, int> > const& path,
              int level, int i, int j)
{
    Node* node = new Node;
    
    // Stores parent edges of the state-space tree
    node->path = path;

    // Skip for the root node
    if (level != 0) {
        
        // Add a current edge to the path
        node->path.push_back(make_pair(i, j));
    }

    // Copy data from the parent node to the current node
    memcpy(node->reducedMatrix, parentMatrix,
           sizeof node->reducedMatrix);

    // Change all entries of row i and column j to INF
    // skip for the root node
    for (int k = 0; level != 0 && k < N; k++) {
        
        // Set outgoing edges for the city i to INF
        node->reducedMatrix[i][k] = INF;
        
        // Set incoming edges to city j to INF
        node->reducedMatrix[k][j] = INF;
    }

    // Set (j, 0) to INF
    // here start node is 0
    node->reducedMatrix[j][0] = INF;

    // Set number of cities visited so far
    node->level = level;

    // Assign current city number
    node->vertex = j;

    // Return node
    return node;
}

// Function to reduce each row so that 
// there must be at least one zero in each row
int rowReduction(int reducedMatrix[N][N], 
                 int row[N])
{
    // Initialize row array to INF
    fill_n(row, N, INF);

    // row[i] contains minimum in row i
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] < row[i]) {
                row[i] = reducedMatrix[i][j];
            }
        }
    }

    // Reduce the minimum value from each element 
    // in each row
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] != INF
                && row[i] != INF) {
                reducedMatrix[i][j] -= row[i];
            }
        }
    }
    return 0;
}

// Function to reduce each column so that 
// there must be at least one zero in each column
int columnReduction(int reducedMatrix[N][N], 
                    int col[N])
{
    // Initialize all elements of array col with INF
    fill_n(col, N, INF);

    // col[j] contains minimum in col j
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] < col[j]) {
                col[j] = reducedMatrix[i][j];
            }
        }
    }
    // Reduce the minimum value from each element 
    // in each column
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] != INF
                && col[j] != INF) {
                reducedMatrix[i][j] -= col[j];
            }
        }
    }
    return 0;
}

// Function to get the lower bound on the path 
// starting at the current minimum node
int calculateCost(int reducedMatrix[N][N])
{
    // Initialize cost to 0
    int cost = 0;

    // Row Reduction
    int row[N];
    rowReduction(reducedMatrix, row);

    // Column Reduction
    int col[N];
    columnReduction(reducedMatrix, col);

    // The total expected cost is 
    // the sum of all reductions
    for (int i = 0; i < N; i++) {
        cost += (row[i] != INT_MAX) ? row[i] : 0,
            cost += (col[i] != INT_MAX) ? col[i] : 0;
    }
    return cost;
}

// Function to print list of cities 
// visited following least cost
void TSPPAthPrint(vector<pair<int, int> > const& list)
{
    for (int i = 0; i < list.size(); i++) {
        cout << list[i].first + 1 << " -> "
             << list[i].second + 1 << "\n";
    }
}

// Comparison object to be used to order the heap
struct Min_Heap {
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return lhs->cost > rhs->cost;
    }
};

// Function to solve the traveling salesman problem 
// using Branch and Bound
int solve(int CostGraphMatrix[N][N])
{
    // Create a priority queue to store live nodes 
    // of the search tree
    priority_queue<Node*, vector<Node*>, Min_Heap> pq;
    vector<pair<int, int> > v;

    // Create a root node and calculate its cost.
    // The TSP starts from the first city, i.e., node 0
    Node* root = newNode(CostGraphMatrix, v, 0, -1, 0);

    // Get the lower bound of the path 
    // starting at node 0
    root->cost = calculateCost(root->reducedMatrix);

    // Add root to the list of live nodes
    pq.push(root);

    // Finds a live node with the least cost, 
    // adds its children to the list of live nodes, 
    // and finally deletes it from the list
    while (!pq.empty()) {
        
        // Find a live node with uint32 _mainmemory[8000];

        // the least estimated cost
        Node* min = pq.top();

        // The found node is deleted from 
        // the list of live nodes
        pq.pop();

        // i stores the current city number
        int i = min->vertex;
        
        // If all cities are visited
        if (min->level == N - 1) {
            
            // Return toh starting city
            min->path.push_back(make_pair(i, 0));
            
            // Print list of cities visited
            TSPPAthPrint(min->path);
            
            // Return optimal cost
            return min->cost;
        }

        // Do for each child of min
        // (i, j) forms an edge in a space tree
        for (int j = 0; j < N; j++) {
            if (min->reducedMatrix[i][j] != INF) {
                
                // Create a child node and 
                // calculate its cost
                Node* child
                    = newNode(min->reducedMatrix, min->path,
                              min->level + 1, i, j);

                child->cost
                    = min->cost + min->reducedMatrix[i][j]
                      + calculateCost(child->reducedMatrix);
                
                // Add a child to the list of live nodes
                pq.push(child);
            }
        }
      
        // Free node as we have already stored edges (i, j)
        // in vector. So no need for a parent node while
        // printing the solution.
        delete min;
    }
    return 0;
}

// Driver code
int toplevel(uint32 *ram, uint32 *message_id, uint32 *number_of_cities, uint32 *scenario_id) {
	#pragma HLS INTERFACE m_axi port=ram offset=slave bundle=MAXI
	#pragma HLS INTERFACE s_axilite port=message_id bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=number_of_cities bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=scenario_id bundle=AXILiteS
	#pragma HLS INTERFACE s_axilite port=return bundle=AXILiteS
    // int CostGraphMatrix[N][N] = { { INF, 10, 15, 20 },
    //                               { 10, INF, 35, 25 },
    //                               { 15, 35, INF, 30 },
    //                               { 20, 25, 30, INF } };

    // int CostGraphMatrix[N][N] = {   { INF, 30, 33, 10, 25},
    //                                 {30,  INF, 19, 15, 18},
    //                                 {33, 19,  INF, 25, 15},
    //                                 {10, 15, 25,  INF,  3},
    //                                 {25, 18, 15,  3,  INF}};

    // now for an 8x8 matrix
    int CostGraphMatrix[N][N] = {   { INF, 10, 15, 20, 25, 30, 35, 40 },
                                    { 10, INF, 35, 25, 30, 35, 40, 45 },
                                    { 15, 35, INF, 30, 35, 40, 45, 50 },
                                    { 20, 25, 30, INF, 45, 50, 55, 60 },
                                    { 25, 30, 35, 45, INF, 65, 70, 75 },
                                    { 30, 35, 40, 50, 65, INF, 85, 90 },
                                    { 35, 40, 45, 55, 70, 85, INF, 105 },
                                    { 40, 45, 50, 60, 75, 90, 105, INF } 
                                };     

    // int CostGraphMatrix[N][N] = {
    //     {0,   10,  104,  34,   26,  139},
    //     {10,   0,   140,  116,  159,  180},
    //     {104, 140,   0,   164,   24,  100},
    //     {34,  116,  164,   0,   126,  117},
    //     {26,  159,   24,  126,   0,    14},
    //     {139, 180,  100,  117,  14,    0} 
    // };
    
    // int CostGraphMatrix[10][10] = {
    //     { 0,  76, 228, 155, 196, 208, 161, 145, 163,  69 },
    //     { 76,   0, 221, 120,  12,  60, 195,  96,  23, 215 },
    //     { 228, 221,   0, 164, 215,  90,   4,  14, 213, 203 },
    //     { 155, 120, 164,   0,  30, 109, 148, 243,  10, 254 },
    //     { 196,  12, 215,  30,   0,  40, 225, 137, 250, 135 },
    //     { 208,  60,  90, 109,  40,   0, 112, 170, 210,  58 },
    //     { 161, 195,   4, 148, 225, 112,   0,  21,  42,  61 },
    //     { 145,  96,  14, 243, 137, 170,  21,   0,  56, 168 },
    //     { 163,  23, 213,  10, 250, 210,  42,  56,   0,  20 },
    //     {  69, 215, 203, 254, 135,  58,  61, 168,  20,   0 }
    // };

    // int CostGraphMatrix[12][12] = {
    //     {0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60},
    //     {10, 0, 12, 18, 22, 28, 32, 38, 42, 48, 52, 58},
    //     {15, 12, 0, 8, 14, 20, 24, 30, 34, 40, 44, 50},
    //     {20, 18, 8, 0, 6, 12, 16, 22, 26, 32, 36, 42},
    //     {25, 22, 14, 6, 0, 8, 12, 18, 22, 28, 32, 38},
    //     {30, 28, 20, 12, 8, 0, 4, 10, 14, 20, 24, 30},
    //     {35, 32, 24, 16, 12, 4, 0, 6, 10, 16, 20, 26},
    //     {40, 38, 30, 22, 18, 10, 6, 0, 4, 10, 14, 20},
    //     {45, 42, 34, 26, 22, 14, 10, 4, 0, 6, 10, 16},
    //     {50, 48, 40, 32, 28, 20, 16, 10, 6, 0, 4, 10},
    //     {55, 52, 44, 36, 32, 24, 20, 14, 10, 4, 0, 6},
    //     {60, 58, 50, 42, 38, 30, 26, 20, 16, 10, 6, 0}
    // };
    
    // now for a 15x15 matrix
    // int CostGraphMatrix[15][15];    
    // // Generate a 15x15 matrix with INF down the leading diagonal
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 15; j++) {
    //         if (i == j) {
    //             CostGraphMatrix[i][j] = INF;
    //         } else {
    //             // Generate random numbers for the other values
    //             CostGraphMatrix[i][j] = rand() % 100 + 1;
    //         }
    //     }
    // }


    // Function call
    int val = solve(CostGraphMatrix);
    std::cout << "Total cost is " << val << "\n";
    return 0;
}
