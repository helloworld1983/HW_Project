#include "GraphMethod.h"

bool BFS(Graph* graph, int baseV, ofstream* fout)
{
	/****************** exception *********************/
	if (graph->getDirected() == true || graph->getWeighted() == true) // if graph has direction or weight, return false
		return false;
	if (baseV > graph->getSize()) // if input vertex isn't exist in the graph
		return false;
	/***********************************************/

	map<int, int>* m = new map<int, int>; // to save incident vertex
	map<int, int> mark;
	queue<int> q;
	q.push(baseV);
	cout << "======= BFS =======" << endl;
	*fout << "======= BFS =======" << endl;
	while (!q.empty())
	{
		int  temp = q.front();
		q.pop();
		cout << temp << " ";
		*fout << temp << " ";
		graph->getIncidentEdges(temp, m); // get incident edges
		map<int, int>::iterator i, j;    // m's iterator
		i = m->begin();
		while (i != m->end())// push current vertex' incident vertex
		{
			if (mark.find(i->second) != mark.end()) // if mark has same key
			{
				j = i++;
				if (mark.find(j->second)->second != j->first) // if vertex which i want to push isn't exist in the mark map, push it into queue
				{
					q.push(j->first);
					mark.insert(make_pair(j->first, j->second));// save visited vertex
				}
				m->erase(j->first); // if vertex which i want to push is already in the mark map, just erase it
			}
			else  // if mark doesn't hava same key
			{
				j = i++;							// back up i's address 
				q.push(j->first);
				mark.insert(make_pair(j->first, j->second));// save visited vertex
				m->erase(j->first);
			}
		}
	}
	cout << endl << "===================" << endl;
	*fout << endl << "===================" << endl;
	return true;
}

bool DFS(Graph* graph, int baseV, ofstream* fout)
{
	/****************** exception *********************/
	if (graph->getDirected() == true || graph->getWeighted() == true) // if graph has direction or weight, return false
		return false;
	if (baseV > graph->getSize()) // if input vertex isn't exist in the graph
		return false;
	/***********************************************/

	map<int, int>* m = new map<int, int>; // to save incident vertex
	map<int, int> mark;
	stack<int> s;
	s.push(baseV);
	cout << "======= DFS =======" << endl;
	*fout << "======= DFS =======" << endl;
	while (!s.empty()) // if stack is empty, escape loop
	{
		int  temp = s.top();
		s.pop();
		cout << temp << " ";
		*fout << temp << " ";
		graph->getIncidentEdges(temp, m); // get incident edges
		map<int, int>::iterator i, j;    // m's iterator
		i = --m->end();
		while (i != m->begin())// push current vertex' incident vertex
		{
			if (mark.find(i->second) != mark.end()) // if mark has same key
			{
				j = i--;
				if (mark.find(j->second)->second != j->first) // if vertex which i want to push isn't exist in the mark map, push it into queue
				{
					s.push(j->first);
					mark.insert(make_pair(j->first, j->second));// save visited vertex
				}
				m->erase(j->first); // if vertex which i want to push is already in the mark map, just erase it
			}
			else  // if mark doesn't hava same key
			{
				j = i--;							// back up i's address 
				s.push(j->first);
				mark.insert(make_pair(j->first, j->second));// save visited vertex
				m->erase(j->first);
			}
		}
		if (i == m->begin()) // if i is begin of map m
		{
			if (mark.find(i->second) != mark.end()) // if mark has same key
			{
				j = i--;
				if (mark.find(j->second)->second != j->first) // if vertex which i want to push isn't exist in the mark map, push it into queue
				{
					s.push(j->first);
					mark.insert(make_pair(j->first, j->second));// save visited vertex
				}
				m->erase(j->first); // if vertex which i want to push is already in the mark map, just erase it
			}
			else  // if mark doesn't hava same key
			{
				j = i--;							// back up i's address 
				s.push(j->first);
				mark.insert(make_pair(j->first, j->second));// save visited vertex
				m->erase(j->first);
			}
		}
	}
	cout << endl << "===================" << endl;
	*fout << endl << "===================" << endl;
	return true;
}

