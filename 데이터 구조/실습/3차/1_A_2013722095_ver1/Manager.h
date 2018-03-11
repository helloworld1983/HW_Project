#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "GraphMethod.h"

class Manager
{
private:
	Graph* graph;
	ofstream fout;
	int load;

public:
	Manager();
	~Manager();

	void	run(const char * command_txt);
	
	bool	LOAD(char* filename);
	bool	PRINT();
	
	bool	mDijkstra(int from);
	bool	mKruskal();
	bool	mBFS(int ver);
	bool	mDFS_R(int ver);
	bool	mDFS(int ver);

	void	printErrorCode(int n);	
};

#endif