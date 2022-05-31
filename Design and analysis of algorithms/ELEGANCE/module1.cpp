
#include "module1.h"
#include "module0.h"

module0_bfs c1;
void module1::initialization()
  {
	int nodes;
	int origin,destin,edges;
	cout<<"\nenter the number of nodes in the graph:";
	cin>>nodes;
	
	c1.create_bfs(nodes);
	cout<<"\nenter the number of edges:";
	cin>>edges;
	c1.graph_edges=edges;
	c1.tree_edges=0;
	//max_nodes=nodes*(nodes-1);
	c1.tree_edges=0;
	c1.non_tree_edges=0;
	c1.cross_edges=0;
	c1.slanting_edges=0;
	c1.connected_vertices=0;

	for(int i=1;i<=edges;i++)
	{
	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 cin>>origin;
	 cin>>destin;
	 //cout<<"origin="<<origin<<"destin="<<destin;
	 if(origin==-1 && destin==-1)
	 break;
	 c1.add_edge(origin,destin,0);
	}
	 c1.bfs_traversal();
	
  }

void module1::test_cycle()
{
	//module0_bfs cycle(nodes);
	if(c1.graph_edges>c1.tree_edges)
	{
	 cout<<"\nthe graph contains cycles"<<endl;
	 is_cyclic=1;
	}
	else
	{
	 cout<<"\nthe graph does not contain cycles"<<endl;
	 is_cyclic=0;
	}
}

void module1::test_tree()
{
	//connected_vertices=0;
	//bfs(1);
	test_cycle();
	test_connected();
	if(is_cyclic==0&&is_connected==1)
	 cout<<"\nthe given graph is a tree\n";
	else
	 cout<<"\nthe given graph is not a tree\n";
}

void module1::test_connected()
{
	//cout<<"connected_vertices="<<connected_vertices;
	//module0_bfs connect(nodes);
	if(c1.connected_vertices==c1.n)
	{
	 cout<<"\nthe graph is connected\n";
	 is_connected=1;
	}	
	else
	{
	 cout<<"\nthe graph is not connected\n";
	 is_connected=0;
	}
}

void module1::test_bipartite()
{
	//bfs_traversal();
	//module0_bfs bi(nodes);
	c1.non_tree_edges=c1.n-c1.tree_edges;
	//cout<<"non _tree_edges="<<non_tree_edges<<endl;
	//cout<<"cross_edges="<<(cross_edges)/2<<endl;
	//cout<<"slanting_edges="<<(slanting_edges)/2<<endl;
	if(c1.non_tree_edges!=0)
	{
	 if(c1.cross_edges==0)
	 cout<<"\nthe graph is bipartite\n";
	 else if(c1.cross_edges>0)
	 cout<<"\nthe graph is not bipartite\n";
	 if(c1.slanting_edges>0)
	  cout<<"\nthe graph contains even cycles\n";
	 else if(c1.slanting_edges==0)
	  cout<<"\nthe graph do not contain even cyles\n";
	}
	else if(c1.non_tree_edges==0)
	 cout<<"\nthe graph does not have non tree edges so the graph is not bipartite\n";
	
}

void module1::spath_fixedvertex()
{
	queue<int> int_q1(50);
	int start;
	for(int j=1;j<=c1.n;j++)
	 {
		c1.visited[j]=0;
		c1.parent[j]=0;
	 }
	cout<<"\nenter the starting vertex:";
	cin>>start;
	c1.parent[start]=0;
	cout<<"\nthe bfs traversal of the graph starting from the given vertex:";
	//module0_bfs c3;
	c1.bfs(start,int_q1,c1.n);
	cout<<endl;
	cout<<"\nthe shortest path is:\n";
	for(int i=1;i<=c1.n;i++)
	{
	 cout<<"\nnode="<<i<<" parent="<<c1.parent[i]<<endl;
	}
	
}

void module1::all_spath()
{
	queue<int> int_q1(50);
	for(int i=1;i<=c1.n;i++)
	{
	 for(int j=1;j<=c1.n;j++)
	 {
		c1.visited[j]=0;
		c1.parent[j]=0;
	 }
	c1.parent[i]=0;
	 cout<<"\nbfs traversal of start vertex "<<i<<" :";
	 //module0_bfs c2;
	 c1.bfs(i,int_q1,c1.n);
	 for(int i=1;i<=c1.n;i++)
	 {
	  if(!c1.visited[i])
	  {
		c1.parent[i]=0;
		c1.bfs(i,int_q1,c1.n);
	  }
	 }//to traverse disconnected components as well
	 cout<<endl;
	 cout<<"\nthe shortest path is:\n";
	 for(int i=1;i<=c1.n;i++)
	 {
	  cout<<"\nnode="<<i<<" parent="<<c1.parent[i]<<endl;
	 }
	
	}
}//to find shortest path from every vertex to the entire graph

/*int main()
  {
     int option;
     do
	{
	  cout << "\nChoices\n1.Test_cycle\n2.Test_tree\n3.Test biparitite\n4.Shortest_path_fixed_vertex\n5.all_pair_Spath\nEnter choice:\n";
	  cin >> option;
	  switch(option)
	    {
	        case 1:
			module1 m1;
		        m1.initialization();
			m1.test_cycle();
			break;
		case 2:
			module1 m2;
		        m2.initialization();
			m2.test_tree();
			break;
		case 3:
			module1 m3;
		        m3.initialization();
			m3.test_bipartite();
			break;
		case 4:
			module1 m4;
		        m4.initialization();
			m4.spath_fixedvertex();
			break;
		case 5:
			module1 m5;
		        m5.initialization();
			m5.all_spath();
			break;
		case 0:break;
	    }
	}while(option!=0);
	return 0;
  }*/
