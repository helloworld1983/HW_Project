#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "vertexSet.h"

bool BFS(Graph* graph, int vertex, ofstream* fout);
bool DFS(Graph* graph, int vertex, ofstream* fout);
bool DFS_R(Graph* graph, vector<bool>* visit, int vertex, ofstream* fout);
bool Kruskal(Graph* graph, ofstream* fout);
bool Dijkstra(Graph* graph, int vertex, ofstream* fout);
#endif