#ifndef _BPTREEDATANODE_H_
#define _BPTREEDATANODE_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode
{
private:
	map <double, map<int, StudentData*> > mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;

public:
	BpTreeDataNode(){
		pNext = '\0';
		pPrev = '\0';
	}

	void setNext(BpTreeNode* pN)	{ pNext = pN; }
	void setPrev(BpTreeNode* pN)	{ pPrev = pN; }
	BpTreeNode* getNext()				{ return pNext; }
	BpTreeNode* getPrev()				{ return pPrev; }	

	void insertDataMap(double avgGrade, map<int, StudentData*> m)
	{	mapData[avgGrade] = m;	}
	void insertIndexMap(double n, BpTreeNode* pN)
	{	return;}
	void deleteMap(double n)
	{	mapData.erase(n);	}	
	map <double, BpTreeNode*>* getIndexMap()	{ return 0; }
	map< double, map<int, StudentData*> > *getDataMap()		{ return &mapData; }
};

#endif