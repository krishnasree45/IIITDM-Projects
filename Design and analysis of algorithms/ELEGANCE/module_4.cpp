#include "module_4.h"


inherit::inherit ()
{
	int i,j;
	minimum_sem=1;
	counter=0;
	cout<<"enter the number of vertices "<<endl;
	cin>>vertices;
	cout<<"enter the number of edges "<<endl;
	cin>>edges;
	for(i=1;i<=vertices;i++)
	{
		for(j=1;j<=vertices;j++)
		
 		 M[i][j]=0;
	}
	for(int k=1;k<=edges;k++)
	 {	
		cout<<"enter the vertices where the edges are "<<endl;
		 cin>>i>>j;
		M[i][j]=1;
		
	 }
	
	for(i=1;i<=vertices;i++)
	  {
		for(j=1;j<=vertices;j++)
		{
		cout<<M[i][j]<<"\t";
		}
		cout<<endl;
	  }
	
	deg_calc(M);
	for(i=1;i<=vertices;i++)
		count_1[i]=0;
	for(i=1;i<=vertices;i++)
		semester[i]=0;
	int count=0;
	
		
		
	min_sem();	
	for(i=1;i<=vertices;i++)
	 {
		if(semester[i]==0)
			semester[i]=minimum_sem;
	 }	
	
	cout<<"semesters"<<endl;
	for(i=1;i<=vertices;i++)
	  cout<<"semester"<<i<<"="<<semester[i]<<endl;
	cout<<"minimum number of semester required is"<<endl;
		cout<<minimum_sem<<endl;
}

void inherit::min_sem()
{
	
	int i,k;
	if(counter==0)
	{
		
		return;
	}
	int sem[50];
	for(i=1;i<=vertices;i++)
		sem[i]=0;
	for(i=1;i<=vertices;i++)
	{
		
		if(indeg[i]==0&&outdeg[i]==0&&count_1[i]==0)
		 {
				
			sem[i]=minimum_sem;
			
			count_1[i]=1;
		 }
		else if(indeg[i]==0&&count_1[i]==0)	
		{	
			sem[i]=minimum_sem;
			count_1[i]=1;
			
			for(k=1;k<=vertices;k++)
			 {
				M[i][k]=0;
			 }
			
		}
		
	}
	
	for(i=1;i<=vertices;i++)
	{
		if(sem[i]!=0)
			semester[i]=sem[i];
	}
	
	
	
	minimum_sem=minimum_sem+1;
	
	deg_calc(M);	
	
	min_sem();
}

void inherit::deg_calc(int M[50][50])
{
	int i,j;
	counter=0;
	for(i=1;i<=vertices;i++)
	{
		indeg[i]=0;
		outdeg[i]=0;
	}
	for(i=1;i<=vertices;i++)
	{
	  for(j=1;j<=vertices;j++)
	   {
		if(M[i][j]==1)
			counter++;
	   }
	}
	
	for(i=1;i<=vertices;i++)
	{
		for(j=1;j<=vertices;j++)
		{
			indeg[i]=M[j][i]+indeg[i];
			outdeg[i]=M[i][j]+outdeg[i];
		}
	}
	/*cout<<"indegree"<<endl;
	for(i=1;i<=vertices;i++)
		cout<<"indeg["<<i<<"]"<<"="<<indeg[i]<<endl;
	cout<<"outdegree"<<endl;
	for(i=1;i<=vertices;i++)
		cout<<"outdeg["<<i<<"]"<<"="<<outdeg[i]<<endl;*/
	
}




ramsey::ramsey()
  {
	int i,j,k,b[100],count=0,n;
     //cout << "\nEnter size of array:";
     //cin >> n;
	n=6;
     for(i=1;i<=6;i++)
	{
	   b[i]=i;
	}
     k=0;
     for(i=1;i<=n;i++)
	{
	  for(j=1;j<=n;j++)
	    {
		if(b[i]<b[j])
		 {
		   count++;
	 	   d[k]=b[i];
		   c[k]=b[j];
		   k++;
		 }
	    }
	}
    powerset(d,count);
  }

void ramsey::powerset(int d[100],int set_size)
  {
      int r=0,index,index1;
      unsigned int pow_set_size = pow(2, set_size);
      int counter, j;
      for(counter = 0; counter < pow_set_size; counter++)
        {
	  r++;
	  for(index=1;index<=6;index++)
	    {
	      for(index1=1;index1<=6;index1++)
	       {
		g[index][index1]=0;
	       }
	    }
          for(j = 0; j < set_size; j++)
            {
              if(counter & (1<<j))
	        {
		   g[d[j]][c[j]]=1;
		   g[c[j]][d[j]]=1;
		   cout << "(" << d[j] << "," << c[j] << ")" << "(" << c[j] << "," << d[j] << ")";
	        }
	        //cout << "(" << d[j] << "," << c[j] << ")" << "(" << c[j] << "," << d[j] << ")";
            }
	  testing(g);
          cout << endl;
        }
      cout << "Total no: of undirected graphs possible are:" << r << endl;
  }

void ramsey::testing(int graph[10][10])
  {
	int index,index1,j;
	for(index=1;index<=6;index++)
	 {
	   for(index1=index+1;index1<=6;index1++)
	    {
		if(graph[index][index1]==1)
		 {
		    int sum=0;
		    for(j=1;j<=6;j++)
		      {
			 sum=sum+(graph[index][j] & graph[index1][j]);
		      }
		    if(sum>=1)
		      {	
		         cout << "\nThere exist atleast 3 mutual friends" << endl;
		         return;
		      }
		 }
		else
		 {
		    for(j=1;j<=6;j++)
		      {
			if(graph[index][j]==0 && graph[index1][j]==0)
			  {
			    cout << "\nThere exists atleast 3 mutual enemies" << endl;
			    return;
			  }
		      }  
		 }
	   	
	    }
	 }
  }


/*int main()
{
	int a;
	cout<<"enter your choice \n1:ramsey_magic\n2:stay_in_btech"<<endl;
	cin>>a;
	if(a==2)
		inherit i;
	else if(a==1)
		ramsey r;
	return 0;
}*/
