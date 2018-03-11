#include "AVLTree.h"

AVLTree::AVLTree(ofstream* fout){
	root='\0';
	make_heap(vHeap.begin(), vHeap.end(), compare);
	this->fout = fout;
}

AVLTree::~AVLTree(){

}

bool AVLTree::Insert(StudentData* pStu){

	AVLNode* pNew = new AVLNode;
	pNew->setSD(pStu);	
	char	*pName = pNew->getSD()->getName();
	int		rotation = 0;

	if(root == '\0'){ // if AVL tree has no node
		root = pNew;
		return true;
	}
	AVLNode* a = root;
	AVLNode* pa = '\0';
	AVLNode* p = root;
	AVLNode* pp = '\0';
	AVLNode* rootsub = '\0';

	while (p != '\0')
	{
		if (p->getBF() != 0)
		{	a = p; pa = pp; } 
		if (strcmp(pNew->getSD()->getName(), p->getSD()->getName()) < 0) // if new node's Student name is smaller than current node's Student name
		{	pp = p; p = p->getLeft(); continue;	}
		if (strcmp(pNew->getSD()->getName(), p->getSD()->getName()) > 0) // if now node's student name is bigger than current node's student name
		{	pp = p; p = p->getRight(); continue;	}
		// no same name case
	}
	if (strcmp(pNew->getSD()->getName(), pp->getSD()->getName()) < 0)
	{ pp->setLeft(pNew); } // if new node's student name is smaller than currentnode(leaf)'s student name
	if (strcmp(pNew->getSD()->getName(), pp->getSD()->getName()) > 0)
	{ pp->setRight(pNew); } // if new node's student name is bigger than currentnode(leaf)'s student name
	

	/*Balance Factor setting*/

	int bf = 0;
	AVLNode *b, *c;
	if (strcmp(pNew->getSD()->getName(), a->getSD()->getName()) > 0) // setting pointer b, c
	{	b = p = a->getRight();	bf = -1;}
	else
	{ b = p = a->getLeft(); bf = 1; }
	
	while (p != pNew)
	{
		if (strcmp(pNew->getSD()->getName(), p->getSD()->getName()) < 0)
		{	p->setBF(1); p = p->getLeft();	} // new node is locate in the left side of a
		else
		{	p->setBF(-1); p = p->getRight();	} // new node is locate in the right side of a
	}

	if (a->getBF() == 0 || (a->getBF() + bf) == 0) // no need to adjust node position(balanced case)
	{
		int value = a->getBF() + bf;
		a->setBF(value);
		return true;
	}
	if (bf == 1) // new node place in the pointer a's left side
	{
		if (b->getBF() == 1)	// new node place in the pointer b's left side
		{
			a->setLeft(b->getRight());
			b->setRight(a);
			a->setBF(0);
			b->setBF(0);
			rootsub = b;
		}
		else							// new node place in the pointer b's right side
		{
			c = b->getRight();
			b->setRight(c->getLeft());
			a->setLeft(c->getRight());
			c->setLeft(b);
			c->setRight(a);
			switch (c->getBF()) // set a, b's balance factor depending on  c's balance factor
			{
			case 0:
				a->setBF(0); b->setBF(0);
				break;
			case 1:
				a->setBF(-1); b->setBF(0);
				break;
			case -1:
				a->setBF(0); b->setBF(1);
				break;
			}
			c->setBF(0); // set c's balance factor to zero and set rootsub pointer to c
			rootsub = c;
		}
	}
	else			// new node place in the pointer a's right side 
	{
		if (b->getBF() == -1) // new node place in the pointer b's right side
		{
			a->setRight(b->getLeft());
			b->setLeft(a);
			a->setBF(0);
			b->setBF(0);
			rootsub = b;
		}
		else								// new node place in the pointer b's left side
		{
			c = b->getLeft();
			b->setLeft(c->getRight());
			a->setRight(c->getLeft());
			c->setRight(b);
			c->setLeft(a);
			switch (c->getBF()) // set a, b's balance factor depending on c's balance factor
			{
			case 0:
				a->setBF(0); b->setBF(0);
				break;
			case 1:
				a->setBF(0); b->setBF(-1);
				break;
			case -1:
				a->setBF(1); b->setBF(0);
				break;
			}
			c->setBF(0);// set c's balance factor to zero and set rootsub pointer to c
			rootsub = c;
		}
	}
	// root sub setting
	if (pa == '\0') root = rootsub;
	else if (a == pa->getLeft()) pa->setLeft(rootsub);
	else pa->setRight(rootsub);
	return true;
}

bool AVLTree::Print(){
	
	stack<AVLNode*> s ;
	AVLNode* currentNode = root; 

	if (currentNode == '\0') // if  AVLTree has no node
		 return false;	

	// print all informations in AVLTree by using inder traversal(ascending print)
	*fout << "========PRINT========" << endl;
	cout << "========PRINT========" << endl;
	while (1)
	{
		while (currentNode)
		{
			s.push(currentNode);
			currentNode = currentNode->getLeft();
		}
		if (s.empty())
			return true;
		currentNode = s.top();
		s.pop();
		printStudentInfo(currentNode->getSD());
		currentNode = currentNode->getRight();
	}

	*fout << "=======================" << endl;
	cout << "=======================" << endl;
	
	return true;
}

bool AVLTree::Search(char* name){
	if (name == '\0')		// when the command file's form isn't suitable
		return false;	

	AVLNode *pCur = root; // current node pointer pCur

	while (pCur)
	{
		if (strcmp(name, pCur->getSD()->getName()) < 0)		// name that want to find is smaller than current node
		{	pCur = pCur->getLeft(); continue;	}
		else if (strcmp(name, pCur->getSD()->getName()) > 0)// name that want to find is bigger than current node
		{	pCur = pCur->getRight(); continue;}
		else																				// find
			break;
	}

	if (pCur == '\0')	// couldn't find name
		return false;	
	
	//record student's information into log file
	*fout << "========SEARCH========" << endl;
	cout << "========SEARCH========" << endl;
	printStudentInfo(pCur->getSD());
	*fout << "=======================" << endl;
	cout << "=======================" << endl;
	// insertion into heap
	StudentData* pSD = pCur->getSD(); 
	vHeap.push_back( make_pair( make_pair(pSD->getAvgGrade(), pSD->getStudentID()), pSD));
	push_heap(vHeap.begin(), vHeap.end(), compare);

	return true;
}

bool AVLTree::Rank(){

	if(vHeap.empty())		return false;

	StudentData* pStu;
	cout << "======== RANK ========" << endl;
	*fout << "======== RANK ========" << endl;
	while(!vHeap.empty()){
		pStu = vHeap.front().second;
		pop_heap(vHeap.begin(), vHeap.end(), compare);
		vHeap.pop_back();
		printStudentInfo(pStu);
	}
	cout << "======================" << endl;
	*fout << "======================" << endl;
	return true;
}

void AVLTree::printStudentInfo(StudentData* pStudentData){
	cout << pStudentData->getStudentID() << " ";
	cout << pStudentData->getName() << " ";
	cout << pStudentData->getYear() << " ";
	cout << pStudentData->getAvgGrade() << endl;
	*fout << pStudentData->getStudentID() << " ";
	*fout << pStudentData->getName() << " ";
	*fout << pStudentData->getYear() << " ";
	*fout << pStudentData->getAvgGrade() << endl;
	pStudentData->printGrade();
	pStudentData->printGrade(fout);
}
