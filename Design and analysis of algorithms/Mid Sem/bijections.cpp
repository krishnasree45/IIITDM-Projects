#include<iostream>

#include<cstdio>
using namespace std;
class bijection
{
	public:
		char val[50],b[50],a[50];
		int n,lev;
		int count;
		void visit(int k);
		void enter_values();
		int enumeration(char a[50],char b[50]);
		int fact(int n);
	
};
void bijection::enter_values()
{
	int i,j;
	lev=-1;
	cout<<"enter the size"<<endl;
	cin>>n;
	cout<<"enter the elements for first set"<<endl;
	 for(i=0;i<n;i++)
   	 {
		val[i]=0;
		j=i+1;
		cin>>a[j];
   	 }
	 
	cout<<"enter the elements for second set"<<endl;
	for(i=0;i<n;i++)
	 cin>>b[i];
	visit(0);
	cout<<"the # of bijections are";
	int m=fact(n);
	cout<<m;
}
void bijection::visit(int k)
{
    int i;
	count=0;
   val[k]=++lev;
    if(lev==n)
    {
      
     
	enumeration(a,b);
	
    //  cout<<"\t\t";
    }
    for(i=0;i<n;i++)
    if(val[i]==0)
    visit(i);
    lev--;
    val[k]=0;
    
}

 int bijection::fact(int n){
   if(n==1)
       return 1;
   else
       return(n*fact(n-1));
 }

int bijection::enumeration(char a[50],char b[50])
{
	int i,j;
	for(i=0;i<n;i++)
	 cout<<"("<<a[val[i]]<<","<<b[i]<<")"<<"\t";
	 count++;
	cout<<endl;
	return 0;
}
int main()
{
	freopen("bijection.txt","w",stdout);
	bijection b1;
	b1.enter_values();
	return 0;
}
	
