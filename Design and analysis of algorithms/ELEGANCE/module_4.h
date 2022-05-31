#include<iostream>
#include <math.h>
#include<stdlib.h>
using namespace std;

class module4
{
	
	public:
		int vertices,edges;
		int M[50][50];
		int count_1[50];
		int minimum_sem;
		int semester[50];
		//graph();
		//module4();
		int counter;
		
		
};

class inherit:public module4
{
	public:
		inherit();
		int indeg[50],outdeg[50];
		void deg_calc(int M[50][50]);
		void min_sem();
};

class ramsey
{
     public:
	ramsey();
	void powerset(int d[100],int set_size);
	void testing(int graph[10][10]);
     private:
	int d[100],c[100],g[10][10];
 };