bool DFS_R(Graph* graph, vector<bool>* visit, int baseV, ofstream* fout)
{
		/****************** exception *********************/
		if (graph->getDirected() == true || graph->getWeighted() == true) // if graph has direction or weight, return false
			return false;
		if (baseV > graph->getSize()) // if input vertex isn't exist in the graph
			return false;
		/***********************************************/
		cout << baseV << " "; // print baseV
		*fout << baseV << " ";
		map<int, int>* m = new map<int, int>;
		graph->getIncidentEdges(baseV, m); // second, save incident edges of vertex(base V)
		while (m->size() != 0)
		{
			map<int, int>::iterator it = m->begin(); // get the smallest incident vertex of current vertex;
			
			while (it != m->end())
			{
				if (visit->at((it->first * graph->getSize()) + it->second) == true) // if that vertex is already visited vertex
					m->erase((it++)->first); // delete that edge in map
				
				else 
					it++;
			}
			if (m->size() == 0) // if map size is 0 after deleting , return true(close current function)
			{
				//cout << baseV << " ";
				return true;
			}

			it = m->begin();
			int a = (it->first * graph->getSize()) + it->second;//  marking
			int b = (it->second * graph->getSize()) + it->first; // marking
			visit->at(a) = true;
			visit->at(b) = true;

			int pass = m->begin()->first;
			m->erase(m->begin()->first); // erase smallest one
			DFS_R(graph, visit, pass, fout);

		}
		return true;
}

bool Kruskal(Graph* graph, ofstream* fout)
{
	/****************exception****************/
	if (graph->getDirected() == true)
		return false;
	if (graph->getWeighted() == false)
		return false;
	/***************************************/

	cout << "====== Kruskal ======" << endl;
	*fout << "====== Kruskal ======" << endl;
	multimap<int, pair<int, int> >* big = new multimap<int, pair<int, int> >;
	for (int a = 0; a < graph->getSize(); a++) // insert all edges of graph
	{
		map <int, int>* m = new map<int, int>;
		graph->getIncidentEdges(a, m); // save vertex a's edges into map m
		for (map <int, int>::iterator temp = m->begin(); temp != m->end(); temp++)
		{
			if ((big->find(temp->first) != big->end()) && (big->find(temp->first)->second.first == temp->second)) // don't insert already exist edge
				continue;
			big->insert(make_pair(temp->first, make_pair(a, temp->second))); // insert (weight, start vertex, destination vertex) set
		}
			
		delete m;
	} // sort all edges in ascending power accoding to weight

	vertexSet* set = new vertexSet(big->size()); // make tree which size is edge's number
	multimap<int, pair<int, int> >::iterator a;
	multimap<int, pair<int, int> > * kru = new multimap<int, pair<int, int> >;

	while (big->size() != 0)
	{
		a = big->begin();
		int s1 = set->Find(a->second.first);
		int s2 = set->Find(a->second.second);
		if (s1 != s2) // if (u,v) don't make cycle
		{
			set->Union(s1, s2);
			kru->insert(make_pair(a->first, a->second)); // insert edge into kru map
		}
		big->erase(a->first); // delete edge from big
	}
	delete big;

	int cost = 0;
	for (a = kru->begin(); a != kru->end(); a++)
	{
		cout << a->first << " ";  // print weight in ascending power
		*fout << a->first << " ";
		cost += a->first;// add weight into cost
	}
	cout << endl << cost << endl; // insert cost
	*fout << endl << cost << endl;
	cout << "=====================" << endl;
	*fout << "=====================" << endl;
	return true;
}

