# include <iostream>
#include "stack.h"
#include "queue.h"

using namespace std;

class module0_dfs
  {
     public:
	void create_dfs(int n);
	void add_edge(int origin,int destin,int flag=0);
	//void display();
		
	void dfs_traversal(stack<int> &int_s1);
	void dfs(int s,stack<int> &int_s1,int f);
	void dfs_transpose(int s);
	void initialization_dfs();
     
	int n;
		
		int **g;//to store input edges
		int **gtranspose;
		bool *visited;//to traverse the graph 
		int *dfsnum;//to find articulation pts and bridges
		int *low;
		int *parent;
		int *cut_vertex;
		int *cut_bridge_source;
		int *cut_bridge_dest;
		int *level;
		int **back_edge;
		
		int num;
		int cut_bridges;

	
  };

class module0_bfs
  {
     public:
	        void create_bfs(int n);
		void add_edge(int origin,int destin,int w_edge=0);
		void display();
		void initialization_bfs();
		
		void bfs_traversal();
		void bfs(int s,queue<int> &int_q1,int n);
	        int graph_edges,tree_edges;
		int non_tree_edges;

		int cross_edges;
		int slanting_edges;
		int connected_vertices;
    
	int n;
	        //int is_connected;
		//int is_cyclic;
		int size;

		int **graph_edge;
		int **weight;
		int **tree_edge;
		int **non_tree_edge;
		bool *visited;
		int **cross_edge;
		int **slanting_edge;
		int *level;
		int *parent;
  };
