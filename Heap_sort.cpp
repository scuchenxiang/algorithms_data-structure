#include<iostream>
using namespace std;
void swap_(int list[],int a, int b)
{
	int tmp = list[a];
	list[a] = list[b];
	list[b] = tmp;
}
int compare(int list[],int a, int b)
{
	return list[a] <list[b];
}
void sink(int list[],int point,int num)
{
	while (2 * point <= num)
	{
		int j = 2 * (point);
		if (j <num && compare(list,j, j + 1))
			j++;
		if (!compare(list,point, j))
			break;
		swap_(list,point, j);
		point = j;
	}
}
 
void heap_sort(int a[],int m,int n)
{
	for (int i = n / 2 ; i >= 1; i--)
	{
		sink(a, i+m-1, n);
	}
	while (n > m)
	{
		swap_(a, m, n+m-1);
		n--;
		sink(a, m, n+m-1);
	}
}
int main()
{
	int test[10] = { 3,4,7,1,2,666,67,9,2,5 };
	heap_sort(test, 0, 10);
	for (int i = 0; i<10; i++)
	{
		cout << test[i] << "  ";
	}
	
	cin.get();
	cin.get();
	return 0;
}
