#include "module0.h"

#include "module2.h"
module0_dfs d2;

void module2::initialization()
{
	int nodes;
	int origin,destin,edges;
	cout<<"\nenter the number of nodes in the graph:";
		cin>>nodes;
		d2. create_dfs(nodes);
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
	  	 d2.add_edge(origin,destin);
		 else if(direct==1)
		 d2.add_edge(origin,destin,1);
		}
		stack<int> int_s1(50);
		cout<<"\nthe dfs traversal of the graph is :\n";
		d2.dfs_traversal(int_s1);

}

void module2::print_cutbridges()
{
	cout<<"\nfor the given graph the cut bridges are:";
	for(int i=1;i<=d2.cut_bridges;i++)
	{
	 cout<<"("<<d2.cut_bridge_source[i]<<","<<d2.cut_bridge_dest[i]<<") ";
	}
	cout<<endl;
	
}

void module2::print_ssc()
{
	stack<int> int_s1(50);
	d2.dfs_traversal(int_s1);
	cout<<"\nthe strongly connected components are:\n";
	for(int i=1;i<=d2.n;i++)
	{
		d2.visited[i]=0;
	}
	/*while(!int_s1.isempty())
	{
	 cout<<int_s1.pop()<<" ";
	}
	cout<<endl;*/
	while(!int_s1.isempty())
	{
	 //cout<<"enter";
	 int value=int_s1.pop();
	 if(d2.visited[value]==0)
	 {
	  d2.dfs_transpose(value);
	  cout<<endl;
	 }
	 
	}
	
	
}



void module2::print_cutvertices()
{
	cout<<"\nfor the given graph the cut vertices are:";
	for(int i=1;i<=d2.n;i++)
	{
	 if(d2.cut_vertex[i]==1)
	 cout<<i<<" ";
	}
	cout<<endl;
}


void module2::print_bcc()
{
	cout<<"\nthe biconnected components are:\n";
	stack<int> int_s1(50);
	for(int i=1;i<=d2.n;i++)
	{
		d2.visited[i]=0;
	}
	for(int i=1;i<=d2.n;i++)
	{
	 if(!d2.visited[i])
	 {
		d2.parent[i]=0;
		d2.level[i]=1;
		d2.dfs(i,int_s1,1);
	 }
	 int j=0;
	/* while(!int_s1.isempty())
	{
	 cout<<int_s1.pop()<<" ";
	}
	cout<<endl;*/
	/*for(int i=1;i<=n;i++)
	{
	 cout<<i<<" parent="<<parent[i]<<" ";
	}*/
	 while(!int_s1.isempty())
	 {
	  j=1;
	  int temp1=int_s1.pop();
	  cout<<(temp1/10)<<"->"<<(temp1%10)<<" ";
	 }
	 if(j=1)
	 {
	  cout<<endl;
	  //count++;
	 }
	}
	
}



/*int main()
{
	int option;
     do
	{
	   cout<<"enter choices \n1:cut_vertices\n2:cut_bridges\n3:strongly_connected_components\n4:biconnected_components\n5:exit"<<endl;
	   cin>>option;
	   switch(option)
	   {
		case 1:
			module2 l1;
			l1.initialization();
			l1.print_cutvertices();
			break;
		case 2:
			module2 l2;
			l2.initialization();
			l2.print_cutbridges();
			break;
		case 3:
			module2 l3;
			l3.initialization();
			l3.print_ssc();
			break;
		case 4:
			module2 l4;
			l3.initialization();	
			l3.print_bcc();
		case 5:
			break;
	  }
	}while(option!=4);
	return 0;
}*/
