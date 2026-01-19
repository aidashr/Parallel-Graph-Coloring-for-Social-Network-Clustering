# Parallel Graph Colouring for Social Network Clustering

This project applies a parallel graph colouring algorithm using OpenMP to social network graphs (e.g., Facebook SNAP dataset) to assign non-conflicting user groups (colors).

## 💡 Features
- Loads Facebook Social Circles graph (`facebook_combined.txt`)
- Uses parallel greedy graph colouring with OpenMP
- Outputs user → color assignments
- Demonstrates conflict-free clustering using parallelism

## 🚀 How to Run

```bash
g++ -fopenmp graph_coloring.cpp -o graphcolor
./graphcolor
