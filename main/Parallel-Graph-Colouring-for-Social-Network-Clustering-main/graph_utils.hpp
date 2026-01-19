#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Graph
{
public:
    Graph() = default;

    // Load graph from edge list file
    void loadFromFile(const std::string &filename);

    // Accessors
    int numVertices() const;
    int numEdges() const;
    const std::vector<int> &getNeighbors(int node) const;
    const std::vector<std::vector<int>> &getAdjList() const;

    // Print basic stats
    void printStats() const;

private:
    std::vector<std::vector<int>> adjList;
    int vertices = 0;
    int edges = 0;
    std::unordered_map<int, int> idMapping; // Original ID -> compressed ID
    std::vector<int> reverseMapping;        // Compressed ID -> Original ID

    void compressIds(const std::vector<std::pair<int, int>> &edges_raw);
};

#endif // GRAPH_UTILS_HPP
