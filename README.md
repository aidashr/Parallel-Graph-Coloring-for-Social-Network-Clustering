# Parallel Graph Coloring for Social Network Clustering (OpenMP)

This repository implements **sequential** and **parallel (OpenMP)** greedy graph coloring for large social-network graphs. Graph coloring assigns a “color” (group/cluster label) to each node such that **no two adjacent nodes share the same color**. The parallel implementation uses a **two-phase** strategy (tentative coloring + conflict detection) to scale across CPU cores while preserving correctness.

The project includes:
- C++17 implementations of **sequential greedy coloring** and **parallel greedy coloring (OpenMP)**
- Performance logging (execution time, CPU time, total colors, mean/variance of assigned colors)
- Benchmark outputs (`results_*.csv`) and analysis plots/notebook

---

## Problem Summary

Given an undirected graph \(G=(V,E)\) representing a social network:
- Each node is a user
- Each edge represents a friendship/connection  
We assign colors to nodes so that adjacent nodes do not share a color. Colors can be interpreted as **conflict-free user groups**, useful for tasks like controlled experiments, segmentation, or reducing direct neighbor overlap.

---

## Approach

### 1) Sequential Greedy Coloring
Iterates vertices and assigns the **lowest available color** not used by its neighbors (classic greedy heuristic).

### 2) Parallel Greedy Coloring (OpenMP)
Implements an iterative two-phase approach:
1. **Tentative parallel coloring:** each thread colors currently-uncolored vertices using the greedy rule.
2. **Conflict detection:** edges are scanned; if a conflict is found (same color on adjacent vertices), one endpoint is reset to “uncolored”.
3. Repeat until no conflicts remain.

Threads are controlled via `OMP_NUM_THREADS`.

---

## Dataset

The repo includes `facebook_combined.txt` (edge list format), commonly associated with the SNAP Facebook social circles dataset structure:
- Each line: `u v` indicating an undirected edge between users `u` and `v`
- Node IDs are **compressed internally** into contiguous indices for efficient storage

> Note: The loader removes self-loops and de-duplicates neighbor lists.

---

## Build Requirements

- **C++17** compatible compiler
- **OpenMP** support (for parallel executable)
- macOS: OpenMP typically requires LLVM/clang with OpenMP runtime (see Makefile notes)
- Linux: `g++` usually supports OpenMP via `-fopenmp`

---

## Build & Run

### Using the provided `Makefile` (macOS LLVM setup)
The `Makefile` is configured for a Homebrew LLVM OpenMP toolchain:

```bash
make
make run-seq
make run-par
