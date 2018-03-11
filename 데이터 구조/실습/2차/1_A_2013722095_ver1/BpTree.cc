#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order){
	root = '\0';
	this->order = order;
	this->fout = fout;
}

BpTree::~BpTree(){

}

void BpTree::Insert(StudentData* pStu){

	double key = pStu->getAvgGrade();	// key = avgGrade
	map<int, StudentData*> value;			// value = map< id, student data* >
	value.insert( make_pair(pStu->getStudentID(), pStu) );	

	if(root == '\0'){ // empty B+ tree
		BpTreeNode* pDataNode = new BpTreeDataNode();	// make new data node		
		pDataNode->insertDataMap(key, value);	// insert grade data
		root = pDataNode;
		return;
	}

	BpTreeNode* pDataNode = searchDataNode(key);	// seach data node with key
	map<double, map<int, StudentData*> > *m = pDataNode->getDataMap(); // *m pointing dataNode's mapdata which u want to insert new data
	map<double, map<int, StudentData*> >::iterator iterator;
	iterator = m->begin();
	int flag = 0; // flag to comfirm pDataNode already has same key with pStu's AvgGread or not
	for (int a = 0; a < m->size(); a++)
	{
		if (m->find(key) == iterator) // find same key?
		{												// yes!!!!!
			flag = 1; // set flag as 1
			iterator->second[pStu->getStudentID()] = pStu;
		}
		else	// nope!!
			iterator++;	// move iterator
	}
	if (flag == 0) // if couldn't find same key
		pDataNode->insertDataMap(key, value);
	
	/* cheking size over */
	if (exceedDataNode(pDataNode))//  if map size is bigger than order-1 after inserting, split them
		splitDataNode(pDataNode); 
	return;


	
}

BpTreeNode* BpTree::searchDataNode(double n){

	BpTreeNode* pCur = root;
	map<double, BpTreeNode*>::iterator itIndex;

	while(pCur->getMostLeftChild() != '\0'){ 
		itIndex = pCur->getIndexMap()->begin();

		if(n < itIndex->first){ 
			pCur = pCur->getMostLeftChild();
		}else{
			while(true){
				itIndex++;
				if(itIndex == pCur->getIndexMap()->end() || n < itIndex->first){
					itIndex--;
					pCur = itIndex->second;
					break;
				}
			}
		}
	}

	return pCur;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode){

	int splitPosition_data = ceil((order-1)/2.0) + 1;
	
	BpTreeNode* parent ='\0' ; // parent of datanode which will be split
	BpTreeNode* newdata = new BpTreeDataNode(); // make new data node
	map<double, map<int, StudentData*> >::iterator iterator, end; 
	iterator = pDataNode->getDataMap()->end();
	iterator--;
	/* split DataNode */
	for (int a = pDataNode->getDataMap()->size(); a >= splitPosition_data; a--)
	{
		end = iterator;
		newdata->insertDataMap(iterator->first, iterator->second);	 iterator--; // copy pDataNode's end() into newone and  move iterator to previous component
		pDataNode->deleteMap(end->first);  // delete end() component 
	}


	/* resetting DataNodes' next/prev link after split*/
	if (pDataNode->getNext() != '\0')
	{
		pDataNode->getNext()->setPrev(newdata);
		newdata->setNext(pDataNode->getNext());
		pDataNode->setNext(newdata);
		newdata->setPrev(pDataNode);
	}
	else
	{
		pDataNode->setNext(newdata);
		newdata->setPrev(pDataNode);
	}
		
	
	
	parent = pDataNode->getParent();
	/*making new IndexNode and add to pDataNode's parent(BpTreeIndexNode)*/
	if (pDataNode->getParent() == '\0') // if pDataNode has no parent
	{
		BpTreeNode* newindex = new BpTreeIndexNode();				// make new indexNode and set it's components
		newindex->insertIndexMap(newdata->getDataMap()->begin()->first, newdata);
		newindex->setMostLeftChild(pDataNode);
		newdata->setParent(newindex);
		pDataNode->setParent(newindex);
		root = newindex;
	}
	else
	{
		parent->insertIndexMap(newdata->getDataMap()->begin()->first, newdata);
		newdata->setParent(parent);
	}
	
	
	/*check IndexNode's size is suitable which has been modified*/
	parent = pDataNode->getParent();

	if (exceedIndexNode(parent)) // if size is over
	{
		BpTreeNode* temp = parent; // travle Node
		while(1)
		{
			if (!exceedIndexNode(temp)) // if current IndexNode's size isn't exceed order, escape loop
				break;
			splitIndexNode(temp);	// split current IndexNode
			temp = temp->getParent(); // move temp's parent
		}
	}
	/* root setting */
	parent = pDataNode; // resetting parent to move to the lowest level node
	while (1)
	{
		if (parent->getParent() == '\0') // if currentNode has no parent
		{	root = parent; break;	}
		parent = parent->getParent();
	}

}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode){ // split IndexNode function
	
	int splitPosition_index = ceil(order/2.0);
	map<double, BpTreeNode*>::iterator iterator, end;
	iterator = pIndexNode->getIndexMap()->end(); // set iterator as pIndexNode's end component
	iterator--;
	BpTreeNode* newindex = new BpTreeIndexNode();
	for (int a = pIndexNode->getIndexMap()->size(); a > splitPosition_index; a--)
	{
		end = iterator;
		newindex->insertIndexMap(iterator->first, iterator->second); iterator--; // copy last component at new index node
		pIndexNode->deleteMap(end->first);								// delete last component of pIndexNode
	}

	if (pIndexNode->getParent() == '\0') // if pIndexNode has no parent
	{
		BpTreeNode* newparent = new BpTreeIndexNode();

		newindex->setMostLeftChild(iterator->second); // set iterator's second value as new indexNode's mostleftchild
		newparent->insertIndexMap(iterator->first, newindex); // copy end component at new parent
		pIndexNode->deleteMap(iterator->first);								  // delete end component of pIndexNode
		newparent->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(newparent);
		newindex->setParent(newparent);
	}
	else								// if pIndexNode already has parent 
	{
		newindex->setMostLeftChild(iterator->second);
		pIndexNode->getParent()->insertIndexMap(iterator->first, newindex);
		pIndexNode->deleteMap(iterator->first);
		newindex->setParent(pIndexNode->getParent());
	}

}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode){ // confirm that datanode's size is suitable or not

	map <double, map<int, StudentData*> > *m = pDataNode->getDataMap();

	if(m->size() > order-1)		return true;
	else								return false;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode){ // confirm that indexnode's size is suitable or not

	map <double, BpTreeNode*> *m = pIndexNode->getIndexMap();
	
	if(m->size() > order-1)		return true;
	else								return false;
}



