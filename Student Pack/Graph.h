#include <list>
#include <stack>
#include <queue>
#include <vector>

#include "HashTable.h"
#include "Edge.h"
/* Do not add new libraries or files */

using namespace std;

/** An adjacency list representation of a directed weighted graph. */
class Graph {

public:

    // TODO: IMPLEMENT THESE FUNCTIONS.
    /** CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR */
    Graph();
    Graph(const Graph &rhs);
    Graph &operator=(const Graph &rhs);
    ~Graph();

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Adds the given node to the graph with vid or country as key,
     * and an empty edge list as value */
    void addNode(const Node &node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Adds a new edge to the edge list of headNode using tailNode and import data */
    void addConnection(const Node &headNode, const Node &tailNode, int import);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Given a country, returns all adjacent
     * countries of the given country as a list of Node
     * Throw ItemNotFoundException, if the given node does not exist */
    list<Node> getAdjacentNodes(const Node &node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** For the given node, returns the sum of imports */
    long getTotalImports(const Node &node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Deletes the given country from the graph
     * together with its incident connections(edges) */
    void deleteNode(const Node &node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Given two countries , returns the least cost path/route between
     * them using import values as the edge weight */
    list<string> findLeastCostPath(const Node &srcNode, const Node &destNode);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Detect whether the graph contains any cycle */
    bool isCyclic();

    // TODO: IMPLEMENT THIS FUNCTION.
    /** returns the BFS path from srcNode to destNode as a list of country names */
    list<string> getBFSPath(const Node &srcNode, const Node &destNode);

private:
    /** Adjacency list representation of the graph;
    You can change the signature of this variable*/
    HashTable<int, list<Edge>> komsuListeleri;


    // == DEFINE HELPER METHODS & VARIABLES BELOW ==
    HashTable<int, std::string> ulkeIsimleri;
    
    struct MesafeVeDugum {
        long mesafe;
        int dugum;

        MesafeVeDugum(long mesafe=-1, int dugum=-1)
            : mesafe(mesafe), dugum(dugum) {}

        struct Kiyasla {
            bool operator()(MesafeVeDugum &a, MesafeVeDugum &b) const {
                return a.mesafe > b.mesafe;
            }
        };
    };

    struct dugumDurumu {
        bool ziyaretEdildi;
        bool kesfediliyor;
        dugumDurumu(bool ziyaretEdildi=false, bool kesfediliyor=false)
            : ziyaretEdildi(ziyaretEdildi), kesfediliyor(kesfediliyor) {}
    };

    bool keydenBaslaDFSCycleVarMi(int key, HashTable<int, dugumDurumu> &dugumDurumlari) const;

    typedef list<Edge>::const_iterator edgeListItr;

};
// End of header file
