#include "module0.h"

void module0_bfs::create_bfs(int n)
{
	this->n=n;
	visited=new bool[(6*n)+1];
	level=new int[(6*n)+1];
	graph_edge=new int *[(6*n)+1];
	weight=new int *[6*n+1];
	tree_edge=new int *[6*n+1];
	cross_edge=new int *[6*n+1];
	slanting_edge=new int *[6*n+1];
	parent=new int[6*n+1];
	

	for(int i=1;i<=(6*n);i++)
	{
		//cout<<"entered";
		graph_edge[i]=new int[(6*n)+1];
		weight[i]=new int[6*n+1];
		tree_edge[i]=new int[6*n+1];
		cross_edge[i]=new int[6*n+1];
		slanting_edge[i]=new int[6*n+1];
		
		for(int j=1;j<=(6*n);j++)
		{
			graph_edge[i][j]=0;
			weight[i][j]=0;
			tree_edge[i][j]=0;
			cross_edge[i][j]=0;
			slanting_edge[i][j]=0;
		}
	}
}//initializing an adjacency matrix

void module0_bfs::initialization_bfs()
{
	int nodes,origin,destin,edges,max_nodes;
	module0_bfs g1;
	cout<<"\nenter the number of nodes in the graph:";
		cin>>nodes;
	g1.create_bfs(nodes);

	cout<<"\nenter the number of edges:";
	cin>>edges;
	g1.graph_edges=edges;
	g1.tree_edges=0;
	max_nodes=nodes*(nodes-1);
	g1.tree_edges=0;
	g1.non_tree_edges=0;
	g1.cross_edges=0;
	g1.slanting_edges=0;
	g1.connected_vertices=0;
	for(int i=1;i<=edges;i++)
	{
	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 cin>>origin;
	 cin>>destin;
	 //cout<<"origin="<<origin<<"destin="<<destin;
	 if(origin==-1 && destin==-1)
	 break;
	 g1.add_edge(origin,destin);
	}
	cout << "Bfs traversal:" << endl;
	g1.bfs_traversal();
	cout << endl;	
}


void module0_bfs::add_edge(int origin,int destin,int w_edge)
{
	
	if(w_edge==0)
	{
	//cout<<"n"<<n<<endl;
	//cout<<"origin="<<origin<<"destin="<<destin<<endl;
	if(origin>(n) ||destin>(n)||origin<0||destin<0)
	{
		cout<<"\ninvalid edge\n";
	}
	else
	{
		graph_edge[origin][destin]=1;
		graph_edge[destin][origin]=1;
	}
	}
	else 
	{
	//cout<<"origin="<<origin<<"destin="<<destin<<endl;
	if(origin>(n) ||destin>(n)||origin<0||destin<0)
	{
		cout<<"\ninvalid edge\n";
	}
	else
	{
		graph_edge[origin][destin]=1;
		graph_edge[destin][origin]=1;
		weight[origin][destin]=w_edge;
		weight[destin][origin]=w_edge;	
	}
	}
}//adding edge to a graph

void module0_bfs::bfs_traversal()
{
	queue<int> int_q1(50);
	for(int i=1;i<=n;i++)
	{
		visited[i]=0;
		level[i]=0;
	}
	for(int i=1;i<=n;i++)
	{
	 if(!visited[i])
	 {
		level[i]=1;
		parent[i]=0;
		bfs(i,int_q1,n);
	 }
	}
	
}

void module0_bfs::bfs(int s,queue<int> &int_q1,int n)
{
int v;
	visited[s]=1;
	int_q1.enqueue(s);
	while(!int_q1.isempty())
	{
	 s=int_q1.dequeue();
	 cout<<s<<" ";
	 //visited[s]=1;
	 connected_vertices++;//to check connectedness
	 //cout << "size:" << size << endl;
	 for(v=1;v<=n;v++)
	 {
	  if(graph_edge[s][v]==1)
	  {
		if(visited[v]==0)
		{
		 int_q1.enqueue(v);
		 visited[v]=1;
		 parent[v]=s;
		 level[v]=level[s]+1;//to store the level of each node in bfs traversal tree
		 tree_edge[s][v]=1;
		 tree_edge[v][s]=1;
		 tree_edges++;//to check for cycles
		}
		else if(visited[v]==1)
		{
		 if(level[s]-level[v]==0)
		 {
		  cross_edge[s][v]=1;// to store non tree edges=>to find odd and even cycles 
		  cross_edge[v][s]=1;
		  cross_edges++;
		 }
		 else if(level[v]-level[s]==1)
		 {
		  slanting_edge[s][v]=1;
		  slanting_edge[v][s]=1;
		  slanting_edges++;
		 }
		}
		
	  }
	 }
	}
}


