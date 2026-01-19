#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <omp.h>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

// Adjacency list representation
vector<vector<int>> graph;
unordered_map<int, int> nodeIdMap;
vector<int> reverseMap;

// Helper to map arbitrary node IDs to 0-based indices
int mapNodeId(int originalId) {
    static int currentId = 0;
    if (nodeIdMap.find(originalId) == nodeIdMap.end()) {
        nodeIdMap[originalId] = currentId++;
        reverseMap.push_back(originalId);
    }
    return nodeIdMap[originalId];
}

// Load Facebook SNAP dataset
void loadGraph(const string &filename) {
    ifstream infile(filename);
    string line;
    int maxId = 0;

    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue; // skip comments
        istringstream iss(line);
        int u_orig, v_orig;
        if (!(iss >> u_orig >> v_orig)) continue;

        int u = mapNodeId(u_orig);
        int v = mapNodeId(v_orig);

        int maxNode = max(u, v);
        if (maxNode >= graph.size())
            graph.resize(maxNode + 1);

        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected
    }

    infile.close();
    cout << "Loaded graph with " << graph.size() << " nodes.\n";
}

int main() {
    string filename = "/Users/anamaybrahme/VScode/Learning/Multi-Core-P-Assignments/Final_Pro_MCP/facebook_combined.txt";  // Adjust path if needed
    loadGraph(filename);

    int n = graph.size();
    vector<int> color(n, -1);
    bool changed = true;

    while (changed) {
        changed = false;

        #pragma omp parallel for schedule(dynamic)
        for (int u = 0; u < n; ++u) {
            if (color[u] != -1) continue;

            set<int> neighbor_colors;
            for (int v : graph[u]) {
                if (color[v] != -1)
                    neighbor_colors.insert(color[v]);
            }

            int c = 0;
            while (neighbor_colors.find(c) != neighbor_colors.end()) c++;

            color[u] = c;
        }

        // Conflict detection
        #pragma omp parallel for schedule(dynamic)
        for (int u = 0; u < n; ++u) {
            for (int v : graph[u]) {
                if (u < v && color[u] == color[v]) {
                    color[u] = -1;
                    #pragma omp atomic write
                    changed = true;
                }
            }
        }
    }

    // Output result
    ofstream outfile("user_colours.txt");
    outfile << "OriginalUserID\tColor\n";
    for (int i = 0; i < n; ++i) {
        outfile << reverseMap[i] << "\t" << color[i] << "\n";
    }
    outfile.close();

    cout << "Coloring complete. Results saved to user_colours.txt\n";
    return 0;
}
