#include "BST.h"

void main() // key(pair���� data.first) = ����� �̸� ���� ����,  element(pair ���� data.second)  = ����� ���� data
{
	//declare the type of K, E
	typedef  int K;
	typedef char E;
	BST<K, E> HWbst;	// Making BST class HWbst  link every nodes

	int menu = 0;
	while (1)
	{
		//   Menu   //
		cout << endl << " #### Choose Menu ####" << endl;
		cout << "1. Insert      2. Delete      3. Print      4. Exit" << endl;
		cin >> menu;
		switch (menu)
		{
			case 1: // Insert Menu
			{
					  
					  pair<K, E> A;  // new node's pair type data
					  cout << " *** ���ο� TreeNode�� Key : ";
					  cin >> A.first;
					  cout << " *** ���ο� TreeNode�� Element : ";
					  cin >> A.second;

					  HWbst.Insert(A); // use insert function
					  break;
					  
			}
			case 2: // Delete Menu
			{	
					  int a = 0;
					  cout << " *** ������ ����� Key �Է�  :  ";
					  cin >> a;
					  HWbst.Delete(a);
					  break;
			}
			case 3: // Print all nodes by level order
			{
					  HWbst.Print();
					  break;
			}
			case 4:  // close this program
			{
					  return;
			}
		}
	}
	
}