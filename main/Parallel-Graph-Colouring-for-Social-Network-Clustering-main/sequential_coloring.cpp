#include "graph_utils.hpp"
#include "metrics.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<int> sequentialGreedyColoring(const Graph &graph)
{
    int n = graph.numVertices();
    std::vector<int> color(n, -1);         // -1 means uncolored
    std::vector<bool> available(n, false); // Marks unavailable colors

    const auto &adjList = graph.getAdjList();

    for (int u = 0; u < n; ++u)
    {
        for (int v : adjList[u])
        {
            if (color[v] != -1)
                available[color[v]] = true;
        }

        int chosen_color = 0;
        while (chosen_color < n && available[chosen_color])
            ++chosen_color;

        color[u] = chosen_color;

        for (int v : adjList[u])
        {
            if (color[v] != -1)
                available[color[v]] = false;
        }
    }

    return color;
}

int main()
{
    try
    {
        std::string filename = "facebook_combined.txt";
        Graph graph;
        graph.loadFromFile(filename);
        graph.printStats();

        Timer timer;
        timer.start();

        std::vector<int> colors = sequentialGreedyColoring(graph);

        timer.stop();
        double cpu_util = getCPUUtilization();

        int max_color = *std::max_element(colors.begin(), colors.end()) + 1;
        double mean_color = computeMean(colors);
        double var_color = computeVariance(colors);
        double exec_time = timer.elapsedSeconds();

        // Console output
        std::cout << "\n--- Sequential Greedy Coloring ---\n";
        std::cout << "Total Colors Used   : " << max_color << "\n";
        std::cout << "Execution Time (s)  : " << exec_time << "\n";
        std::cout << "CPU Time (s)        : " << cpu_util << "\n";
        std::cout << "Mean Color Value    : " << mean_color << "\n";
        std::cout << "Color Variance      : " << var_color << "\n";

        // Write to CSV
        std::ofstream outfile("results_sequential.csv", std::ios::app); // append mode
        if (outfile.tellp() == 0) {
            // Write header if file is new
            outfile << "Algorithm,Threads,TotalColors,ExecutionTime,CPUTime,MeanColor,ColorVariance\n";
        }
        outfile << "Sequential,1," << max_color << "," << exec_time << "," << cpu_util << ","
                << mean_color << "," << var_color << "\n";
        outfile.close();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
