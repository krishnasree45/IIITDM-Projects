# include <iostream>
#include<stdlib.h>
using namespace std;

class schedule
  {
     public:
	void initialization();
	void line(int);
	void print(int,int);
	
	void greedy(int);
	void print_greedy();
     private:
	int f1[50],f2[50],a1[50],a2[50],t1[50],t2[50];
	int e1,e2,x1,x2,n,fopt,lopt,l1[50],l2[50];
	int f3[50],f4[50];
	int l3[50],l4[50];
	int fopt1,lopt1;
  };

void schedule::initialization()
  {
	int i,k;
	srand((unsigned)time(NULL));
	for(k=1;k<=5;k++)
	{
	cout << "n.o of stages are :";
	n=rand()%50+1;
	cout<<n<<endl;
	cout << "\nEnter entry cost:" << endl;
	e1=rand()%50+1;
	e2=rand()%50+1;
	cout<<"first line cost is: "<<e1<<"\tsecond is :"<<e2<<endl;
	cout << "\nthe cost for each satge in line 1:" << endl;
	for(i=1;i<=n;i++)
	{
	 a1[i]=rand()%50+1;
	 cout<<"\t"<<a1[i];
	 }
	cout<<endl;
	cout << "\nthe cost for each satge in line 2:" << endl;
	for(i=1;i<=n;i++)
	 {
	 a2[i]=rand()%50+1;
	 cout<<"\t"<<a2[i];
	 }
	 cout<<endl;
	cout << "\nthe transfer cost for each satge in line 1:" << endl;
	for(i=1;i<n;i++)
	 {
	   t1[i]=rand()%50+1;
	   cout<<t1[i]<<"\t";
	 }
	
	cout << "\nEnter transfer cost for each satge in line 2:" << endl;
	for(i=1;i<n;i++)
	{
	   t2[i]=rand()%50+1;
	   cout<<t2[i]<<"\t";
	 }
	
	cout << "\nEnter exit cost:" << endl;
	x1=rand()%50+1;
	x2=rand()%50+1;
	cout<<"exit cost for 1st line is :"<<x1<<"\t for 2nd is: "<<x2;
	
	
	line(1);
	cout << "f_opt: wrt dp " << fopt << endl;
	cout<<"o/p wrt dp"<<endl;
	print(lopt,n);
	
	greedy(0);
	cout << "f_opt: wrt greedy\t" << fopt1 << endl;
	cout<<"o/p wrt greedy"<<endl;
	print_greedy();
	if(fopt==fopt1)
	 cout<<"both o/p's are equal"<<endl;
	else
	 cout<<"not equal"<<endl;
	}
	
  }

/*int schedule::line(int index)
  {
	if(index==1)
	 {
	  f1[index]=e1+a1[1];
	  f2[index]=e2+a2[1];
	 }
	else if(f1[index]==0 || f2[index]==0)
	  {
	     f1[index]=line1(index);
	     f2[index]=line2(index);
	  }
  }

int schedule::line1(int index)
  {
	 	
  }*/

void schedule::line(int index)
  {
	if(index==1)
	 {
	  f1[index]=e1+a1[1];
	  f2[index]=e2+a2[1];
	 }
	else if(index>n)
	  {
	     if(f1[n]+x1<=f2[n]+x2)
	      {
		lopt=1;
		fopt=f1[n]+x1;
	      }
	     else
	      {
		lopt=2;
		fopt=f2[n]+x2;
	      }
	     return;
	  }
	else
	  {
	     if(f1[index-1]+a1[index]<=f2[index-1]+t2[index-1]+a1[index])
	      {
		f1[index]=f1[index-1]+a1[index];
		l1[index]=1;
	      }
	    else
	      {
		f1[index]=f2[index-1]+t2[index-1]+a1[index];
		l1[index]=2;
	      }
	    if(f2[index-1]+a2[index]<=f1[index-1]+t1[index-1]+a2[index])
	      {
		f2[index]=f2[index-1]+a2[index];
		l2[index]=2;
	      }
	    else
	      {
		f2[index]=f1[index-1]+t1[index-1]+a2[index];
		l2[index]=1;
	      }
	  }
	line(index+1);
  }

void schedule::print(int i,int j)
  {
	if(j==0)
	  return;
	else
	  {
		if(i==1)
		  print(l1[j],j-1);
	        else
		  print(l2[j],j-1);
		cout << "line" << i << " " <<  "station" << j << "\t";
 	  }
	cout<<"\t";
  }

void schedule::greedy(int index)
{
	int i,j;
	//for(i=0;i<=n;i++)
	//{
	//	l3[i]=0;
		//l4[i]=0;
	//}
	if(index==0)
	 {
		if(e1+a1[1]<e2+a2[1])
		{
		  l3[index+1]=1;
		  fopt1=e1+a1[1];
		}
		else
		{
			l3[index+1]=2;
			fopt1=e2+a2[1];
		}
	}
	else if(index==n)
	{
		if(l3[index]==1)
			{
				fopt1=fopt1+x1+a1[index];
				lopt1=1;
				l3[index]=1;
			}
		if(l3[index]==2){
			fopt1=fopt1+x2+a2[index];
				lopt1=2;
				l3[index]=2;
			}
				return ;				
	}
	else
	{
		if(l3[index]==1)
		{
			if(a1[index+1]<a2[index+1]+t1[index])
			{
				l3[index+1]=1;	
				fopt1=fopt1+a1[index+1];
			}
			else
			{
				l3[index+1]=2;
				fopt1=fopt1+a2[index+1]+t1[index];
			}
		}
		if(l3[index]==2)
		{
			if(a2[index+1]<a2[index+1]+t2[index])
			{
				l3[index+1]=1;
				fopt1=fopt1+a2[index+1];
			}
			else
			{
				l3[index+1]=2;
				fopt1=fopt1+a1[index+1];
			}	

		}

	}

	greedy(index+1);
}

void schedule::print_greedy()
{
	for(int i=1;i<=n;i++)
	{
		cout<<l3[i];
		cout<<"\t";
	}
	cout<<endl;
}
int main()
  {
	schedule s;
	s.initialization();
	return 0;
  }
