#include "module0.h"
#include "module1.h"
#include "module2.h"
#include "module3.h"
#include "module_4.h"

int main()
{
	
	int c;
	
	
	do
	 {
		cout<<"enter choices\n1:BFS\n2:DFS\n3:test_cycle\n4:test_tree\n5:test_bipartite\n6:shortest_path_fixed_vertex\n7:all_pair_spath\n8:articulation_points\n9:bridges\n10:strongly_connected_components\n11:biconnected_components\n\n12:spath_weighted\n13:test even cycle\n14:longest_path_tree\n15:ramsey_magic\n16:stay_in_btech\n17:exit"<<endl;
		   	cout << "\nEnter your choice:";
	    		cin >> c;
	    	 //cout<<"choice"<<c<<endl;
	        switch(c)
		   {
			case 1:
			     {
				module0_bfs m0_1;
				m0_1.initialization_bfs();
			     }
				break;
		       case 2:
				{
				module0_dfs m0_2;
				m0_2.initialization_dfs();
				}
				break;
			case 3:
				{
				module1 m1_1;
				m1_1.initialization();
				m1_1.test_cycle();
				}
				break;
			case 4:{
				module1 m1_2;
				m1_2.initialization();
				m1_2.test_tree();
				}
				break;
			case 5:
				{
				module1 m1_3;
				m1_3.initialization();
				m1_3.test_bipartite();
				}
				break;
			case 6:
				{
				module1 m1_4;
				m1_4.initialization();
				m1_4.spath_fixedvertex();
				}
				break;
			case 7:
				{
				module1 m1_5;
				m1_5.initialization();
				m1_5.all_spath();
				}
				break;
			case 8:
				{	
				module2 m2_1;
				m2_1.initialization();
				m2_1.print_cutvertices();
				}
				break;
			case 9:{
				module2 m2_2;
				 m2_2.initialization();
				 m2_2.print_cutbridges();
				}
				break;
			case 10:{
				module2 m2_3;
				m2_3.initialization();
				m2_3.print_ssc();
				}
				break;
			case 11:
				{
				module2 m2_4;
				m2_4.initialization();	
				m2_4.print_bcc();
				}
				break;
			case 12:
				{
				module3 m3_1;
				m3_1.initialization_bfs();
				m3_1.shortest_path_weighted();
				}
				break;
			case 13:
				{
				module3 m3_2;
				m3_2.even_cycle();
				}
				break;
			case 14:
				{
				module3 m3_3;
				m3_3.initialization_dfs();
				m3_3.tree_longest_path();
				}
				break;
			
			case 15:
				{
				ramsey r;
				}
				break;
				
			case 16:
				{
				inherit i;
				}
				break;
			case 17:
				break;
		}
	}while(c!=17);
	return 0;
}