bool Dijkstra(Graph* graph, int baseV, ofstream* fout)
{
	/****************exception****************/
	if (graph->getDirected() == false || graph->getWeighted() == false) // not suitable to excute
		return false;
	if (baseV > graph->getSize()) // if graph don't have baseV vertex
		return false;
	/***************************************/
	int size = graph->getSize();

	/*make array to insert infomation of vertex*/
	/* distance , visit, path */

	int ** length = new int*[size];
	int ** info = new int*[size];
	for (int a = 0; a < size; a++)
	{
		length[a] = new int[size];
		info[a] = new int[3];
		memset(length[a], 0, sizeof(int)*size); // initialize length array into 0
		info[a][0] = -1;		// initialize distance to INF
		info[a][1] = 0;				// initialize visit to 0
		info[a][2] = -1;			// initialize path tot -1
	}
	for (int a = 0; a < graph->getSize(); a++) // insert all edges of graph
	{
		map <int, int>* m = new map<int, int>;
		graph->getIncidentEdges(a, m); // save vertex a's edges into map m
		for (map <int, int>::iterator temp = m->begin(); temp != m->end(); temp++)
			length[a][temp->second] = temp->first;// insert length
		delete m;
	} // sort all edges in ascending power accoding to weight

	/*before start, find the vertex which is not a baseV vertex and nobody try to visit*/
	for (int a = 0; a < size; a++) // a : base vertex
	{
		if (a == baseV)
			continue;
		int flag = 0;
		/* confirm sombody try to visit or not */
		for (int b = 0; b < size; b++) // b : vertex which try to visit a
		{
			if (length[b][a] != 0) // if somebody try to visit
				flag = 1;			// set flag to 1
		}

		if (flag == 0) // if nobody try to visit...... set its visit value to 1 (;_;)
			info[a][1] = 1;
	}
	int all_visit = 0;
	int start_ver = baseV;
	info[baseV][0] = 0;
	info[baseV][1] = 1;
	while (1)
	{
		/* reset distance*/
		for (int a = 0; a < size; a++)
		{
			if (length[start_ver][a] >(int)0) // if weight from current vertex to destination vertex(a) is exist
			{
				if ((info[a][0] == -1) || (info[a][0] > info[start_ver][0] + length[start_ver][a]))// if distance is -1 or
									//	(cost to current vertex + cost from current vertex to destination vertex (a))is smaller than existing cost
				{
					info[a][0] = info[start_ver][0] + length[start_ver][a]; // change cost
					info[a][2] = start_ver;									// set start_vertex
				}
				else; // info[a][0] <= info[start_ver][0] + length[start_ver][a]
			}
		}

		info[start_ver][1] = 1;
		for (int a = 0; a < size; a++) // confirm all vertex is visited or not
		{
			all_visit = 1;
			if (info[a][1] == 0) // if didn't visited vertex found
			{
				all_visit = 0; // set
				break;
			}
		}
		if (all_visit == 1) // if all vertex have visited
			break;			// escape loop
		/*find the nearest vertex*/
		map<int, int>* near = new map<int, int>;
		for (int a = 0; a < size; a++)
		{
			if (info[a][0] != -1 && info[a][1] != 1)
				near->insert(make_pair(info[a][0], a));	// insert vertex which visit value is 0
		}
		 // set current vertex's visit value to 1
		start_ver = near->begin()->second; // set start_ver to the nearest one of currrent vertex
		delete near;
	}

	cout << "====== Dijkstra ======"<<endl;
	*fout << "====== Dijkstra ======" << endl;
	
	cout << baseV << endl;
	*fout << baseV << endl;
	/* print */
	for (int a = 0; a < size; a++)
	{
		if (a == baseV) // pass start vertex 
			continue;
		if (info[a][2] == -1) // if current vertex is not start vertex and its path value is -1
		{
			cout << "[" << a << "] " << " x " << endl;
			*fout << "[" << a << "] " << " x " << endl;
			continue;
		}
		cout << "[" << a << "] ";
		*fout << "[" << a << "] ";

		int sum = 0;
		if (info[a][2] == baseV)
		{
			cout << info[a][2] << " ";
			*fout << info[a][2] << " ";
		}
		else
		{
			int b = a;
			while (info[b][2] != baseV)
			{
				cout << info[b][2]<< " ";
				*fout << info[b][2] << " ";
				b = info[b][2];
			}
			cout << info[b][2] << " ";
			*fout << info[b][2] << " ";
		}
		sum = info[a][0];
		cout << "(" << sum << ")" << endl;
		*fout << "(" << sum << ")" << endl;
	}
	cout << "======================"<< endl;
	*fout << "======================" << endl;
	return true;
}
