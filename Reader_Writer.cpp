#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
CRITICAL_SECTION mutex;
CRITICAL_SECTION mutexr;
int ReadCount;
int wnum[100][3];
int rnum[100][3];
int Numr = 0;
int Numw = 0;
DWORD WINAPI Reader(LPVOID pArg)
{
	    Sleep(rnum[(int)pArg][1]*1000);
		printf("\n����%d�ȴ���......", rnum[(int)pArg][0]);
		EnterCriticalSection(&mutexr);
		if (ReadCount == 0)
			EnterCriticalSection(&mutex);
		ReadCount++;
		LeaveCriticalSection(&mutexr);
		printf("\n����%d���ڶ�......", rnum[(int)pArg][0]);
		Sleep(rnum[(int)pArg][2]*1000);
		printf("\n����%d������......", rnum[(int)pArg][0]);
		EnterCriticalSection(&mutexr);
		ReadCount--;
		if (ReadCount == 0)
			LeaveCriticalSection(&mutex);
		LeaveCriticalSection(&mutexr);
	return 0;
}
 
DWORD WINAPI Writer(LPVOID pArg)
{
	Sleep(wnum[(int)pArg][1] * 1000);
	printf("\n\t\t\tд��%d�ȴ�д......", wnum[(int)pArg][0]);
	EnterCriticalSection(&mutex);
	printf("\n\t\t\tд��%d����д......", wnum[(int)pArg][0]);
	Sleep(wnum[(int)pArg][2] * 1000);
	printf("\n\t\t\tд��%d����д......", wnum[(int)pArg][0]);
	LeaveCriticalSection(&mutex);
	return 0;
}
int main()
{
	char ch;
	FILE *p;
	int tmp=0;
	p = fopen("test.txt", "r");
	fscanf(p, "%d", &tmp); 
	while (tmp != EOF)
	{
		ch = getc(p);
		while (!(ch == 'W' || ch == 'R'))
		{
			if (ch == EOF)
				goto T;
			ch = getc(p);
		}
		if (ch == 'W')
		{
			wnum[Numw][0] = tmp;
			fscanf(p, "%d", &wnum[Numw][1]);
			fscanf(p, "%d", &wnum[Numw][2]);
			Numw++;
		}
		else if (ch == 'R')
		{
			rnum[Numr][0] = tmp;
			fscanf(p, "%d", &rnum[Numr][1]);
			fscanf(p, "%d", &rnum[Numr][2]);
			Numr++;
		}
		fscanf(p, "%d", &tmp);
	}
T:;
	/*for (int i = 0; i < Numr; i++)
	{
		printf("%d", rnum[i][0]);
		printf("%d", rnum[i][1]);
		printf("%d", rnum[i][2]);
	}*/
 
 
	ReadCount = 0;
 
	InitializeCriticalSection(&mutex);
 
	InitializeCriticalSection(&mutexr);
 
	HANDLE WThread[20];
	HANDLE RThread[20];
 
	for (int i = 0; i<Numr; i++)
		RThread[i] = CreateThread(NULL, 0, Reader, (LPVOID)i, 0, NULL);//�������߳�
	for (int i = 0; i<Numw; i++)
		WThread[i] = CreateThread(NULL, 0, Writer, (LPVOID)i, 0, NULL);//����д�߳�
 
	WaitForMultipleObjects(Numr, RThread, TRUE, INFINITE);
	WaitForMultipleObjects(Numw, WThread, TRUE, INFINITE);
 
 
	system("pause");
	return 0;
}
