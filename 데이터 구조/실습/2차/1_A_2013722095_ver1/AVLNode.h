#ifndef _AVLNODE_H_
#define _AVLNODE_H_

#include "Year.h"

class AVLNode
{
private:
	AVLNode*			pRight;
	AVLNode*			pLeft;
	StudentData*		pStudentData;
	int						mBF;

public:
	AVLNode(){
		pRight = '\0';
		pLeft = '\0';
		pStudentData = '\0';
		mBF = 0;
	}
	~AVLNode(){};

	void	setRight(AVLNode* SN)			{ pRight = SN; }
	void	setLeft(AVLNode* SN)			{ pLeft = SN; }
	void	setSD(StudentData* pStu)	{ pStudentData = pStu; }
	void	setBF(int a)							{ mBF = a; }

	AVLNode*		getRight()	{ return pRight; }
	AVLNode*		getLeft()	{ return pLeft; }
	StudentData*	getSD()		{ return pStudentData; }
	int					getBF()		{ return mBF; }
};

#endif