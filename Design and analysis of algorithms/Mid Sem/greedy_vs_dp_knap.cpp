#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
int n=10;
using namespace std;
class knapsack
{
	public:
	    	int W,M[100][100],weight[100],profit[100];
		int enter_values();
		int max(int a,int b);
		int knapsackdp(int weight[100],int profit[100],int W);
		//int profit_1[100],W1;
		int m,greedy;
		int greedy_knapsack(int weight[100],int profit[100],int W);

		int a[50],b[50];
};
int knapsack::enter_values()
{
	int i,j,k;
	int weight1[100],profit1[100];
	cout<<"enter the number of objects u wish to enter"<<endl;
	cin>>n;
	W=100;
	//cout<<"enter the maxi weight of the knapsack"<<endl;
	// cin>>W;
	for(k=1;k<=50;k++)
	{
	 cout<<"the weights of the objects"<<endl;
	 for(i=1;i<=n;i++)
	 {
		weight[i]=(rand()%50)+1;
		cout<<weight[i];
		cout<<"\t";
	 }
	 cout<<endl;	
	cout<<" the profits for the objects"<<endl;
	  for(i=1;i<=n;i++)
	 {
		profit[i]=(rand()%50)+1;
		cout<<profit[i];
		cout<<"\t";
	 }
	 cout<<endl;
	 knapsackdp(weight,profit,W);
	 for(i=1;i<=n;i++)
	  weight1[i]=weight[i];
	  cout<<endl;
	 for(i=1;i<=n;i++)
	  profit1[i]=profit[i];
	 cout<<endl;
	 for(i=1;i<=n;i++)
	 {
	  for(j=i+1;j<=n;j++)
	  {
		if(profit[i]<profit[j])
		 {
			int temp=profit[i];
			profit[i]=profit[j];
			profit[j]=temp;
		
			temp=weight[i];
			weight[i]=weight[j];
			weight[j]=temp;
	 	 }
	  }
	}
	 greedy_knapsack(weight,profit,W);
	 cout<<"the weight objects after sorting wrt profit are"<<endl;
	 for(i=1;i<=n;i++)
	  cout<<weight[i]<<"\t";
	 cout<<endl;
	  
	 cout<<"the profit objects after sorting wrt profit are"<<endl;
	 for(i=1;i<=n;i++)
	  cout<<profit[i]<<"\t";
	  cout<<endl;

	cout<<"the elements in the knapsack as per dp  are"<<endl;
	for(i=1;i<=n;i++)
	 cout<<a[i]<<"\t";
	  cout<<endl;
	
	 
	cout<<"maximum profit stored as per dp is: "<<m<<endl;
	
	 cout<<"the elements in the knapsack of greedy are"<<endl;
	 for(i=1;i<=n;i++)
	  cout<<b[i]<<"\t";
	   cout<<endl;
	 cout<<"the profit obtained in knapsack of greedy are"<<endl;
	 cout<<greedy<<endl;
	int count=0;
	
		if(m==greedy)
		 cout<<"yes,the o/p's are equal"<<endl;
	else 
	 cout<<"no"<<endl;
	  cout<<endl;
	  cout<<endl;
	
	
	
	}
	
}

int knapsack::knapsackdp (int weight[100],int profit[100],int W)
{
	int i,j,w;
	
	for(i=1;i<=n;i++)
	 a[i]=0;
	 for(i=0;i<=W;i++)
	   M[0][i]=0;
	 for(i=0;i<=n;i++)
	   M[i][0]=0;
	for(i=1;i<=n;i++)
	{
	  for(w=1;w<=W;w++)
	  {
		if(w<weight[i])
		{
		  M[i][w]=M[i-1][w];
		  a[i]=0;
		}
		else
		{
		  M[i][w]=max(M[i-1][w],profit[i]+M[i-1][w-weight[i]]);
		  if(M[i][w]==profit[i]+M[i-1][w-weight[i]])
		     a[i]=1;
		  else
		     a[i]=0;
		}
	 }
	}
	m= M[n][W];
	return 0;
}

int knapsack::max(int a,int b)
{
	if(a>b)
		return a;
	else
		return b;
}


int knapsack::greedy_knapsack(int weight[100],int profit[100],int W)
{
	int i,j;
	W=100;
	greedy=0;
	int u=W;
	 for(i=1;i<=n;i++)
		b[i]=0;
	for(i=1;i<=n;i++)
	{
	  if(weight[i]>u)
		continue;
	  else 
	   {
		b[i]=1;
		greedy=greedy+profit[i];
		u=u-weight[i];
		//cout<<"weight for i:"<<i<<" "<<u<<endl;
	   }
	}
		
}
	
	 



int main()
{
	freopen("greedy_vs_dp_knap.txt","w",stdout);
	knapsack k1;
	k1.enter_values();
	//return 0;
}
