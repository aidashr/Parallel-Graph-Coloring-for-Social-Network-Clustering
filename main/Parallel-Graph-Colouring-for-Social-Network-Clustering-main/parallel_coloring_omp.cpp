#include "graph_utils.hpp"
#include "metrics.hpp"

#include <omp.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib> // for std::getenv

// Read thread count from env
int getNumThreadsFromEnv()
{
    const char* env = std::getenv("OMP_NUM_THREADS");
    if (env)
        return std::stoi(env);
    return omp_get_max_threads(); // fallback
}

std::vector<int> parallelGreedyColoring(const Graph &graph, int num_threads)
{
    int n = graph.numVertices();
    const auto &adjList = graph.getAdjList();

    std::vector<int> color(n, -1); // -1 = uncolored

    bool changed;

    do
    {
        changed = false;

        // Phase 1: Tentative parallel coloring
#pragma omp parallel for num_threads(num_threads) schedule(dynamic)
        for (int u = 0; u < n; ++u)
        {
            if (color[u] != -1)
                continue;

            std::vector<bool> usedColors(n, false);
            for (int v : adjList[u])
            {
                if (color[v] != -1)
                {
                    usedColors[color[v]] = true;
                }
            }

            for (int c = 0; c < n; ++c)
            {
                if (!usedColors[c])
                {
                    color[u] = c;
                    break;
                }
            }
        }

        // Phase 2: Conflict detection and reset
#pragma omp parallel for num_threads(num_threads) schedule(dynamic)
        for (int u = 0; u < n; ++u)
        {
            for (int v : adjList[u])
            {
                if (u < v && color[u] == color[v])
                {
#pragma omp critical
                {
                    color[v] = -1;
                    changed = true;
                }
                }
            }
        };

    } while (changed);

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

        int num_threads = getNumThreadsFromEnv();
        std::cout << "Using OpenMP with " << num_threads << " threads.\n";

        Timer timer;
        timer.start();

        std::vector<int> colors = parallelGreedyColoring(graph, num_threads);

        timer.stop();
        double cpu_util = getCPUUtilization();

        int max_color = *std::max_element(colors.begin(), colors.end()) + 1;
        double mean_color = computeMean(colors);
        double var_color = computeVariance(colors);
        double exec_time = timer.elapsedSeconds();

        // Console output
        std::cout << "\n--- Parallel Greedy Coloring (OpenMP) ---\n";
        std::cout << "Total Colors Used   : " << max_color << "\n";
        std::cout << "Execution Time (s)  : " << exec_time << "\n";
        std::cout << "CPU Time (s)        : " << cpu_util << "\n";
        std::cout << "Mean Color Value    : " << mean_color << "\n";
        std::cout << "Color Variance      : " << var_color << "\n";

        // Write to CSV
        std::ofstream outfile("results_parallel.csv", std::ios::app); // append mode
        bool write_header = outfile.tellp() == 0;
        if (write_header)
        {
            outfile << "Algorithm,Threads,TotalColors,ExecutionTime,CPUTime,MeanColor,ColorVariance\n";
        }

        outfile << "Parallel," << num_threads << "," << max_color << ","
                << exec_time << "," << cpu_util << ","
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
