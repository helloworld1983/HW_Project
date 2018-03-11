#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"

class BpTreeIndexNode : public BpTreeNode
{
private:
	map <double, BpTreeNode*> mapIndex;
	
public:
	BpTreeIndexNode(){}
	~BpTreeIndexNode(){}
	
	void setNext(BpTreeNode* pN) { return ; };
	void setPrev(BpTreeNode* pN) { return; };
	BpTreeNode* getNext() { return 0; };
	BpTreeNode* getPrev()  { return 0; };
	void insertIndexMap(double n, BpTreeNode* pN)	{	mapIndex[n] = pN;	}
	void insertDataMap(double avgGrade, map<int, StudentData*> m)	{	return; }
	void deleteMap(double n)	{	mapIndex.erase(n);	}
	map <double, BpTreeNode*>* getIndexMap()	{ return &mapIndex; }
	map< double, map<int, StudentData*> > *getDataMap(){ return 0; };
};

#endif