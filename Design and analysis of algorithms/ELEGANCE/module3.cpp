#include "module0.h"
#include "module3.h"

module0_bfs b1;
module0_dfs d1;

void module3::initialization_dfs()
{
	int nodes;
	int origin,destin,edges;
	cout<<"\nenter the number of nodes in the graph:";
	cin>>nodes;
	d1.create_dfs(nodes);
	cout<<"\nenter the number of edges:";
	cin>>edges;
	int direct;
	cout<<"ur graph is directed/undirected(1/0)"<<endl;
	cin>>direct;
	//max_nodes=nodes*(nodes-1);
	
        for(int i=1;i<=edges;i++)
	     {
	 	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 	 cin>>origin;
	 	 cin>>destin;
	 	 //cout<<"origin="<<origin<<"destin="<<destin;
	         if(origin==-1 && destin==-1)
	 	 break;
		 
		 if(direct==0)
	  	 d1.add_edge(origin,destin);
		 else if(direct==1)
		 d1.add_edge(origin,destin,1);
	     }
		 stack<int> int_s1(50);
		 cout<<"\nthe dfs traversal of the graph is :\n";
		 d1.dfs_traversal(int_s1);

}


void module3::initialization_bfs()
{
	int nodes;
	int origin,destin,edges;
	cout<<"\nenter the number of nodes in the graph:";
	cin>>nodes;
	
	b1.create_bfs(nodes);
	cout<<"\nenter the number of edges:";
	cin>>edges;
	b1.graph_edges=edges;
	b1.tree_edges=0;
	//max_nodes=nodes*(nodes-1);
	b1.tree_edges=0;
	b1.non_tree_edges=0;
	b1.cross_edges=0;
	b1.slanting_edges=0;
	b1.connected_vertices=0;
	int weighted,weight_1;
	cout<<"ur graph is weighted /unweighted"<<endl;
	cin>>weighted;
	for(int i=1;i<=edges;i++)
	{
	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 cin>>origin;
	 cin>>destin;
	 //cout<<"origin="<<origin<<"destin="<<destin;
	 if(origin==-1 && destin==-1)
	 break;
	 if(weighted==1)
	 {
	 cout<<"enter the weight "<<endl;
	 cin>>weight_1;
	 b1.add_edge(origin,destin,weight_1);
	 }
	 else if(weighted!=1)
	 {
	  b1.add_edge(origin,destin,0);
	 }
	}
	 b1.bfs_traversal();
}


void module3::shortest_path_weighted()
{

	int index,index1,num,temp,w;
	int start;
	b1.size=b1.n;
	for(index=1;index<b1.n;index++)
	  {
	    for(index1=index+1;index1<=b1.n;index1++)
	      {
	        if(b1.graph_edge[index][index1]==1)
	         {
		 w=b1.weight[index][index1];
		 num=0.1;
		 temp=index;
		 while(w!=1)
		  {
		     b1.graph_edge[index][index1]=0;
		     b1.graph_edge[index1][index]=0;
		     b1.size++;
		     //name[size]=size;
		     //actual[size]=(index*10)+index1+num;
		     num=num+0.1;
		    b1.graph_edge[temp][b1.size]=1;
		     b1.graph_edge[b1.size][temp]=1;
		     w--;
		     temp=b1.size;
		  }
	        b1. graph_edge[temp][index1]=1;
		 b1.graph_edge[index1][temp]=1;
		}
	      }
	  } 
	

	queue<int> int_q1(50);
	cout << "\nEnter source vertex:";
	cin >> start;
	b1.parent[start]=0;
	cout << "\nthe bfs traversal of the graph:";
	for(index=1;index<=b1.size;index++)
	  b1.visited[index]=0;
	     b1.   bfs(start,int_q1,b1.size);
	 cout<<endl;
	/*for(index=0;index<(size-1);index++)
	  {
	     cout<<int_q1.values[index]<<" ";
	  } */
	cout<<"\n shortest path:" << endl;
	
	for(index=0;index<(b1.size);index++)
	  {
	     print_path(int_q1.values[index],start);
	  } 
}



void module3::print_path(int node,int start)
  {
	if(node<=b1.n)
	      {
		 cout << node;
		
		 int x=b1.parent[node];
		  while(x!=start && x!=0)
		   {
	             while(x>b1.n)
		     {
			x=b1.parent[x];
		     }
		    cout << "<-" << x;
		    x=b1.parent[x];
	          }
		 cout << "<-" << start << endl;
	   }
  }




void module3::tree_longest_path()
{
	int index;
	stack<int> int_s1(50);
	d1.dfs_traversal(int_s1);
	cout<<endl;

	int max=1;
	for( index=2;index<=d1.n;index++)
	 {
	    if(d1.level[index]>d1.level[max])
	     max=index;
	 }
	for(index=1;index<=d1.n;index++)
	 {
	   d1.visited[index]=0;
	   d1.parent[index]=0;
	   d1.level[index]=0;
	 }
	d1.level[max]=1;
	d1.parent[max]=0;
	d1.dfs(max,int_s1,0);
	cout << endl;

	max=1;
	for(index=2;index<=d1.n;index++)
	 {
	    if(d1.level[index]>d1.level[max])
	     max=index;
	 }
	cout << "Longest Path:" << max << " ";
	index=max;
	while(d1.parent[index]!=0)
	  {
	      cout << d1.parent[index] << " ";
	      index=d1.parent[index];
 	  }
	cout << endl;
}

void module3::even_cycle()
  {
	int is_even=0;
	stack<int> int_s1(50);
	d1.initialization_dfs();
	b1.initialization_bfs();
	cout << "Bfs traversal:" << endl;
	b1.bfs_traversal();
	b1.non_tree_edges=b1.n-b1.tree_edges;
	if(b1.non_tree_edges==0)
	{
	 cout<<"\nthe graph do not contain even cycles\n";
	}
	else
	{
	cout<<"\nthe dfs traversal of the graph is :\n";
		d1.dfs_traversal(int_s1);
	 	cout << endl;
	for(int i=1;i<d1.n;i++)
	{
	 for(int j=i;j<=d1.n;j++)
	 {
	  if(d1.back_edge[i][j]==1||d1.back_edge[j][i]==1)
	  {
	   int z=d1.level[i]-d1.level[j];
	   if(z<0)
	     z=-z;
	    if(z%2==1)
	     {
	       is_even=1;
		
	 cout<<"\nthe graph contain even cycles\n";
		break;
	} 
	  }
	 }
	}
	
	if(is_even==0)
	{
	 if(b1.slanting_edges!=0)
	 cout<<"\nthe graph contain even cycles\n";
	 else	
	cout<<"\nthe graph do not contain even cycles\n";
	}
	}
  }

/*int main()
{
	int option;
	do
	{
		cout<<"enter choices\n1:spath_weighted\n2:longest _path_tree\n3:exit\n"<<endl;
		cin>>option;
		switch(option)
		{
			case 1:
				module3 u1;
				u1.initialization_bfs();
				u1.shortest_path_weighted();
				break;
			case 2:
				module3 u2;
				u2.initialization_dfs();
				u2.tree_longest_path();
				break;
			case 3:
				break;
		}
	}while(option!=3);
	return 0;
}*/
