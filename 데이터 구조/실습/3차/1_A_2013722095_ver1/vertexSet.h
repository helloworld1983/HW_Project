#ifndef _VERTEXSET_H_
#define _VERTEXSET_H_

#include <iostream>

class vertexSet
{
private:
	int* m_Parent;
	int m_Size;

public:
	vertexSet(int size)
	{
		m_Parent = new int[size];
		for (int a = 0; a < size; a++)
			m_Parent[a] = -1;
		m_Size = size;
	};
	~vertexSet(){};

	int Find(int ver) // find root
	{
		int r = ver;
		for (; m_Parent[r] >= 0; r = m_Parent[r]);
		while (ver != r)
		{
			int s = m_Parent[ver];
			m_Parent[ver] = r;
			ver = s;
		}
		return r;
	};

	void Union(int v1, int v2) // union with weight rule
	{
		m_Parent[v1] = v2;
	};
};
#endif
