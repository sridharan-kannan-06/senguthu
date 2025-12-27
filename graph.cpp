#include "graph.hpp"

graph::graph(int rows, int cols, std::vector<std::vector<cell_type>>* cell_ptr) {
    n_rows = rows;
    n_cols = cols;
    cells = cell_ptr;
}

int graph::get_degree(int r, int c) {
    int degree = 0;
    // Check all 4 cells around the node (r,c)
    if(r > 0 && c > 0 && (*cells)[r-1][c-1] == cell_type::back_slash) degree++;
    if(r > 0 && c < n_cols && (*cells)[r-1][c] == cell_type::forward_slash) degree++;
    if(r < n_rows && c > 0 && (*cells)[r][c-1] == cell_type::forward_slash) degree++;
    if(r < n_rows && c < n_cols && (*cells)[r][c] == cell_type::back_slash) degree++;
    return degree;
}

bool graph::has_path(int r1, int c1, int r2, int c2) {
    std::vector<std::vector<bool>> visited(n_rows + 1, std::vector<bool>(n_cols + 1, false));
    std::queue<std::pair<int, int>> q;
    q.push({r1, c1});
    visited[r1][c1] = true;
    
    while(!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        
        if(r == r2 && c == c2) return true;
        
        // 1. Top-Left (Backslash)
        if(r > 0 && c > 0 && (*cells)[r-1][c-1] == cell_type::back_slash && !visited[r-1][c-1]) {
            visited[r-1][c-1] = true; q.push({r-1, c-1});
        }
        // 2. Top-Right (Forwardslash)
        if(r > 0 && c < n_cols && (*cells)[r-1][c] == cell_type::forward_slash && !visited[r-1][c+1]) {
            visited[r-1][c+1] = true; q.push({r-1, c+1});
        }
        // 3. Bottom-Left (Forwardslash)
        if(r < n_rows && c > 0 && (*cells)[r][c-1] == cell_type::forward_slash && !visited[r+1][c-1]) {
            visited[r+1][c-1] = true; q.push({r+1, c-1});
        }
        // 4. Bottom-Right (Backslash)
        if(r < n_rows && c < n_cols && (*cells)[r][c] == cell_type::back_slash && !visited[r+1][c+1]) {
            visited[r+1][c+1] = true; q.push({r+1, c+1});
        }
    }
    return false;
}