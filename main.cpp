#include "lipton-tarjan.h"
#include <iostream> 
using namespace std;

int main()
{
	bgraph g;
        add_edge(0,1,g); add_edge(1,2,g); add_edge(2,3,g); add_edge(3,0,g); add_edge(3,4,g); add_edge(4,5,g); add_edge(5,6,g); add_edge(6,3,g); add_edge(0,4,g); add_edge(1,3,g);
        add_edge(3,5,g); add_edge(2,6,g); add_edge(1,4,g); add_edge(1,5,g); add_edge(1,6,g); 

	lipton_tarjan(g);
}