bool BpTree::Print(){
	if (root == '\0')
		return false;
	BpTreeNode* temp = root;
	map<double,map<int, StudentData*> >::iterator it;
	map<double, BpTreeNode*>::iterator end;

	while (1) // move to the biggest datanode
	{
		if (temp->getMostLeftChild() == '\0')
			break;
		end = temp->getIndexMap()->end();
		end--;
		temp = end->second; // move to most right side
	}

	it = temp->getDataMap()->end(); // it = datamap's last component
	it--;

	cout << "======== PRINT ========" << endl;
	*fout << "======== PRINT ========" << endl;
	while (1)
	{
		map<int, StudentData*>::iterator stu = it->second.begin();
		for (int a = 0; a < it->second.size(); a++) // print Student's info
		{
			printStudentInfo(stu->second);
			stu++;
		}
		
		if (it == temp->getDataMap()->begin()) // if it located in datamap's begin
		{
			if (temp->getPrev() == '\0') // if current node is the smallest datanode, escape loop
				break;
			temp = temp->getPrev();
			it = temp->getDataMap()->end();
			it--;
			continue;
		}
		
		it--;	// move it to datamap's begin
	}
	cout << "=======================" << endl;
	*fout << "=======================" << endl;
	return true;
}

bool BpTree::Search(double a, double b, int year){ 

	if (searchDataNode(b)->getDataMap()->find(b) == searchDataNode(b)->getDataMap()->end())  //if there's no one suitable for searching condition
	{
		if (searchDataNode(a)->getDataMap()->find(a) == searchDataNode(a)->getDataMap()->end())
			return false;
	}

	map<double, map<int, StudentData*> >::iterator big, small, temp, end;
	BpTreeNode* datanode;
	// find component which same or smaller than b
	big = searchDataNode(b)->getDataMap()->begin();
	end = searchDataNode(b)->getDataMap()->end();
	end--;
	while (1)
	{

		if (big == end)// if big located at the last component, escape loop
			break;
		if (big->first > b) // if big's key is bigger than b
		{	
			big--; break;
		} // move big to previous component and escape loop
		big++;
		
	}


	// find component which same or bigger than a
	datanode = searchDataNode(a);

	end = datanode->getDataMap()->end();
	end--;
	small = datanode->getDataMap()->begin();
	while (1)
	{
		
		if (small->first >= a) // if small's first value is same or bigger than a, escape loop
			break;
		else
		{
			if (small->first ==end->first) // if small located at datamap's last component
			{
				datanode = datanode->getNext(); // move next datanode
				small = datanode->getDataMap()->begin(); // re-set small as datamap's begin component
				end = datanode->getDataMap()->end(); // re-set end as current datamap's last component
				end--;
				continue;
			}
			small++; // move small to next component
		}

	}

	datanode = searchDataNode(b);
	temp = big;
	cout << "======== SEARCH ========" << endl;
	*fout << "======== SEARCH ========" << endl;
	
	while (1) 
	{
		map<int, StudentData*>::iterator itstu = temp->second.begin();
		for (int a = 0; a < temp->second.size(); a++)  // print specific grade Student's infomation
		{
			if (itstu->second->getYear() == year)
				printStudentInfo(itstu->second); 
			itstu++;
		}
		if (temp->first == small->first) // if temp lacated in small
			break;				// exit loop
		if (temp == datanode->getDataMap()->begin()) // if temp located in datamap's begin
		{
			datanode = datanode->getPrev();						// move to current datanode's prev node
			temp = datanode->getDataMap()->end();		// set temp as end of current datanode's datamap
			temp--;
			continue;
		}
		temp--;
	}
	*fout << "========================" << endl;
	cout << "========================" << endl;
	
	return true;
}

void BpTree::printStudentInfo(StudentData* pStudentData){
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