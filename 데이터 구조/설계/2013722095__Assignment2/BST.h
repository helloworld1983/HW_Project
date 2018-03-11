#include <iostream>
#include <utility>
#include <queue>
using namespace std;


template <class T>
class TreeNode // Node class 
{
public:
	T data;
	TreeNode<T>* leftChild = NULL;
	TreeNode<T>* rightChild = NULL;
	TreeNode(T Newpair) :data(Newpair), leftChild(NULL), rightChild(NULL){} // constructer
};

template <class K, class E>
class Dictionary // template class having pure virtual function
{
public:
	virtual bool IsEmpty()const = 0;
	virtual pair<K, E> * Get(const K& )const = 0;
	virtual void Insert(const pair<K,E>& ) =0;
	virtual void Delete(const K&) = 0;
};

template <class K, class E>
class BST : public Dictionary<K,E> // class BST inherit Dictionary's  public member(virtual function)
{
public:
	TreeNode<pair<K, E>>* root = NULL; // first made Node

	bool IsEmpty() const;
	pair<K, E>* Get(const K& ) const;	
	pair<K, E>* Get(TreeNode<pair<K,E>>* , const K& ) const;
	void Insert(const pair<K, E>& );
	void Delete(const K &);
	void Print();
};
// BST overrided Dictionary's functions
template <class K, class E>
bool BST<K,E>::IsEmpty() const // if BST has no node, return true
{
	if (root == NULL)
		return true;
	return false;// has any node, return  false
};

/* None Recursive Searching*/
/*
template<class K, class E>
pair<K, E>* BST<K, E>::Get(const K& k) const
{
	TreeNode<pair<K, E>>* currentNode = root;
	while (currentNode)
	{
		if (k < currentNode->data.first)
			currentNode = currentNode->leftChild;
		else if (k > currentNode->data.first)
			currentNode = currentNode->rightChild;
		else
			return &currentNode->data;
	}
}
*/
/* Recursive Searching */
template<class K, class E>// Get Driver
pair<K, E>* BST<K,E>::Get(const K& t) const
{
	return Get(root, t);
};

template <class K, class E>
pair<K, E>* BST<K,E>::Get(TreeNode<pair<K, E>>* p, const K& k) const // Get function's overloading(Get Workhorse)
{
	if (!p)
		return 0;
	if (k < p->data.first)
		return Get(p->leftChild, k);
	if (k > p->data.first)
		return Get(p->rightChild, k);
	return &p->data;
};

/* Insert function */
// recieve pair type data
template<class K, class E>
void BST<K, E>::Insert(const pair<K,E>& Newpair)
{
	TreeNode<pair<K, E>>* currentNode = root; // start at root
	TreeNode<pair<K, E>>* parent = NULL; // currentNode's previous node(parent )


	while (currentNode) // if currentNode is NULL, break this function
	{
		parent = currentNode; // parent is currentNode

		if (Newpair.first < currentNode->data.first) // if currentNode's first is bigger than recieved pair's first data
			currentNode = currentNode->leftChild; // currentNode pointer point its leftChild

		else if (Newpair.first > currentNode->data.first)// if currentNode's first is smaller than recieved pair's first data
			currentNode = currentNode->rightChild;// currentNode pointer point its rightChild

		else // if each second data is equal, save Newpair's second data to currentNode's second data
		{
			currentNode->data.second = Newpair.second;
			return;
		}
	}

	currentNode = new TreeNode<pair<K, E>>(Newpair); // dynamic allocating recieved pair data(Newpair)

	if (IsEmpty())// if BST has no node
		root = currentNode; // currentNode(dynamically allocated) is root
	else 
	{
		if (parent->data.first < Newpair.first) // new node is located in the rightside of parent
			parent->rightChild = currentNode;
		else
			parent->leftChild = currentNode; //new node is located in the rightside of parent
	}

};

template <class K, class E>  // Used Level order to print all Node
void BST<K, E>::Print()
{
	queue<TreeNode<pair<K,E>>*> q;
	TreeNode<pair<K,E>>* currentNode = root;
	while (currentNode)
	{
		cout << currentNode->data.first << "("<<currentNode->data.second<<") " ;
		if (currentNode->leftChild)
			q.push(currentNode->leftChild);
		if (currentNode->rightChild)
			q.push(currentNode->rightChild);
		if (q.empty())
			return;
		currentNode = q.front();
		q.pop();
	}
	
};

/*Delete function*/
template<class K, class E>
void BST<K, E>::Delete(const K& k) // recieve Key value
{
	TreeNode<pair<K, E>>* currentNode = root;
	TreeNode<pair<K, E>>* parent = NULL; // parent of currentNode
	while (currentNode&& k != currentNode->data.first) // move to the node which will delete
	{
		parent = currentNode;
		if (k < currentNode->data.first)
			currentNode = currentNode->leftChild;
		else
			currentNode = currentNode->rightChild;
	}
	if (currentNode == 0) // not found 
		return ;
	if (currentNode->leftChild == 0 && currentNode->rightChild == 0) // if currentNode is leaf node
	{
		if (parent == 0)
			root = 0;
		else if (parent->leftChild == currentNode)
			parent->leftChild = NULL;
		else
			parent->rightChild = NULL;
		delete currentNode;
	}

	else if (currentNode->leftChild == 0) // if currentNode only has right child
	{
		if (parent == NULL)
			root = currentNode->rightChild;
		else if (parent->leftChild == currentNode)
			parent->leftChild = currentNode->rightChild;
		else
			parent->rightChild = currentNode->rightChild;
		delete currentNode;
	}

	else if (currentNode->rightChild == 0) // if currentNode only has left child
	{
		if (parent == NULL)
			root = currentNode->leftChild;
		else if (parent->leftChild == currentNode)
			parent->leftChild = currentNode->leftChild;
		else
			parent->rightChild = currentNode->leftChild;
		delete currentNode;
	}

	// if parent has two(left, right) child
	else
	{
		TreeNode<pair<K, E>> *prevprev = currentNode, *prev = currentNode->rightChild, *curr = currentNode->rightChild->leftChild;
		while (curr)// find the smallest node bigger than currentNode
		{
			prevprev = prev;
			prev = curr;
			curr = curr->leftChild;
		}
		currentNode->data = prev->data;
		if (prevprev == currentNode)
			prevprev->rightChild = prev->rightChild;
		else
			prevprev->leftChild = prev->rightChild;
		delete prev;
	}
}