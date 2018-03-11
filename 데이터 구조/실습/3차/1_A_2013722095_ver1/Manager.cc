#include "Manager.h"
#pragma warning(disable:4996)
Manager::Manager()
{
	fout.open("log.txt", ios::app);
	load = 0;
}

Manager::~Manager()
{
	fout.close();
}

void Manager::run(const char* command_txt)
{
	ifstream fin;		fin.open(command_txt); // open command file
		
	if(!fin)													// fail to open command file
	{
		fout << "[ERROR] command file open error!" << endl;
		cout<<"[ERROR] command file open error!"<<endl; // print error sentence
		return;
	}
	
	char*	str='\0';
	char*	str2='\0';
	char	buf[128]={0};

	while(fin.getline(buf, 64)) // get command file's line while the file is over
	{
		str=strtok(buf, " \t\n");	// seperate line by tap,enter
		
		if (strcmp(str, "LOAD") == 0) // if token is LOAD
		{
			if ((str = strtok('\0', " \t\n")) == '\0') // if token is '\0', print error code
				printErrorCode(100);

			if (load == 1)										// if program load command file already
			{
				printErrorCode(100);							// print error code 
			}
			else if ((str2 = strtok('\0', " \t\n")) != '\0' || !LOAD(str)) // if token2 is '\0' or program failed to open file which name is same with token2
			{
				printErrorCode(100);															// print error code
			}
			load = 1;												// set load flag to 1
			continue;
		}
		if (strcmp(str, "PRINT") == 0)
		{
			if (PRINT() == false) // if PRINT return false, print error code 
				printErrorCode(200);
			continue;
		}
		if (strcmp(str, "BFS") == 0)
		{
			str = strtok('\0', " \t\n"); // get next word
			if (str == '\0' || mBFS(atoi(str)) == false) // if BFS return false or next word is null, print error code 
				printErrorCode(300);
			continue;
		}
		if (strcmp(str, "DFS") == 0)
		{
			str = strtok('\0', " \t\n"); // get next word
			if (str == '\0'|| mDFS(atoi(str)) == false)// if DFS return false or next word is null, print error code 
				printErrorCode(400);
			continue;
		}
		if (strcmp(str, "DFS_R") ==0)
		{
			str = strtok('\0', " \t\n"); // get next word
			if (str == '\0' || mDFS_R(atoi(str)) == false)// if DFS_R return false or next word is null, print error code 
				printErrorCode(500);
			continue;
		}
		if (strcmp(str, "Kruskal") ==0)
		{
			if (mKruskal() == false)// if Kruskal return false, print error code 
				printErrorCode(600);
			continue;
		}
		if (strcmp(str, "Dijkstra") == 0)
		{
			str = strtok('\0', " \t\n"); // get next word
			if (str == '\0' || mDijkstra(atoi(str)) == false)// if Dijkstra return false or next word is null, print error code 
				printErrorCode(700);
			continue;
		}
		if (strcmp(str, "EXIT") == 0) // if token is same with EXIT, close program
			return;

		fout<<endl;
	}
	fin.close();
}

bool Manager::LOAD(char* filename) // open graph data file
{
	char*	str		='\0';
	char*	str2	='\0';
	char*	str3	='\0';
	char*	str4	='\0';
	char	buf[128]={0};
	char	buf2[128]={0};
	int counter = 0;
	ifstream fin;
	fin.open(filename); // open data file
	
	if(!fin)					// if failed to open data file return false
	{
		return false;
	}

	fin.getline(buf, 128); // get line
	str = strtok(buf, " \t\n"); // get type
	str2 = strtok('\0', " \t\n");//get direct
	str3 = strtok('\0', " \t\n"); // get weight
	fin.getline(buf2, 128);
	str4 = strtok(buf2, " \t\n"); // get size

	if (strcmp(str, "L") == 0) // if data is list type
	{
		graph = new ListGraph(false, (atoi(str2) != 0), (atoi(str3) != 0), atoi(str4)); // insert this graph's infomation into listgraph's private member


		int vertex = 0;
		while (fin.getline(buf, 128)) // while line is NULL
		{
			if (counter > graph->getSize()) // if size is not equal
				return false;
			str2 = strtok(buf, " \t\n");			// get vertex
			str3 = strtok('\0', " \t\n");	// get weight

			if (str3 == '\0') // if weight is not null
			{
				vertex = atoi(str2); // set str2 to vertex
				counter += 1; // plus counter 
			}

			else // if weight is null, it means this line has only vertex info
			{
				graph->insertEdge(vertex, atoi(str2), atoi(str3)); // insert edge
				continue;
			}
		}
	
	} // get information from file


	if (strcmp(str, "M") == 0) // if data is matrix type
	{
		graph = new MatrixGraph(true, (atoi(str2) != 0), (atoi(str3) != 0), atoi(str4)); // get information from file
		int a = 0;
		while (fin.getline(buf2, 128)) //  loop; // get line
		{
			if (counter >= graph->getSize()) // if graph data information in the textfile is more than graph size, return false
				return false;
			str = strtok(buf2, " \t\n"); // get token
			graph->insertEdge(a, 0,atoi(str)); // insert atoi(str) to m_Map[a][0]
			for (int b = 1; b < graph->getSize(); b++) // inner loop
			{
				str = strtok('\0', " \t\n");						// get token
				graph->insertEdge(a, b, atoi(str)); // insert token to m_Map[a][b]
			}
			counter += 1; // plus counter 
			a++;
		}
	}



	cout << "======= LOAD ======" << endl;
	fout << "======= LOAD ======" << endl;
	cout << "Success" << endl;
	fout << "Success" << endl;
	cout << "===================" << endl;
	fout << "===================" << endl;


	return true;
}

bool Manager::PRINT()
{
	if (graph->printGraph(&fout) == false) // if this graph's print function return false , return false
		return false;
	return true;
}

bool	Manager::mBFS(int ver)
{
	if (BFS(graph, ver, &fout) == false) // if BFS function return false, return false
		return false;
	return true;
}

bool	Manager::mDFS_R(int ver) 
{
	cout << "====== DFS_R ======" << endl;
	fout << "====== DFS_R ======" << endl;
	int size = (graph->getSize() * graph->getSize()); 
	vector<bool> visit(size, false); // make vector's size as graph's size*graph's size and then initialize it to false
	if (DFS_R(graph, &visit, ver, &fout) == false) // if DFS_R function return false, return false
		return false;
	cout <<endl << "===================" << endl;
	fout << endl << "===================" << endl;
	return true;
}

bool	Manager::mDFS(int ver)
{
	if (DFS(graph, ver, &fout) == false) // if DFS function return false, return false
		return false;
	return true;
}

bool Manager::mDijkstra(int from)
{
	if (Dijkstra(graph, from,&fout) == false) // if Dijkstra function return false, return false
		return false;
	return true;
}

bool	Manager::mKruskal() 
{
	if (Kruskal(graph, &fout) == false) // Kruskal function return false, return false
		return false;
	return true;
}

void Manager::printErrorCode(int n)
{
	fout<<"======== ERROR ========"<<endl;
	fout<<n<<endl;
	fout<<"======================="<<endl;
	cout << "======== ERROR ========" << endl;
	cout << n << endl;
	cout << "=======================" << endl;
}