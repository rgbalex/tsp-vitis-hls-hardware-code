#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_VERTICES 20
#define MAX_EDGES 100
#define INT_MAX __INT_MAX__

typedef struct {
	int vertices[MAX_VERTICES];
	int edges[MAX_EDGES][2];
	int nVertices;
	int nEdges;
} Graph;

int g[MAX_VERTICES][1 << MAX_VERTICES];

void tsp(Graph *graph, int nVertices) {
	int i, j, k, s, mask;

	for (i = 0; i < nVertices; i++) {
		g[i][1 << i] = graph->edges[i][1];
	}

	for (s = 2; s < (1 << nVertices); s++) {
		for (i = 0; i < nVertices; i++) {
			if (((s >> i) & 1) == 0) continue;
			mask = s ^ (1 << i);
			g[i][s] = INT_MAX;
			for (j = 0; j < nVertices; j++) {
				if (i == j) continue;
				if (((mask >> j) & 1) == 0) continue;
				g[i][s] = fmin(g[i][s], g[j][mask] + graph->edges[j][i]);
			}
		}
	}

	int min_cost = INT_MAX;
	for (i = 0; i < nVertices; i++) {
		min_cost = fmin(min_cost, g[i][(1 << nVertices) - 1] + graph->edges[i][0]);
	}

	printf("Minimum cost: %d\n", min_cost);
}

int main() {
	Graph graph;
	// Initialize graph data structure
	Graph.graph = {
		.vertices = {0, 1, 2, 3},
		.edges = {
			{0, 1, 2, 3},
			{1, 0, 4, 5},
			{2, 4, 0, 6},
			{3, 5, 6, 0}
		},
		.nVertices = 4,
		.nEdges = 4
	};


	tsp(&graph, graph.nVertices);
	return 0;
}
