/***********************************************************************
** Date: 		5/30/22
** Project :	test_driver.cpp
** Programers:	Jiahao Liang
** File:		test_driver.cpp
** Purpose:
** Notes:
***********************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include "adjacency_matrix.h"
#include "adjacency_list.h"

int num = 20;

int main(int argc, char *argv[])
{
	std::vector<double> num = {1, 2, 3, 4, 5, 6};
	AdjacencyMatrix<double, true, double> test(num);
	test.addEdge(1, 2, 0.1);
	test.addEdge(1, 3, 0.2);
	test.addEdge(2, 4, 0.3);
	test.addEdge(1, 5, 0.5);
	test.addEdge(4, 5, 0.2);
	test.addEdge(2, 6, 0.4);
	test.DFS(2);
	test.DFSInConnectedComponents();
	test.BFS();
	test.BFSInConnectedComponents();
	test.dijkstraPath(1);
	return 0;
}