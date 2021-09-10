#include<iostream>
#include<string>
#include"Graph.h"

int main() 
{ 
	Graph g;

	bool isBreak = false;
	while (!isBreak) 
	{
		std::cout << "-----------------------------------------\n";
		std::cout << "+\tPress 1 To Input Graph\t\t+\n";
		std::cout << "+\tPress 2 To Save Graph\t\t+\n";
		std::cout << "+\tPress 3 To Add Vertex\t\t+\n";
		std::cout << "+\tPress 4 To Add Edge\t\t+\n";
		std::cout << "+\tPress 5 To Print MST\t\t+\n";
		std::cout << "+\tPress 6 To Print Tree\t\t+\n";
		std::cout << "+\tPress Any Other Key To Exit\t+\n";
		std::cout << "-----------------------------------------\n";
		int choice = -1;
		std::cout << ">>";
		std::cin >> choice;
		
		switch (choice) {
		case 1:
		{
			std::string filename = "graph.txt";
			/*std::cout << "Enter Filename: ";
			std::cin >> filename;*/
			g.inputGraph(filename);
			break;
		}
		case 2:
		{
			std::string filename = "graph.txt";
			/*std::cout << "Enter Filename: ";
			std::cin >> filename;*/
			g.saveGraph(filename);
			break;
		}case 3:
		{
			int vID = -1;			// vertex ID
			std::string vType = ""; // vertex type
			std::cout << "Enter Vertex ID: "; std::cin >> vID;
			std::cout << "Enter Vertex Type: "; std::cin >> vType;
			g.addVertex(vID - 1, vType);
			break;
		}
		case 4:
		{
			int start_ID = -1, end_ID = -1, weight = 0;
			std::cout << "Enter Starting Vertex ID: "; std::cin >> start_ID;
			std::cout << "Enter Ending Vertex ID: "; std::cin >> end_ID;
			std::cout << "Enter Vertex Weight: "; std::cin >> weight;
			g.addEdge(start_ID - 1, end_ID - 1, weight);
			break;
		}
		case 5:
		{
			Graph mst(g.getTotalVertices());	// to get minimum spanning tree
			primMST(g, mst);
			mst.makeDotFile("graphviz.dot");
			std::cout << std::endl << "[Total Minimum Weight: " << mst.calculateWeight() << "]\n";
			break;
		}
		case 6:
		{
			std::cout << std::endl << g << std::endl;
			break;
		}
		default:
		{
			isBreak = true;
			break;
		}
		}
		
		system("pause");
		system("cls");
	}


	return 0;
}
