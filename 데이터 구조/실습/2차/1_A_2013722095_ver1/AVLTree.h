#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "AVLNode.h"

class AVLTree{
private:
	AVLNode*	root;
	vector < pair< pair<double, int>, StudentData* > > vHeap;
	ofstream*	fout;

public:
	AVLTree(ofstream* fout);
	~AVLTree();
	
	bool		Insert(StudentData* pStu);
	bool		Print();
	bool		Search(char* name);
	bool		Rank();
	void		printErrorCode(int n);
	void		printStudentInfo(StudentData* pStudentData);
};

#endif