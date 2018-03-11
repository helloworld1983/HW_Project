#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(bool type, bool directed, bool weighted, int size) : Graph(type, directed, weighted, size)
{
	m_Mat = new int*[size];
	for (int a = 0; a < size; a++)
	{
		m_Mat[a] = new int[size];
		memset(m_Mat[a], 0,sizeof(int)*(size)); // initilizing
	}
}

MatrixGraph::~MatrixGraph()
{
	for (int a = 0; a < this->m_Size; a++)
		delete m_Mat[a];
}

void MatrixGraph::getIncidentEdges(int vertex, map<int, int> *m) 
{	
	int a = vertex;
	if (this->m_Directed == false && this->m_Weighted == true) //Kruskal(MST)
	{
		for (int b = vertex + 1; b < m_Size; b++) // insert graph array's half side data 
		{
			if (m_Mat[a][b] != (int)0)
				m->insert(make_pair(m_Mat[a][b], b)); // insert weight,destination vertex
		}
			
	}
	else if (this->m_Directed == true && this->m_Weighted == true) // Dijkstra
	{
		for (int b = 0; b < m_Size; b++)
		{
			if (m_Mat[a][b] != (int)0)
				m->insert(make_pair(m_Mat[a][b], b)); // insert weight,destination vertex
		}
	}
	else // RFS, DFS, DFS_R
	{
		int b = 0;
		while (b < this->m_Size)
		{
			if (m_Mat[a][b] >= 1)// if m_Mat[a][b] has value bigger than 1, insert it into map* m
				m->insert(make_pair(b, a));	// insert 'destination vertex' and 'start vertex'
			b++;
		}
	}
	
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
	if (this->m_Directed == true) // Dijkstra 
	{
		m_Mat[from][to] = weight;
	}
	else						 // RFS, DFS, DFS_R, Kruskal
	{
		m_Mat[from][to] = weight;
		m_Mat[to][from] = weight;
	}
	
}

bool	MatrixGraph::printGraph(ofstream* fout)
{
	int flag = 0;
	for (int a = 0; a < this->m_Size; a++) // find edge
	{
		for (int b = 0; b < this->m_Size; b++)
		{
			if (m_Mat[a][b] != 0) // if found a edge, set flag to 1
				flag = 1;
		}
	}
	if (flag == 0) // if couldn't find edge, return false
		return false;

	
	cout << "======== PRINT ========" << endl;
	*fout << "======== PRINT ========" << endl;
	for (int a = 0; a < this->m_Size; a++) // start vertex
	{
		for (int b = 0; b < this->m_Size; b++) // destination vertex
		{
			cout << " " << m_Mat[a][b];
			*fout << " " << m_Mat[a][b];
		}
		cout << endl;
		*fout << endl;
	}
	cout << "=====================" << endl;
	*fout << "=====================" << endl;
	return true;
}
