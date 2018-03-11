#include "Graph.h"

Graph::Graph(bool type, bool directed, bool weighted, int size)
{
	m_Type = type;
	m_Directed = directed;
	m_Weighted = weighted;
	m_Size = size;
}

Graph::~Graph()
{
}

bool Graph::getDirected()
{
	return m_Directed;
}

bool Graph::getWeighted()
{
	return m_Weighted;
}

int Graph::getSize()
{
	return m_Size;
}