void module0_dfs::create_dfs(int n)
{
	this->n=n;
	visited=new bool[n+1];
	g=new int *[n+1];
	back_edge=new int *[n+1];
	gtranspose=new int *[n+1];
	for(int i=1;i<=n;i++)
	{
		//cout<<"entered";
		g[i]=new int[n+1];
		back_edge[i]=new int [n+1];
		gtranspose[i]=new int[n+1];
		for(int j=1;j<=n;j++)
		{
			g[i][j]=0;
			gtranspose[i][j]=0;
			back_edge[i][j]=0;
		}
	}
	dfsnum=new int[n+1];
	low=new int[n+1];
	parent=new int[n+1];
	cut_vertex=new int[n+1];
	cut_bridge_dest=new int[100];
	cut_bridge_source=new int[100];
	level=new int[100];
	num=0;
	cut_bridges=0;
}//initializing an adjacency matrix

void module0_dfs::initialization_dfs()
{
		int nodes,origin,destin,edges,max_nodes;
		module0_dfs g2;
		 cout<<"\nenter the number of nodes in the graph:";
		cin>>nodes;
		g2.create_dfs(nodes);
		cout<<"\nenter the number of edges:";
		cin>>edges;
		//max_nodes=nodes*(nodes-1);
		for(int i=1;i<=edges;i++)
		{
	 	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 	 cin>>origin;
	 	 cin>>destin;
	 	 //cout<<"origin="<<origin<<"destin="<<destin;
	         if(origin==-1 && destin==-1)
	 	 break;
	  	 g2.add_edge(origin,destin);
		}
		stack<int> int_s1(50);
		cout<<"\nthe dfs traversal of the graph is :\n";
		g2.dfs_traversal(int_s1);
	 	cout << endl;
	      

}

void module0_dfs::add_edge(int origin,int destin,int flag)
{
	//cout<<"origin="<<origin<<"destin="<<destin<<endl;
	if(origin>(n) ||destin>(n)||origin<0||destin<0)
	{
		cout<<"\ninvalid edge\n";
	}
	else if(flag==0)
	{
		g[origin][destin]=1;
		g[destin][origin]=1;
		back_edge[origin][destin]=1;
		back_edge[destin][origin]=1;
	}
	else if(flag==1)
	{
		g[origin][destin]=1;
		gtranspose[destin][origin]=1;
	}
}//adding edge to a graph

void module0_dfs::dfs(int s,stack<int> &int_s1,int f)
{
	if(f==1)
	{
	 //cout<<"enter";
	 int v;
	 int ed;
	 int temp1;
	 int temp2;
	 int children;
	 children=0;
	 visited[s]=1;
	 num=num+1;
	 dfsnum[s]=low[s]=num;
	 for(v=1;v<=n;v++)
	 {
	  if(g[s][v]==1)
	  {
	   if(visited[v]==0)
	   {
	   children++;
	   visited[v]=1;
	   parent[v]=s;
	   ed=(s*10)+v;
	   //cout<<ed<<endl;
	   int_s1.push(ed);
	   dfs(v,int_s1,1);
	   if(low[v]<low[s])
		low[s]=low[v];
	   temp2=(10*s)+v;
	   if((dfsnum[s]==1&&children>1) ||(dfsnum[s]>1&&low[v]>=dfsnum[s]))
	   {
		temp1=int_s1.top_value();
		while(temp1!=temp2)
		{
	 	 cout<<(temp1/10)<<"->"<<(temp1%10)<<" ";
		 int_s1.pop();
		 temp1=int_s1.top_value();
		}
		cout<<(temp1/10)<<"->"<<(temp1%10)<<" ";
		int_s1.pop();
		cout<<endl;
		//count++;
	   }
	 }
	 else if(visited[v]==1&&v!=parent[s] && dfsnum[v]<low[s])
	 {
		if(dfsnum[v]<low[s])
			low[s]=dfsnum[v];
	 	int ed=(s*10)+v;
		//cout<<"ed="<<ed<<" ";
		int_s1.push(ed);
	 }
	}
	}
	}		 
	
	else
	{
	int v;
	int children;
	children=0;
	num=num+1;
	dfsnum[s]=low[s]=num;
	visited[s]=1;
	cout<<s<<" ";//(dfsnum="<<dfsnum[s]<<" l="<<low[s]<<") ";
	for(v=1;v<=n;v++)
	{
	 if(visited[v]==0)
	 {
	  if(g[s][v]==1)
	  {
	  children++;
	  //visited[v]=1;
	  level[v]=level[s]+1;
	  parent[v]=s;
	  back_edge[s][v]=0;
	  back_edge[v][s]=0;
	  dfs(v,int_s1,0);
	  
	  //cout<<"enter="<<v<<" "<<low[v]<<" "<<dfsnum[v]<<" "<<dfsnum[s]<<" ";
	  if(low[v]>=dfsnum[s])
	  {
		//cout<<"\nenter\n";
		cut_vertex[s]=1;
		//cout<<"\ncut vertex="<<s<<endl;
		
		if(low[v]>dfsnum[s])
		{
		 cut_bridges=cut_bridges+1;
		 cut_bridge_source[cut_bridges]=s;
		 cut_bridge_dest[cut_bridges]=v;
		}
	  }
	  if(low[v]<low[s])
		low[s]=low[v];

	  if(parent[s]==0&&children>1)
	  	cut_vertex[s]=1;
	  else if(parent[s]==0&&children<=1)
		cut_vertex[s]=0;//conditions for root to be a cut vertex
	  }
	 
	 }
	 else
	 {
	  
	  if(g[s][v]==1&&parent[s]!=v )//(u,v) is a back edge
	 {
	  if(dfsnum[v]<low[s])
		low[s]=dfsnum[v];
	 }
	 }
	
	}
	int_s1.push(s);//to print strongly connected components
	}
	
	 	
		
}//dfs for each vertex


