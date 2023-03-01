/***********************************************************************
** Date: 		5/30/22
** Project :	adjacency_list.h
** Programers:	Jiahao Liang
** File:		adjacency_list.h
** Purpose:		The class adjacency_list.h, which implement the adjacency_list.h ADT
** Notes:		
***********************************************************************/

#pragma once
#ifndef _ADJACENCY_LIST_H_
#define _ADJACENCY_LIST_H_

#include <iostream>
#include <vector>
#include <queue>

template<class T, bool Direction=false, class W=int>
class AdjacencyList
{
public:
	class EdgeNode
	{
	public:
		friend class AdjacencyList;
	public:
		EdgeNode(const int& index = int(), const W& weight = W(), EdgeNode* nextNode = nullptr)
		:	index(index)
		,	weight(weight)
		, 	nextNode(nextNode)
		{}
	private:
		int index;
		W weight;
		EdgeNode* nextNode;
	};
	
	class VertexNode
	{
	public:
		friend class AdjacencyList;
	public:
		VertexNode(const T& vertex, const int& index, const W& weight)
		:	vertex(vertex)
		,	head(index, weight)
		{}
		VertexNode(const T& vertex, EdgeNode* head = nullptr)
		:	vertex(vertex)
		,	head(head)
		{}
		VertexNode(const VertexNode& another)
		:	vertex(another.vertex)
		,	head(another.head)
		{}
	private:
		T vertex;
		EdgeNode* head; // points to the next vertex of the first adjacent to that vertex
	};
	
public:
	AdjacencyList(const int& capacity, const T* vertexs);
	AdjacencyList(const std::vector<T>& vertexs);
	AdjacencyList(const AdjacencyList<T, Direction, W>& another);
	~AdjacencyList();
	int getVertexCount() const;
	int getEdgeCount() const;
	bool isDirected() const;
	bool isEdge(const T& srcVertex, const T& dstVertex) const;
	void addVertex(const T& vertex);
	void eraseVertex(const T& vertexToDelete);
	void addEdge(const T& srcVertex, const T& dstVertex, const W& weight = true);
	void eraseEdge(const T& srcVertex, const T& dstVertex);
	void clear();
	void DFS(const T& srcVertex) const;
	void DFS() const;
	void DFSInConnectedComponents() const;
	void BFS(const T& secVertex) const;
	void BFS() const;
	void BFSInConnectedComponents() const;
	AdjacencyList* inverseAdjacencyList();
	
private:
	int LocateVertexIndex(const T& vertex) const;
	EdgeNode* findEdgeNode(const int& srcIndex, const T& targetVertex) const;
	void addEdge(const int& srcIndex, const int& dstIndex, const W& weight, const bool& = true);
	void eraseEdge(const int& srcIndex, const int& dstIndex, const T& srcVertex, const T& dstVertex);
	void DFS(std::vector<int>& visited, size_t srcIndex) const;
	void BFS(std::vector<int>& visited, size_t srcIndex) const;
	
private:
	std::vector<VertexNode*>* vertexList;
	int edgeCount;
};

template<class T, bool Direction, class W>
AdjacencyList<T, Direction, W>::AdjacencyList(const int& capacity, const T* vertexs)
:	vertexList(new std::vector<VertexNode*>(capacity))
,	edgeCount(0)
{
	for(size_t i = 0; i < capacity; ++i)
	{
		vertexList->at(i) = new VertexNode(vertexs[i]);
	}
}

template<class T, bool Direction, class W>
AdjacencyList<T, Direction, W>::AdjacencyList(const std::vector<T>& vertexs)
:	vertexList(new std::vector<VertexNode*>(vertexs.size()))
,	edgeCount(0)
{
	for(size_t i = 0; i < vertexs.size(); ++i)
	{
		vertexList->at(i) = new VertexNode(vertexs.at(i));
	}
}

template<class T, bool Direction, class W>
AdjacencyList<T, Direction, W>::AdjacencyList(const AdjacencyList& another)
:	vertexList(new std::vector<VertexNode*>())
,	edgeCount(another.edgeCount)
{
	for(const auto &i : *another.vertexList)
	{
		vertexList->push_back(new VertexNode(*i));
	}
}

