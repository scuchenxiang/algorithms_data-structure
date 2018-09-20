#include<iostream>
#include<vector>
using namespace std;
class priotyquene
{
private:
	vector<int> list;
public:
	void swap_(int m, int n)
	{
		int tmp = list[m];
		list[m] = list[n];
		list[n] = tmp;
	}
	priotyquene()
	{
		list.push_back(0);
	}
	bool isempty()
	{
		return list.size()-2 == 0;
	}
	void insert(int key)
	{
		list.push_back(key);
		swim(list.size()-2);
	}
	void swim(int point)
	{
		while (point > 1 && compare(point / 2, point))
		{
			swap_(point/2, point);
			point /= 2;
		}
	}
	int compare(int m,int n)
	{
		return list[m] < list[n];
	}
	int demax()
	{
		vector<int>::iterator iter = list.end();
		int max = list[1];
		list[1] = list[list.size() - 1];
		list.erase(iter);
		sink(1);
	}
	void sink(int point)
	{
		while (2*point <= list.size() - 2)
		{
			int j = 2 * point;
			if (j < list.size() - 2 && compare(j, j + 1))
				j++;
			if (!compare(point, j))
				break;
			swap_(point, j);
			point = j;
		}
	}
};

