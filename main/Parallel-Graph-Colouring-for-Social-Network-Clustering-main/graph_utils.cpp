#include "graph_utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

void Graph::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<std::pair<int, int>> edgeListRaw;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        int u, v;
        if (iss >> u >> v)
        {
            edgeListRaw.emplace_back(u, v);
        }
    }
    file.close();

    compressIds(edgeListRaw);

    adjList.resize(vertices);
    for (const auto &[u_orig, v_orig] : edgeListRaw)
    {
        int u = idMapping[u_orig];
        int v = idMapping[v_orig];
        if (u != v)
        { // avoid self-loops
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            ++edges;
        }
    }

    // Remove duplicate neighbors
    for (auto &neighbors : adjList)
    {
        std::sort(neighbors.begin(), neighbors.end());
        neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }
}

void Graph::compressIds(const std::vector<std::pair<int, int>> &edges_raw)
{
    std::unordered_set<int> unique_ids;
    for (const auto &[u, v] : edges_raw)
    {
        unique_ids.insert(u);
        unique_ids.insert(v);
    }

    reverseMapping.resize(unique_ids.size());
    int idx = 0;
    for (int id : unique_ids)
    {
        idMapping[id] = idx;
        reverseMapping[idx] = id;
        ++idx;
    }

    vertices = static_cast<int>(unique_ids.size());
}

int Graph::numVertices() const
{
    return vertices;
}

int Graph::numEdges() const
{
    return edges;
}

const std::vector<int> &Graph::getNeighbors(int node) const
{
    return adjList[node];
}

const std::vector<std::vector<int>> &Graph::getAdjList() const
{
    return adjList;
}

void Graph::printStats() const
{
    std::cout << "Graph loaded:\n";
    std::cout << "  Nodes: " << vertices << "\n";
    std::cout << "  Edges: " << edges << "\n";
}
