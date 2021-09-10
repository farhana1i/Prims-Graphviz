#pragma once
#include<iostream>
#include<list>
#include<vector>
#include<fstream>
#include<queue>
#include<string>
#include<stdlib.h>
#include"MinHeap.h"


struct Vertex {
	int weight;
	int ID;				// vertex ID
	std::string type;	// type of vertex

	// constructors
	Vertex(int ID = 0, std::string type = "", int weight = 0) 
	{
		this->ID = ID;
		this->type = type;
		this->weight = weight;
	}
	Vertex(const Vertex& v) 
	{
		this->ID = v.ID;
		this->type = v.type;
		this->weight = v.weight;
	}

	Vertex operator = (const Vertex& v) 
	{
		if (&v != this) {
			this->ID = v.ID;
			this->type = v.type;
			this->weight = v.weight;
		}
		return *this;
	}
	
	// return true, if two vertices are equal
	bool operator == (const Vertex& v) {
		return (this->ID == v.ID);
	}
};

class Graph 
{
private:
	
	std::vector<std::list<Vertex*>> adjList;		// list of all vertexes
	int totalEdges;									// total number of edges in a connected graph
	
	// to increase allocated/reserved space of vertices list
	inline void reserveSpace(int size)
	{
		assert(size > 0);
		adjList.reserve(this->adjList.size() + size);
	}

	// to deeply copy all vertices and edges
	void copyAdjList(std::vector<std::list<Vertex*>> desAdjList) 
	{
		for (unsigned int vertexInd = 0; vertexInd < desAdjList.size(); ++vertexInd)
		{
			std::list<Vertex*>::iterator adjVertex = desAdjList[vertexInd].begin();
			for (; adjVertex != desAdjList[vertexInd].end(); ++adjVertex)			// copy all adjacent nodes
			{
				this->adjList[vertexInd].push_back(new Vertex(*(*(adjVertex))));
			}
		}
	}

	// to delete graph data
	void deleteGraph() 
	{
		this->totalEdges = 0;
		// to deallocate memory
		for (std::list<Vertex*> v : adjList) 
		{
			if (adjList.size() > 0) 
			{
				std::list<Vertex*>::iterator itr = v.begin();
				for (; itr != v.end(); ++itr) 
				{
					if ((*(itr)) != nullptr) delete (*(itr));
				}
			}
		}
	}

public:
	// constructors
	Graph(unsigned int size = 1) 
	{
		assert(size > 0);
		adjList.reserve(size);
		this->totalEdges = 0;
	}
	Graph(const Graph& g)
	{
		this->adjList.resize(g.adjList.size());
		copyAdjList(g.adjList);
		this->totalEdges = g.totalEdges;
	}

	Graph operator = (const Graph& g) 
	{
		if (&g != this) 
		{
			this->deleteGraph();		// delete data of the graph
			this->adjList.resize(g.adjList.size());
			copyAdjList(g.adjList);
			this->totalEdges = g.totalEdges;
		}
		return *this;
	}

	// to add vertex in a graph
	void addVertex(int vID, std::string vType) 
	{
		assert(vID >= 0);
		std::list<Vertex*> newVertex;
		newVertex.push_back(new Vertex(vID, vType));
		
		if (vID >= adjList.size()) adjList.resize(vID + 1);	// if ID is out of bounds

		adjList.at(vID) = (newVertex);
	}

	// to add an edge b/w two vertices
	void addEdge(int start_ID, int end_ID, int weight) 
	{
		assert(start_ID >= 0 && start_ID < adjList.size() && end_ID >= 0 && end_ID < adjList.size());
		
		std::list<Vertex*>* start = &(adjList[start_ID]); // reference of list of start node
		std::list<Vertex*>* end = &(adjList[end_ID]);	// reference of list of end node

		if (start->size() <= 0 || end->size() <= 0) return;

		// update adjacent nodes
		Vertex* adjA = new Vertex(*(*(end->begin()))), *adjB = new Vertex(*(*(start->begin())));
		adjA->weight = adjB->weight = weight;		// update weight of an edge
		++totalEdges;

		// since graph is undirected, build a 2-way edge
		start->push_back(adjA);
		end->push_back(adjB);
	}