void module0_dfs::dfs_traversal(stack<int> &int_s1)
{
	for(int i=1;i<=n;i++)
	{
		visited[i]=0;
	}
	for(int i=1;i<=n;i++)
	{
	 if(!visited[i])
	 {
		level[i]=1;
		dfs(i,int_s1,0);
	 }
	}
	//cout<<"parent[1]="<<parent[1]<<endl;
}// dfs traversal for entire grah and is useful when there are disconnected components


void module0_dfs::dfs_transpose(int s)
{
	int v;
	visited[s]=1;
	cout<<s<<" ";
	
	for(v=1;v<=n;v++)
	{
	 if(visited[v]==0&&gtranspose[s][v]==1)
	 {
	  dfs_transpose(v);
	 }
	}
	
}

/*int main()
  {
     int nodes,origin,destin,edges,max_nodes;
	int option,w_edge;
     do
      {
	cout << "\nChoices:\n1.BFS\n2.DFS\n0.exit\nEnter choice" << endl;
	cin >> option;
	switch(option)
	  {
	     case 1:
		{
		cout<<"\nenter the number of nodes in the graph:";
		cin>>nodes;
	module0_bfs g1(nodes);

	cout<<"\nenter the number of edges:";
	cin>>edges;
	g1.graph_edges=edges;
	g1.tree_edges=0;
	max_nodes=nodes*(nodes-1);
	g1.tree_edges=0;
	g1.non_tree_edges=0;
	g1.cross_edges=0;
	g1.slanting_edges=0;
	g1.connected_vertices=0;
	for(int i=1;i<=edges;i++)
	{
	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 cin>>origin;
	 cin>>destin;
	 //cout<<"origin="<<origin<<"destin="<<destin;
	 if(origin==-1 && destin==-1)
	 break;
	 g1.add_edge(origin,destin);
	}
	cout << "Bfs traversal:" << endl;
	g1.bfs_traversal();
	cout << endl;
	}
	break;
	
	     case 2:
	      {
	        cout<<"\nenter the number of nodes in the graph:";
		cin>>nodes;
		module0_dfs g2(nodes);
		cout<<"\nenter the number of edges:";
		cin>>edges;
		//max_nodes=nodes*(nodes-1);
		for(int i=1;i<=edges;i++)
		{
	 	 cout<<"\nenter edge(-1 -1 to exit):"<<endl;
	 	 cin>>origin;
	 	 cin>>destin;
	 	 //cout<<"origin="<<origin<<"destin="<<destin;
	         if(origin==-1 && destin==-1)
	 	 break;
	  	 g2.add_edge(origin,destin);
		}
		stack<int> int_s1(50);
		cout<<"\nthe dfs traversal of the graph is :\n";
		g2.dfs_traversal(int_s1);
	 	cout << endl;
	      }
		break;
	     case 0:break;
	       
	  }
      }while(option!=0);

	return 0;
  }*/
 
