#include "game_board.hpp"

game_board::game_board(level_grid level) {
    n_rows = level.rows;
    n_cols = level.cols;
    constraints = level.constraints;
    saturation_limit = level.saturation_limit;
    
    cells.resize(n_rows, std::vector<cell_type>(n_cols, cell_type::empty));
    board_graph = new graph(n_rows, n_cols, &cells);
}

game_board::~game_board() {
    delete board_graph;
}

int game_board::count_consecutive(int r, int c, int dr, int dc, cell_type type) {
    int count = 0;
    int curr_r = r + dr;
    int curr_c = c + dc;
    
    while(curr_r >= 0 && curr_r < n_rows && curr_c >= 0 && curr_c < n_cols) {
        if(cells[curr_r][curr_c] == type) {
            count++;
            curr_r += dr;
            curr_c += dc;
        } else {
            break;
        }
    }
    return count;
}

bool game_board::is_valid_move(int row, int col, cell_type type) {
    if(row < 0 || row >= n_rows || col < 0 || col >= n_cols) return false;
    if(cells[row][col] != cell_type::empty) return false;
    
    int r1, c1, r2, c2;
    if(type == cell_type::forward_slash) { 
        r1 = row + 1; c1 = col;
        r2 = row;     c2 = col + 1;
    } else { 
        r1 = row;     c1 = col;
        r2 = row + 1; c2 = col + 1;
    }
    
    // 1. Check Constraints
    int lim1 = constraints[r1][c1];
    if(lim1 != -1 && board_graph->get_degree(r1, c1) >= lim1) return false;
    
    int lim2 = constraints[r2][c2];
    if(lim2 != -1 && board_graph->get_degree(r2, c2) >= lim2) return false;
    
    // 2. Check Cycles
    if(board_graph->has_path(r1, c1, r2, c2)) return false;
    
    // 3. NEW: Check Straight Line Saturation
    int current_len = 1; // The move itself
    
    if(type == cell_type::forward_slash) {
        // Check Up-Right (-1, +1) and Bottom-Left (+1, -1)
        current_len += count_consecutive(row, col, -1, 1, type);
        current_len += count_consecutive(row, col, 1, -1, type);
    } else {
        // Check Up-Left (-1, -1) and Bottom-Right (+1, +1)
        current_len += count_consecutive(row, col, -1, -1, type);
        current_len += count_consecutive(row, col, 1, 1, type);
    }
    
    if(current_len > saturation_limit) return false;
    
    return true;
}

void game_board::apply_move(int row, int col, cell_type type) {
    cells[row][col] = type;
}

bool game_board::has_valid_moves() {
    for(int r = 0; r < n_rows; r++) {
        for(int c = 0; c < n_cols; c++) {
            if(cells[r][c] == cell_type::empty) {
                if(is_valid_move(r, c, cell_type::forward_slash)) return true;
                if(is_valid_move(r, c, cell_type::back_slash)) return true;
            }
        }
    }
    return false;
}

std::string game_board::get_invalid_reason(int row, int col, cell_type type) {
    if(cells[row][col] != cell_type::empty) return "Cell occupied!";
    
    int r1, c1, r2, c2;
    if(type == cell_type::forward_slash) {
        r1 = row + 1; c1 = col;
        r2 = row;     c2 = col + 1;
    } else {
        r1 = row;     c1 = col;
        r2 = row + 1; c2 = col + 1;
    }
    
    int lim1 = constraints[r1][c1];
    if(lim1 != -1 && board_graph->get_degree(r1, c1) >= lim1) return "Node Full!";
    
    int lim2 = constraints[r2][c2];
    if(lim2 != -1 && board_graph->get_degree(r2, c2) >= lim2) return "Node Full!";
    
    if(board_graph->has_path(r1, c1, r2, c2)) return "Loops not allowed!";
    
    // Saturation Reason
    int current_len = 1;
    if(type == cell_type::forward_slash) {
        current_len += count_consecutive(row, col, -1, 1, type);
        current_len += count_consecutive(row, col, 1, -1, type);
    } else {
        current_len += count_consecutive(row, col, -1, -1, type);
        current_len += count_consecutive(row, col, 1, 1, type);
    }
    
    if(current_len > saturation_limit) return "Line too long!";
    
    return "Invalid Move";
}