#include "Graph.h"
#include <iostream>
/*You can add additional libraries here to read and process the dataset*/
#include <fstream> // file read/write

using namespace std;

// tester
int main() {
    /*
    // read input file
    ifstream inputF;

    // Metal trade Graph
    Graph graph;

    fstream file("world_trade.txt");

    string vertices;
    getline(file, vertices, '\t'); // skip header
    getline(file, vertices, '\n'); // number of vertices

    int numvertices = atoi( vertices.c_str() );
    HashTable<int, Node> nodeTable;

    // Build the graph from the txt input file
    for (int i = 0; i < numvertices; i++) {

        string vid, country, continent, gdp;
        getline(file, vid, '\t');
        getline(file, country, '\t');
        getline(file, continent, '\t');
        getline(file, gdp, '\n');

        Node node(atoi(vid.c_str()), country, continent, atol(gdp.c_str()));
        nodeTable.Insert(node.getVid(), node);

        graph.addNode(node);
    }
    getline(file, vertices, '\n'); // skip header

    // create connections
    string line;
    while (getline(file, line)) {
        int pos = line.find("\t");
        int vid1 = atoi(line.substr(0, pos).c_str());

        line = line.substr(pos+1);
        pos = line.find("\t");
        int vid2 = atoi(line.substr(0, pos).c_str());

        line = line.substr(pos+1);
        long import = atol(line.c_str());

        graph.addConnection(nodeTable.Get(vid1), nodeTable.Get(vid2), import);
    }

    file.close();
		*/
		
	// YOU CAN WRITE YOUR TEST CASES BELOW
	HashTable<int, int> table;
    table.Insert(4,33);
    table.Insert(5,322);
    table.Insert(8,3123);
    table.Insert(1,37777);
    table.yazdir();
    table.Insert(123,33);
    table.Insert(15351234,99993);
    table.Insert(1443,43);
    table.yazdir();



    

    return 0;
}
