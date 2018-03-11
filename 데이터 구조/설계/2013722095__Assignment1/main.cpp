#include "Tree.h"
void main()
{
	typedef int T;

	Tree<T> HWTree;
	int menu;
	

	while (1)
	{
		cout <<endl<< " 1. Insert     2.  Print  3.   exit" << endl;
		cout << "choose menu : ";
		cin >> menu;

		switch (menu)
		{
		case 1:
		{
				  TreeNode<T> * A = new TreeNode<T>; // creat new node A
				  HWTree.DataSet(A); // set the data of new node
				  HWTree.Insert(A); 
				  break;
		}
		case 2:
		{	
				cout <<endl<< "**** Inorder " << endl;
				HWTree. Inorder();
				cout << endl<<"**** Preorder " << endl;
				HWTree.Preorder();
				cout << endl << "**** Postorder " << endl;
				HWTree.Postorder();
				cout << endl << "**** NonrecInorder " << endl;
				HWTree.NonrecInorder();
				cout << endl << "****Levelnorder " << endl;
				HWTree.Levelorder();
				break;
		}
		case 3:
			return;

		}
	}
}