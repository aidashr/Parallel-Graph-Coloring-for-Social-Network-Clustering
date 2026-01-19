# Use Homebrew-installed LLVM for OpenMP
LLVM_DIR := /usr/local/opt/llvm

CXX := $(LLVM_DIR)/bin/clang++
CXXFLAGS := -std=c++17 -O2 -Wall -fopenmp
CPPFLAGS := -I$(LLVM_DIR)/include
LDFLAGS := -L$(LLVM_DIR)/lib

# Source files
SRC_COMMON := graph_utils.cpp metrics.cpp
SEQ_SRC := sequential_coloring.cpp
PAR_SRC := parallel_coloring_omp.cpp

SEQ_BIN := sequential_coloring
PAR_BIN := parallel_coloring

# Default target
all: $(SEQ_BIN) $(PAR_BIN)

$(SEQ_BIN): $(SEQ_SRC) $(SRC_COMMON)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	-xattr -d com.apple.quarantine $@

$(PAR_BIN): $(PAR_SRC) $(SRC_COMMON)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	-xattr -d com.apple.quarantine $@

# Run sequential coloring
run-seq: $(SEQ_BIN)
	./$(SEQ_BIN)

# Run parallel coloring with current default thread count
run-par: $(PAR_BIN)
	./$(PAR_BIN)

# ✅ Benchmark parallel coloring with multiple thread counts
run-par-benchmark: $(PAR_BIN)
	@for t in 1 2 4 8 10 12 16; do \
		echo "\n--- Running with $$t threads ---"; \
		OMP_NUM_THREADS=$$t ./$(PAR_BIN); \
	done

# Clean build artifacts
clean:
	rm -f $(SEQ_BIN) $(PAR_BIN) *.o *.out *.csv

.PHONY: all clean run-seq run-par run-par-benchmark