	// to get weight of an edge (start_ID, end_ID)
	const int getWeight(int start_ID, int end_ID) 
	{
		assert(start_ID >= 0 && start_ID < adjList.size() && end_ID >= 0 && end_ID < adjList.size());

		if (adjList[start_ID].size() <= 0) return 0;

		std::list<Vertex*>::iterator itr = adjList[start_ID].begin();
		for (; itr != adjList[start_ID].end(); ++itr) {
			if ((*(itr))->ID == end_ID) return (*(itr))->weight;
		}
		return 0;
	}

	// return total number of vertices
	int getTotalVertices() { return adjList.size(); }
	
	// return total number of edges
	int getTotalEdges() { return this->totalEdges; }
	
	// BFS traversal to calculate total wight of a graph
	// Total Weight = Sum of weight of all edges
	int calculateWeight(int start_ID = 0)
	{
		assert(start_ID >= 0 && start_ID < adjList.size());

		int V = this->adjList.size();  // total number of vertices
		bool* visited = new bool[V];

		for (int i = 0; i < V; ++i) visited[i] = false;	// mark all nodes as unvisited
		int startVertexInd = start_ID;
		
		std::queue<int> vQue;
		vQue.push(startVertexInd);
		int totalWeight = 0;	// to store total weight of the graph

		visited[startVertexInd] = true;
		while (!vQue.empty()) 
		{
			int currVertex = vQue.front();
			vQue.pop();
			// adjacent nodes
			std::list<Vertex*>::iterator itr = adjList[currVertex].begin();
			for (; itr != adjList[currVertex].end(); ++itr) {
				if (!visited[(*(itr))->ID]) {
					totalWeight += (*(itr))->weight;
					visited[(*(itr))->ID] = true;
					vQue.push((*(itr))->ID);
				}
			}
		}
		return totalWeight;
	}

	// to get minimum spanning tree
	friend void primMST(Graph& g, Graph& MST, int startVertexIndex = 0)
	{
		assert(startVertexIndex >= 0 && startVertexIndex < g.adjList.size());
		int V = g.adjList.size();				// total number of vertices
		int* weightkeyList = new int[V];		// weight key for all vertices (INFINITY except the 0th one)
		bool* isInHeap = new bool[V];			// keep track of presence of all vertices in heap
		int* parent = new int[V];				// to keep ID of parent node of all the vertices
		MinHeap<int, int> minHeap(V);			// minHeap of vertices, [Vertex ===> (key, vertexIndex)]

		for (int i = startVertexIndex; i < V; ++i) 
		{
			weightkeyList[i] = INT_MAX;			// initially set weight of all vertices to inifity
			isInHeap[i] = true;					// initially all nodes are in min heap

			if (g.adjList[i].size() > 0) {
				minHeap.insert(weightkeyList[i], (*(g.adjList[i].begin()))->ID); // if there is node at ith index, push vertex/node in heap
			}
			parent[i] = -1;						// initially we have no information about the parent of any node 
		}

		weightkeyList[startVertexIndex] = 0;	// set weight of start node to zero
		while (!minHeap.isEmpty()) 
		{
			int currVertexID = -1;				
			minHeap.getMinimum(currVertexID);	// extract minimum node
			minHeap.deleteMin();
			assert (currVertexID >= 0);
			isInHeap[currVertexID] = false;		// marked false as it is not in heap now
	
			// if there is atleast one node at [currVertexID] index of adjacency list, add this vertex to MST
			if (g.adjList[currVertexID].size() > 0) {
				MST.addVertex(currVertexID, (*(g.adjList[currVertexID].begin()))->type);
			}
			// if there is a parent of current vertex, make an edge b/w parent and child vertex
			if (parent[currVertexID] != -1) {
				MST.addEdge(parent[currVertexID], currVertexID, g.getWeight(parent[currVertexID], currVertexID));
			}
			
			// update weight and min heap
			if (g.adjList[currVertexID].size() > 0) 
			{
				std::list<Vertex*>::iterator itr = g.adjList[currVertexID].begin();
				++itr;
				for (; itr != g.adjList[currVertexID].end(); ++itr) 
				{
					if (isInHeap[(*(itr))->ID] && (*(itr))->weight < weightkeyList[(*(itr))->ID])
					{
						weightkeyList[(*(itr))->ID] = (*(itr))->weight;
						parent[(*(itr))->ID] = currVertexID;
						int adjVertexInd = minHeap.getValueInd((*(itr))->ID);
						if (adjVertexInd == -1) continue;
						minHeap.decreaseKey(adjVertexInd, (*(itr))->weight);
					}
				}
			}
		}
	}


