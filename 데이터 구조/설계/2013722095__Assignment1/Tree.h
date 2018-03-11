#include <iostream>
#include <stack>
#include <queue>
using namespace std;


template <class T> class Tree;

template <class T>
class TreeNode  // declare Node
{
	friend Tree<T>;
private :
	T data =NULL;
	TreeNode<T>* leftChild =NULL;
	TreeNode<T>* rightChild=NULL;
};

template <class T>
class Tree
{

private:
	TreeNode<T>* root; 
public:
	Tree() // set root to NULL when you make the Tree
	{
		root = NULL;
	};
	~Tree(){};
	void Visit(TreeNode<T>* t) // print Node's data
	{
		cout << t->data << " ";
	};

	/* Inorder */
	// print currentNode's left child first,  currentNode and then print currentNode's right child
	// all traversal activate reculsivly

	void Inorder() // Driver of Inorder
	{
		Inorder(root);
	};
	void Inorder(TreeNode<T>* currentNode) // Workhorse of Inorder
	{
		if (currentNode) // if currentNode's value is NULL, break this function
		{
			Inorder(currentNode->leftChild);// activate Inorder function by using currentNode's leftChild(reculsivly activate)
			Visit(currentNode); // print currentNode
			Inorder(currentNode->rightChild); // activate Inorder function by using currentNode's rightChild(reculsivly activate)
		}
	};
	/* Preorder */
	// all traversal activate reculsivly
	// print currentNode first and print the currentNode's left child and then print currentNode's right child
	
	void Preorder()// Deriver of Preorder
	{
		Preorder(root);
	};
	void Preorder(TreeNode<T>* currentNode)// Workhorse of Preorder
	{
		if (currentNode)// if currentNode's value is NULL, break this function
		{
			Visit(currentNode); // print currentNode's data
			Preorder(currentNode->leftChild); // activate Preorder function by using currentNode's leftChild(reculsivly activate)
			Preorder(currentNode->rightChild); // activate Preorder function by using currentNode's rightChild(reculsivly activate)
		}
	};
	/* Postorder */
	// all traversal activate reculsivly
	// print the currentNode's left child and print currentNode's right child print currentNode 

	void Postorder() // Driver of Postorder
	{
		Postorder(root);
	};
	void Postorder(TreeNode<T>* currentNode) // Workhorse of Postorder
	{
		if (currentNode)// if currentNode's value is NULL, break this function
		{
			Postorder(currentNode->leftChild); // activate Postorder function by using currentNode's leftChild(reculsivly activate)
			Postorder(currentNode->rightChild); // activate Postorder function by using currentNode's rightChild(reculsivly activate)
			Visit(currentNode); // print currentNode
		}
	};
	/*NonrecInorder*/
	// All nodes are save in stack(none reculsive) by Inorder traversal
	void NonrecInorder()
	{
		stack<TreeNode<T>*> s;
		TreeNode<T>* currentNode = root;
		while (1)
		{
			while (currentNode) // if there's no currentNode's leftChild , break this functioin
			{
				s.push(currentNode); // push currentNode to stack s
				currentNode = currentNode->leftChild; // currentNode pointer points its leftChild
			}
			if (s.empty())// if stack is empty,finish NonrecInorder function
				return;
			currentNode = s.top();// Now currentNode is the top node in the stack
			s.pop();// take out the top node in the stack
			Visit(currentNode); // print the node which was taken out
			currentNode = currentNode->rightChild; // currentNode pointer points its rightChild
		}
	}

	/*Levelorder */
	// Print all nodes in level unit
	void Levelorder()
	{
		queue<TreeNode<T>*> q;
		TreeNode<T>* currentNode = root;
		while (currentNode)
		{
			Visit(currentNode);
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
	void DataSet(TreeNode<T>* t) // save name(T type) to Node* t's data;
	{
		T name;
		cout << "**Insert Data : ";
		cin >> name;
		t->data = name;
	}
	void Insert(TreeNode<T>* t)
	{
		if (root == NULL) // if there's no node
		{
			root = t;
			return;
		}
		TreeNode<T>* temp = root;// pointer temp point root
		while (1)// seting New node(t)'s place
		{
			
			if (t->data < temp->data)// if new node(t)'s data value is smaller than  currentnode(temp)
			{
				if (temp->leftChild == NULL)//if currentNode(temp) has no leftChild
				{
					temp->leftChild = t; // currentNode(temp)'s leftChild is new node(t)
					break;
				}
				temp = temp->leftChild;//if currentNode(temp) has leftChild, currentNode is its leftChlid
				continue;// go to the start position of while function
			}
			else//if new node(t)'s data value is bigger than  currentnode(temp)
			{

				if (temp->rightChild == NULL)//if currentNode(temp) has no rightChild
				{
					temp->rightChild = t; // currentNode(temp)'s rightChild is new node(t)
					break;
				}
				temp = temp->rightChild; // /if currentNode(temp) has rightChild, currentNode is its rightChlid
				continue;// go to the start position of while function
			}

		}
		
	};

	class InorderIterator
	{
	private:
		stack<TreeNode<T>*> s;
		TreeNode<T>* currentNode;
	public:
		InorderIterator()
		{
			currentNode = root;
		}
		T* Next()
		{
			while (currentNode)
			{
				s.push(currentNode);
				surrentNode = currentNode->leftChild;
			}
			if (s.empty())
				return 0;
			currentNode = s.top();
			s.pop();
			T& temp = currentNode->data;
			currentNode = currentNode->rightChild;
			return &temp;
		}
	};
};
