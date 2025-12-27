#pragma once
#include "cell_types.hpp"
#include <vector>
#include <queue>

class graph {
private:
    int n_rows;
    int n_cols;
    std::vector<std::vector<cell_type>>* cells;
    
public:
    graph(int rows, int cols, std::vector<std::vector<cell_type>>* cell_ptr);
    
    // Returns how many lines are currently connected to node (r,c)
    int get_degree(int r, int c);
    
    // Checks if a path exists between two nodes (used for cycle detection)
    bool has_path(int r1, int c1, int r2, int c2);
};