	// to write/print adjacency list of the graph
	friend std::ostream& operator << (std::ostream& _cout, Graph& g) 
	{
		for (int i = 0; i < g.adjList.size(); ++i) 
		{
			if (g.adjList[i].size() > 0) 
			{
				std::list<Vertex*>::iterator itr = g.adjList[i].begin();
				std::cout << "[" << (*itr)->ID << "]";
				++itr;
				for (; itr != g.adjList[i].end(); ++itr) 
				{
					std::cout << " --> ";
					std::cout << "(" << (*(itr))->ID + 1 << ", " << (*(itr))->weight << ")";
				}
				std::cout << "\n";
			}
		}	
		return _cout;
	}

	// to print all edges of a graph
	void printEdges() 
	{
		if (this->adjList.size() <= 0) return;	// no vertex, so no edge

		std::cout << "[(start, end), weight]\n";
		bool* visitedFalgs = new bool[this->adjList.size()];
		
		for (int i = 0; i < this->adjList.size(); ++i)
		{
			visitedFalgs[i] = false;	// mark all nodes as unvisited
		}
		
		for (int i = 0; i < this->adjList.size(); ++i)
		{
			visitedFalgs[i] = true;		// current node is visited now

			if (this->adjList[i].size() <= 1) continue;		// if no edge, move to next node

			std::list<Vertex*>::iterator itr, head;
			itr = head = this->adjList[i].begin();
			++itr;
			for (; itr != this->adjList[i].end(); ++itr) 
			{
				if (!visitedFalgs[(*(itr))->ID])
				{
					std::cout << "[(" << (*head)->ID + 1 << ", " << (*(itr))->ID + 1 << "), " << (*(itr))->weight << "]\n";
				}
			}

		}
	}


	// read data from the file and build a graph
	void inputGraph(std::string filename)
	{
		std::fstream fin(filename);
		if (fin.fail()) std::cout << "OOPS! FAIL TO OPEN FILE!\n";
		else 
		{
			// input vertices information
			int totalVertices = 0, totalEdges = 0;
			fin >> totalVertices;
			this->reserveSpace(totalVertices);		// allocate space
			for (int i = 0; i < totalVertices; ++i) {
				int ID = 0;
				std::string type = "";
				fin >> ID;
				fin.ignore();
				fin.ignore();
				std::getline(fin, type);

				this->addVertex(ID - 1, type);	// add vertex in the graph
			}
			// input edges information
			fin >> totalEdges;
			for (int i = 0; i < totalEdges; ++i) {
				int start_ind = 0, end_ind = 0, weight = 0;

				fin >> start_ind;
				fin.ignore();
				fin >> end_ind;
				fin >> weight;

				this->addEdge(start_ind - 1, end_ind - 1, weight);	// add an edge b/w two nodes
			}
			std::cout << "\nFile Is Loaded Successfully!!!\n";
		}
		fin.close();
	}


