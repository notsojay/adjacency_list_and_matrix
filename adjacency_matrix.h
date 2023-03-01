/***********************************************************************
** Date: 		5/28/22
** Project :	adjacency_matrix.h
** Programers:	Jiahao Liang
** File:		adjacency_matrix.h
** Purpose:		The class adjacency_matrix.h, which implement the adjacency_matrix.h ADT
** Notes:		
***********************************************************************/

#pragma once
#ifndef _ADJACENCY_MATRIX_H_
#define _ADJACENCY_MATRIX_H_

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <queue>
#include <unordered_map>

template<class T, class W>
struct DijkstraNode
{
	T currentVertex;
	T previousVertex;
	W weight;
	DijkstraNode(const T & currentVertex = T(), const T & previousVertex = T(), const W & weight = W()):currentVertex(currentVertex), previousVertex(previousVertex), weight(weight)
	{
		
	}
};

template<class T, class W>
class DijkstraNodeComparator
{
public:
	bool operator()(DijkstraNode<T, W> elem1, DijkstraNode<T, W> elem2)
	{
		return elem1.weight > elem2.weight;
	}
};

/*
**	T for valueType;
**	Direction=false for undirected graphs and vice versa for directed graphs;
**	W for weightType of edge;
*/
template<class T, bool Direction=false, class W=int>
class AdjacencyMatrix
{
public:
	AdjacencyMatrix(const int& capacity, const T* vertexs);
	AdjacencyMatrix(const std::vector<T>& vertexs);
	AdjacencyMatrix(const std::vector<std::pair<T, T>>& edge);
	AdjacencyMatrix(const AdjacencyMatrix<T, Direction, W>* const another);
	~AdjacencyMatrix();
	int getVertexCount() const;
	int getEdgeCount() const;
	bool isDirected() const;
	bool isEdge(const T& srcVertex, const T& dstVertex) const;
	void addEdge(const T& srcVertex, const T& dstVertex, const W& weight = 1);
	void eraseEdge(const T& srcVertex, const T& dstVertex);
	void DFS(const T& srcVertex) const;
	void DFS() const;
	void DFSInConnectedComponents() const;
	void BFS(const T& srcVertex) const;
	void BFS() const;
	void BFSInConnectedComponents() const;
	void dijkstraPath(T secVertex);
	void printMatrix() const;
	void clear();
	
private:
	int LocateVertexIndex(const T& vertex) const;
	void DFS(size_t srcIndex, std::vector<int>& visited) const;
	void BFSInConnectedComponents(size_t srcIndex, std::vector<int>& visited) const;
	
private:
	std::vector<T>* vertexArray;
	std::vector<std::vector<W>>* edgeMatrix;
	int edgeCount;
};

template<class T, bool Direction, class W>
AdjacencyMatrix<T, Direction, W>::AdjacencyMatrix(const int& capacity, const T* vertexs)
:	vertexArray(new std::vector<int>(capacity))
,	edgeMatrix(new std::vector<std::vector<int>>())
,	edgeCount(0)
{
	this->vertexArray->reserve(capacity);
	for(size_t i = 0; i < capacity; ++i)
	{
		vertexArray->push_back(vertexs[i]);
	}
	this->edgeMatrix->resize(capacity);
	for(auto &i : *edgeMatrix)
	{
		i.resize(capacity, W());
	}
}

template<class T, bool Direction, class W>
AdjacencyMatrix<T, Direction, W>::AdjacencyMatrix(const std::vector<T>& vertexs)
:	vertexArray(new std::vector<T>(vertexs))
,	edgeMatrix(new std::vector<std::vector<W>>(vertexs.size()))
,	edgeCount(0)
{
	for(auto &i : *edgeMatrix)
	{
		i.resize(vertexArray->size(), W());
	}
}

template<class T, bool Direction, class W>
AdjacencyMatrix<T, Direction, W>::AdjacencyMatrix(const AdjacencyMatrix<T, Direction, W>* const another)
:	vertexArray(new std::vector<T>(another->vertexArray))
,	edgeMatrix(new std::vector<std::vector<W>>(another->edgeMatrix))
,	edgeCount(another->edgeCount)
{}