template<class T, bool Direction, class W>
AdjacencyList<T, Direction, W>::~AdjacencyList()
{
	clear();
}

template<class T, bool Direction, class W>
typename AdjacencyList<T, Direction, W>::EdgeNode*
AdjacencyList<T, Direction, W>::findEdgeNode(const int& srcIndex, const T& targetVertex) const
{
	EdgeNode* current = vertexList->at(srcIndex)->head;
	while(current != nullptr)
	{
		if(vertexList->at(current->index)->vertex == targetVertex)
			return current;
		current = current->nextNode;
	}
	return nullptr;
}

template<class T, bool Direction, class W>
int
AdjacencyList<T, Direction, W>::getVertexCount() const
{
	return vertexList->size();
}

template<class T, bool Direction, class W>
int
AdjacencyList<T, Direction, W>::getEdgeCount() const
{
	return edgeCount;
}

template<class T, bool Direction, class W>
bool
AdjacencyList<T, Direction, W>::isDirected() const
{
	return Direction;
}

template<class T, bool Direction, class W>
bool
AdjacencyList<T, Direction, W>::isEdge(const T& srcVertex, const T& dstVertex) const
{
	int srcIndex = LocateVertexIndex(srcVertex);
	if(srcIndex == -1) return false;
	EdgeNode* current = findEdgeNode(srcIndex, dstVertex);
	if(current != nullptr)
		return true;
	return false;
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::addVertex(const T& vertex)
{
	VertexNode* newVertexNode = new VertexNode(vertex);
	vertexList->push_back(new VertexNode(vertex));
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::eraseVertex(const T& vertexToDelete)
{
	int toDeleteIndex = LocateVertexIndex(vertexToDelete);
	EdgeNode* edgeToDelete = vertexList->at(toDeleteIndex)->head->nextNode;
	while(edgeToDelete != nullptr)
	{
		edgeToDelete = vertexList->at(toDeleteIndex)->head->nextNode;
		delete vertexList->at(toDeleteIndex)->head;
		vertexList->at(toDeleteIndex)->head = edgeToDelete;
	}
	std::swap(vertexList->at(toDeleteIndex), vertexList->at(vertexList->size()-1));
	vertexList->pop_back();
	for(size_t i = 0; i < vertexList->size(); ++i)
	{
		eraseEdge(vertexList->at(i)->vertex, vertexToDelete);
	}
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::addEdge(const T& srcVertex, const T& dstVertex, const W& weight)
{
	int srcIndex = LocateVertexIndex(srcVertex);
	int dstIndex = LocateVertexIndex(dstVertex);
	if(srcIndex == -1 || dstIndex == -1) return;
	addEdge(srcIndex, dstIndex, weight, true);
	++edgeCount;
	if(!Direction)
		addEdge(dstIndex, srcIndex, weight, true);
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::addEdge(const int& srcIndex, const int& dstIndex, const W& weight, const bool&)
{
	EdgeNode* newEdge = new EdgeNode(dstIndex, weight);
	newEdge->nextNode = vertexList->at(srcIndex)->head;
	vertexList->at(srcIndex)->head = newEdge;
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::eraseEdge(const T& srcVertex, const T& dstVertex)
{
	int srcIndex = LocateVertexIndex(srcVertex);
	int dstIndex = LocateVertexIndex(dstVertex);
	if(srcIndex == -1 || dstIndex == -1) return;
	eraseEdge(srcIndex, dstIndex, srcVertex, dstVertex);
	--edgeCount;
	if(!Direction)
		eraseEdge(dstIndex, srcIndex, dstVertex, srcVertex);
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::eraseEdge(const int& srcIndex, const int& dstIndex, const T& srcVertex, const T& dstVertex)
{
	EdgeNode*& current = vertexList->at(srcIndex)->head;
	if(vertexList->at(current->index)->vertex == dstVertex)
	{
		if(current->nextNode != nullptr)
		{
			EdgeNode*& toDelete = current;
			vertexList->at(srcIndex)->head = current->nextNode;
			delete toDelete;
			toDelete = nullptr;
			return;
		}
		else if(current->nextNode == nullptr)
		{
			delete vertexList->at(srcIndex)->head;
			vertexList->at(srcIndex)->head = nullptr;
			return;
		}
	}
	while(vertexList->at(current->nextNode->index)->vertex != dstVertex)
		current = current->nextNode;
	EdgeNode*& toDelete = current->nextNode;
	if(toDelete->nextNode != nullptr)
	{
		current->nextNode = toDelete->nextNode;
		delete toDelete;
		toDelete = nullptr;
	}
	else if(toDelete->nextNode == nullptr)
	{
		current->nextNode = nullptr;
		delete toDelete;
		toDelete = nullptr;
	}
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::DFS(const T& srcVertex) const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "DFS: ";
	DFS(visited, LocateVertexIndex(srcVertex));
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::DFS() const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "DFS: ";
	DFS(visited, 0);
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::DFSInConnectedComponents() const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "DFS in Connected Components: ";
	for(size_t i = 0; i < vertexList->size(); ++i)
	{
		if(visited.at(i) == false)
			DFS(visited, i);
	}
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::DFS(std::vector<int>& visited, size_t srcIndex) const
{
	std::cout << vertexList->at(srcIndex)->vertex << " ";
	visited.at(srcIndex) = true;
	EdgeNode* current = vertexList->at(srcIndex)->head;
	while(current != nullptr)
	{
		if(visited.at(current->index) == false)
		{
			DFS(visited, current->index);
		}
		current = current->nextNode;
	}
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::BFS(const T& srcVertex) const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "BFS: ";
	BFS(visited, LocateVertexIndex(srcVertex));
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::BFS() const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "BFS: ";
	BFS(visited, 0);
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::BFSInConnectedComponents() const
{
	std::vector<int> visited(vertexList->size(), false);
	std::cout << "BFS in Connected Components: ";
	for(size_t i = 0; i < vertexList->size(); ++i)
	{
		if(visited.at(i) == false)
		{
			BFS(visited, i);
		}
	}
	std::cout << "\n";
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::BFS(std::vector<int>& visited, size_t srcIndex) const
{
	std::queue<int> q;
	q.push(srcIndex);
	visited.at(srcIndex) = true;
	while(!q.empty())
	{
		int front = q.front();
		q.pop();
		std::cout << vertexList->at(front)->vertex << " ";
		EdgeNode* current = vertexList->at(front)->head;
		while(current != nullptr)
		{
			if(visited.at(current->index) == false)
			{
				q.push(current->index);
				visited.at(current->index) = true;
			}
			current = current->nextNode;
		}
	}
}

template<class T, bool Direction, class W>
AdjacencyList<T, Direction, W>*
AdjacencyList<T, Direction, W>::inverseAdjacencyList()
{
	if(Direction == false)
		return nullptr;
	std::vector<T> vertexs(vertexList->size());
	for(size_t i = 0; i < vertexList->size(); ++i)
		vertexs.at(i) = vertexList->at(i)->vertex;
	AdjacencyList* InAL = new AdjacencyList(vertexs);
	for(size_t i = 0; i < vertexList->size(); ++i)
	{
		EdgeNode* current = vertexList->at(i)->head;
		if(current != nullptr)
			InAL->addEdge(InAL->vertexList->at(current->index)->vertex, InAL->vertexList->at(i)->vertex);
		while(current != nullptr)
		{
			InAL->addEdge(InAL->vertexList->at(current->index)->vertex, InAL->vertexList->at(i)->vertex);
			current = current->nextNode;
		}
	}
	return InAL;
}

template<class T, bool Direction, class W>
int
AdjacencyList<T, Direction, W>::LocateVertexIndex(const T& vertex) const
{
	for(size_t i = 0; i < vertexList->size(); ++i)
	{
		if(vertexList->at(i)->vertex == vertex)
			return i;
	}
	return -1;
}

template<class T, bool Direction, class W>
void
AdjacencyList<T, Direction, W>::clear()
{
	if(vertexList == nullptr)
		return;
	for(auto &i : *vertexList)
	{
		EdgeNode* current = i->head;
		while(current != nullptr)
		{
			current = i->head->nextNode;
			delete i->head;
			i->head = current;
		}
	}
	delete vertexList;
	vertexList = nullptr;
}

#endif
