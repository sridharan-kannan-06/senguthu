#pragma once
#include "cell_types.hpp"
#include "level_data.hpp"
#include "graph.hpp"
#include <vector>
#include <string>

class game_board {
private:
    int n_rows;
    int n_cols;
    int saturation_limit; 
    std::vector<std::vector<cell_type>> cells;
    std::vector<std::vector<int>> constraints;
    graph* board_graph;
    
    int count_consecutive(int r, int c, int dr, int dc, cell_type type);

public:
    game_board(level_grid level);
    ~game_board();
    
    bool is_valid_move(int row, int col, cell_type type);
    void apply_move(int row, int col, cell_type type);
    bool has_valid_moves();
    std::string get_invalid_reason(int row, int col, cell_type type);
    
    int get_rows() { return n_rows; }
    int get_cols() { return n_cols; }
    int get_constraint(int r, int c) { return constraints[r][c]; }
    int get_degree(int r, int c) { return board_graph->get_degree(r, c); }
    cell_type get_cell(int r, int c) { return cells[r][c]; }
};