template<class T, bool Direction, class W>
AdjacencyMatrix<T, Direction, W>::~AdjacencyMatrix()
{
	clear();
}

template<class T, bool Direction, class W>
int
AdjacencyMatrix<T, Direction, W>::getVertexCount() const
{
	return vertexArray->size();
}

template<class T, bool Direction, class W>
int
AdjacencyMatrix<T, Direction, W>::getEdgeCount() const
{
	return edgeCount;
}

template<class T, bool Direction, class W>
bool
AdjacencyMatrix<T, Direction, W>::isDirected() const
{
	return Direction;
}

template<class T, bool Direction, class W>
bool
AdjacencyMatrix<T, Direction, W>::isEdge(const T& srcVertex, const T& dstVertex) const
{
	int srcIndex = LocateVertexIndex(srcVertex);
	int dstIndex = LocateVertexIndex(dstVertex);
	if(srcIndex == -1 || dstIndex == -1) return false;
	if(Direction)
		return edgeMatrix->at(srcIndex).at(dstIndex) != W();
	return edgeMatrix->at(srcIndex).at(dstIndex) != W() || edgeMatrix->at(dstIndex).at(srcIndex) != W();
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::addEdge(const T& srcVertex, const T& dstVertex, const W& weight)
{
	int srcIndex = LocateVertexIndex(srcVertex);
	int dstIndex = LocateVertexIndex(dstVertex);
	if(srcIndex == -1 || dstIndex == -1) return;
	edgeMatrix->at(srcIndex).at(dstIndex) = weight;
	++edgeCount;
	if(!Direction)
		edgeMatrix->at(dstIndex).at(srcIndex) = weight;
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::eraseEdge(const T& srcVertex, const T& dstVertex)
{
	int srcIndex = LocateVertexIndex(srcVertex);
	int dstIndex = LocateVertexIndex(dstVertex);
	if(srcIndex == -1 || dstIndex == -1) return;
	edgeMatrix->at(srcIndex).at(dstIndex) = W();
	--edgeCount;
	if(!Direction)
		edgeMatrix->at(dstIndex).at(srcIndex) = W();
}

template<class T, bool Direction, class W>
int
AdjacencyMatrix<T, Direction, W>::LocateVertexIndex(const T& vertex) const
{
	for(size_t i = 0; i < vertexArray->size(); ++i)
	{
		if(vertexArray->at(i) == vertex)
			return i;
	}
	return -1;
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::DFS(const T& srcVertex) const
{
	int srcIndex = LocateVertexIndex(srcVertex);
	if(srcIndex == -1) return;
	std::vector<int> visited(vertexArray->size(), false);
	std::cout << "DFS: ";
	DFS(srcIndex, visited);
	std::cout << '\n';
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::DFS() const
{
	std::vector<int> visited(vertexArray->size(), false);
	std::cout << "DFS: ";
	DFS(0, visited);
	std::cout << '\n';
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::DFS(size_t srcIndex, std::vector<int>& visited) const
{
	std::cout << vertexArray->at(srcIndex) << " ";
	visited[srcIndex] = true;
	for(size_t i = 0; i < vertexArray->size(); ++i)
	{
		if(visited[i] == false && edgeMatrix->at(srcIndex).at(i) != W())
			DFS(i, visited);
	}
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::DFSInConnectedComponents() const
{
	std::vector<int> visited(vertexArray->size(), false);
	std::cout << "DFS in Connected Components: ";
	for(size_t i = 0; i < vertexArray->size(); ++i)
	{
		if(visited[i] == false)
			DFS(i, visited);
	}
	std::cout << '\n';
}


template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::BFS(const T& srcVertex) const
{
	int srcIndex = LocateVertexIndex(srcVertex);
	if(srcIndex == -1) return;
	std::vector<int> visited(vertexArray->size(), false);
	std::queue<int> q;
	q.push(srcIndex);
	visited[srcIndex] = true;
	std::cout << "BFS: ";
	while(!q.empty())
	{
		int front = q.front();
		std::cout << vertexArray->at(front) << " ";
		q.pop();
		for(size_t i = 0; i < vertexArray->size(); ++i)
		{
			if(visited[i] == false && edgeMatrix->at(front).at(i) != W())
			{
				q.push(i);
				visited[i] = true;
			}
		}
	}
	std::cout << '\n';
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::BFS() const
{
	std::vector<int> visited(vertexArray->size(), false);
	std::queue<int> q;
	q.push(0);
	visited[0] = true;
	std::cout << "BFS: ";
	while(!q.empty())
	{
		int front = q.front();
		std::cout << vertexArray->at(front) << " ";
		q.pop();
		for(size_t i = 0; i < vertexArray->size(); ++i)
		{
			if(visited[i] == false && edgeMatrix->at(front).at(i) != W())
			{
				q.push(i);
				visited[i] = true;
			}
		}
	}
	std::cout << '\n';
}

template<class T, bool Direction, class W>
void 
AdjacencyMatrix<T, Direction, W>::BFSInConnectedComponents() const
{
	std::vector<int> visited(vertexArray->size(), false);
	std::cout << "BFS in Connected Components: ";
	for(size_t i = 0; i < vertexArray->size(); ++i)
	{
		if(visited[i] == false)
			BFSInConnectedComponents(i, visited);
	}
	std::cout << '\n';
}

template<class T, bool Direction, class W>
void 
AdjacencyMatrix<T, Direction, W>::BFSInConnectedComponents(size_t srcIndex, std::vector<int>& visited) const
{
	std::queue<int> q;
	q.push(srcIndex);
	visited[srcIndex] = true;
	while(!q.empty())
	{
		int front = q.front();
		std::cout << vertexArray->at(front) << " ";
		q.pop();
		for(size_t i = 0; i < vertexArray->size(); ++i)
		{
			if(visited[i] == false && edgeMatrix->at(front).at(i) != W())
			{
				q.push(i);
				visited[i] = true;
			}
		}
	}
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::printMatrix() const
{
	for(size_t i = 0; i < edgeMatrix->size(); ++i)
	{
		for(size_t j = 0; j < edgeMatrix->at(i).size(); ++j)
		{
			std::cout << edgeMatrix->at(i).at(j) << " ";
		}
		std::cout << '\n';
	}
}

template<class T, bool Direction, class W>
void
AdjacencyMatrix<T, Direction, W>::clear()
{
	if(vertexArray != nullptr)
	{
		delete vertexArray;
		vertexArray = nullptr;
	}
	if(edgeMatrix != nullptr)
	{
		delete edgeMatrix;
		edgeMatrix = nullptr;
	}
}


template<class T, bool Direction, class W>
void 
AdjacencyMatrix<T, Direction, W>::dijkstraPath(T srcVertex)
{
	std::vector<W> distance(vertexArray->size(), std::numeric_limits<W>::max());
	std::vector<T> prevVertex(vertexArray->size(), T());
	std::priority_queue<DijkstraNode<T, W>, std::vector<DijkstraNode<T, W>>, DijkstraNodeComparator<T, W>> unVisited;
	
	for(const auto &i : *vertexArray)
	{
		unVisited.push(DijkstraNode<T, W>(i, T(), std::numeric_limits<W>::max()));
	}
	distance.at(LocateVertexIndex(srcVertex)) = 0;
	while(!unVisited.empty())
	{
		DijkstraNode<T, W> current = unVisited.top();
		unVisited.pop();
		for(int i = 0; i < vertexArray->size(); ++i)
		{
			if(isEdge(current.currentVertex, vertexArray->at(i)))
			{
				W edgeWeight = edgeMatrix->at(LocateVertexIndex(current.currentVertex)).at(i);
				W alternativePathDistance = distance.at(LocateVertexIndex(current.currentVertex)) + edgeWeight;
				if(alternativePathDistance < distance[i])
				{
					distance.at(i) = alternativePathDistance;
					current.previousVertex = vertexArray->at(i);
					current.weight = alternativePathDistance;
				}
			}
		}
	}
	for(size_t i = 0; i < vertexArray->size(); ++i)
	{
		std::cout << "Dijkstra shortest distance: "<< srcVertex << " -> " << vertexArray->at(i) << ": " << distance[i] << '\n';
	}

}

#endif