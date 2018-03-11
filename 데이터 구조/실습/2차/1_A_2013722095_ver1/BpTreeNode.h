#ifndef _BpTreeNode_H_
#define _BpTreeNode_H_

#include "Year.h"

class BpTreeNode {
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode(){
		pParent = '\0';
		pMostLeftChild = '\0';
	}

	void setMostLeftChild(BpTreeNode* pN)	{ pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN)				{ pParent = pN; }
	
	BpTreeNode* getParent()						{ return pParent;	}
	BpTreeNode* getMostLeftChild()				{ return pMostLeftChild; }

	virtual void setNext(BpTreeNode* pN) =0;
	virtual void setPrev(BpTreeNode* pN) =0;
	virtual BpTreeNode* getNext() =0;
	virtual BpTreeNode* getPrev() =0;
	virtual void insertDataMap(double, map<int, StudentData*>) = 0;
	virtual void insertIndexMap(double, BpTreeNode* )=0;
	virtual void deleteMap(double) = 0;
	virtual map< double, map<int, StudentData*> > *getDataMap() = 0;
	virtual map <double, BpTreeNode*>* getIndexMap() = 0;

};

#endif