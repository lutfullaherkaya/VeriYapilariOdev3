#include "Graph.h"
#include <iostream>
/*You can add additional libraries here to read and process the dataset*/
#include <fstream> // file read/write

using namespace std;

// tester
int main() {
    
    // read input file
    ifstream inputF;

    // Metal trade Graph
    Graph graph;

    // fstream file("world_trade.txt");
    fstream file("D:\\world_trade.txt");

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
		
    Node turkey(76, "Turkey", "Asia", 31);
    Node honduras(30, "Honduras", "Asia", 312);
	// YOU CAN WRITE YOUR TEST CASES BELOW
    std::list<std::string> path = graph.findLeastCostPath(turkey, honduras);
    std::list<std::string>::const_iterator itr;
    for (itr = path.begin(); itr != path.end(); ++itr) {
        std::cout << *itr << " ";
    }
    
    
    /*
    Graph cizge;
    Node turkiye(31, std::string("turkiye"), std::string("asya"), 31000);
    Node ameriga(52, std::string("amerikabd"), std::string("ameriagga"), 31000);

    cizge.addNode(turkiye);
    cizge.addNode(ameriga);
    cizge.addConnection(turkiye, ameriga, 31313131);
    std::list<std::string> path = cizge.getBFSPath(turkiye, ameriga);
    std::list<std::string>::const_iterator itr;
    for (itr = path.begin(); itr != path.end(); ++itr) {
        std::cout << *itr << " ";
    }
    std::cout << cizge.isCyclic();

    */

    return 0;
}
