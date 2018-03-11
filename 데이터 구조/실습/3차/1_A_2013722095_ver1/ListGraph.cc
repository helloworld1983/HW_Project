#include "ListGraph.h"

ListGraph::ListGraph(bool type, bool directed, bool weighted, int size) : Graph(type, directed, weighted, size)
{		
	m_List = new map<int, int>[size];
}

ListGraph::~ListGraph()
{
	delete m_List;
}

void ListGraph::getIncidentEdges(int vertex, map<int, int>* m) 
{
	int a = vertex;
	if (this->m_Directed == false && this->m_Weighted == true) // Kruskal(MST)
	{
		for (map<int, int>::iterator it = m_List[a].begin(); it != m_List[a].end(); it++)
		{
				m->insert(make_pair(it->second, it->first)); // insert 'weight', 'destination vertex'
		}
	}
	else if (this->m_Directed == true && this->m_Weighted == true) // Dijkstra
	{
		for (map<int, int>::iterator it = m_List[a].begin(); it != m_List[a].end(); it++)
			m->insert(make_pair(it->second, it->first));// insert 'weight', 'destination vertex'
	}
	else // RFS, DFS, DFS_R
	{
		for (map<int, int>::iterator it = m_List[a].begin(); it != m_List[a].end(); it++)
			m->insert(make_pair(it->first, a)); // insert 'destination vertex', 'start vertex'
	}
	
}

void ListGraph::insertEdge(int from, int to, int weight)
{
	m_List[from][to] = weight;
}

bool	ListGraph::printGraph(ofstream* fout)
{
	if (!m_List->begin()->first) // if m_List's begin vaule has no data, return false 
		return false;
	cout << "====== PRINT =======" << endl;
	*fout << "====== PRINT =======" << endl;
	
	for (int a = 0; a < this->m_Size; a++) 
	{
		cout << a << "  ";
		*fout << a << "  ";
		for (map<int, int>::iterator it = m_List[a].begin(); it != m_List[a].end(); it++) 
		{
			cout << " " << it->first << "," << it->second;
			*fout << " " << it->first << "," << it->second;
		}
		cout << endl;
		*fout << endl;
	}
	cout << "====================" << endl << endl;
	*fout << "====================" << endl << endl;
	return true;
}