	// save graph in a file
	void saveGraph(std::string filename) 
	{
		std::fstream fout;
		fout.open(filename, std::ios::out);
		if (fout.fail()) std::cout << "OOPS! FAIL TO OPEN FILE!\n";
		else
		{
			// save vertices information
			fout << this->adjList.size();
			fout << "\n";
			for (int i = 0; i < adjList.size(); ++i)
			{
				if (this->adjList[i].size() > 0)
				{
					fout << (*(adjList[i].begin()))->ID + 1 << ", " << (*(adjList[i].begin()))->type << "\n";
				}
			}

			// save edges information
			fout << this->totalEdges;
			fout << "\n";
			bool* visitedFalgs = new bool[this->adjList.size()];
			for (int i = 0; i < this->adjList.size(); ++i)
			{
				visitedFalgs[i] = false;	// all nodes are unvisited initially
			}
			for (int i = 0; i < this->adjList.size(); ++i) 
			{
				visitedFalgs[i] = true;	// node is visited now
				
				if (this->adjList[i].size() <= 1) continue;		// if no edge, move to the next node

				std::list<Vertex*>::iterator itr, head;
				itr = head = this->adjList[i].begin();
				++itr;
				for (; itr != this->adjList[i].end(); ++itr) 
				{
					if (!visitedFalgs[(*(itr))->ID]) 
					{
						fout << (*head)->ID + 1 << "," << (*(itr))->ID + 1 << " " << (*(itr))->weight << "\n";
					}
				}
			}
			std::cout << "\nFile Is Saved Successfully!!!\n";
		}
		fout.close();
	}

	// to make .dot file of graph
	// so that we can png file of graph
	// Display png file using ms-paint
	// use cmd commands to deal with ms-paint
	void makeDotFile(std::string filename)
	{
		if (this->adjList.size() <= 0) return;	// no vertex, so no edge

		std::fstream fout(filename);
		if (fout.fail()) 
		{
			std::cout << "OOPS! FAIL TO OPEN FILE\n";
		}
		else 
		{
			std::string syntax = "graph{ \n";

			bool* visitedFalgs = new bool[this->adjList.size()]; // to keep record of visited nodes
			for (int i = 0; i < this->adjList.size(); ++i)
			{
				visitedFalgs[i] = false;	// mark all nodes as unvisited initially
			}

			// visit all one-way edges in string
			for (int i = 0; i < this->adjList.size(); ++i)
			{
				visitedFalgs[i] = true;		// current node is visited now

				if (this->adjList[i].size() <= 1) continue;		// if no edge, move to next node

				std::list<Vertex*>::iterator itr, head;
				itr = head = this->adjList[i].begin();
				++itr;
				for (; itr != this->adjList[i].end(); ++itr)
				{
					if (!visitedFalgs[(*(itr))->ID])
					{
						std::cout << "[(" << (*head)->ID + 1 << ", " << (*(itr))->type << "), " << (*(itr))->ID + 1 << "), " << (*(itr))->weight << "]\n";

						// start node
						std::string vertex1Label = " [label = \"  \nId = \"]\n";	// label of a vertex

						// add label of vertex
						vertex1Label.insert(0, std::to_string((*head)->ID + 1));
						int ind = vertex1Label.find('\"');
						vertex1Label.insert(ind + 1, (*head)->type);
						ind = vertex1Label.find("Id = ");
						vertex1Label.insert(ind + 5, std::to_string((*head)->ID + 1));
						syntax += vertex1Label;


						// end node
						std::string vertex2Label = " [label = \"  \nId = \"]\n";	// label of a vertex

						// add label of vertex
						vertex2Label.insert(0, std::to_string((*itr)->ID + 1));
						ind = vertex2Label.find('\"');
						vertex2Label.insert(ind + 1, (*itr)->type);
						ind = vertex2Label.find("Id = ");
						vertex2Label.insert(ind + 5, std::to_string((*itr)->ID + 1));
						syntax += vertex2Label;

						std::string edge = " --  [label = \"\"]\n";	// append startNode at 0th index

						// add edge information
						edge.insert(0, std::to_string(((*head)->ID) + 1));			// append startNode at 0th index
						ind = edge.find("--");
						edge.insert(ind + 2, std::to_string(((*itr)->ID) + 1));
						ind = edge.find('\"');
						edge.insert(ind + 1, std::to_string((*(itr))->weight));	// append weight of edge
						syntax += (edge);
					}
				}
			}
			syntax.append("}");
			fout << syntax;
			fout.close();

			std::string command = "start /B cmd /c dot -Tpng graphviz.dot > output.png";
			system(command.c_str());
			std::string command2 = "start /B cmd /c mspaint.exe output.png";
			system(command2.c_str());
			std::string command3 = "start /B cmd /c exit";
			system(command3.c_str());
		}
	}
	
	// destructor
	~Graph() {
		this->deleteGraph(); //  to delete data of the graph
	}
};

