#include "greedy_cpu.hpp"
#include <algorithm>

greedy_cpu::greedy_cpu(game_board* b) {
    board = b;
}

std::vector<game_move> greedy_cpu::get_all_valid_moves() {
    std::vector<game_move> moves;
    for(int r = 0; r < board->get_rows(); r++) {
        for(int c = 0; c < board->get_cols(); c++) {
            if(board->is_valid_move(r, c, cell_type::forward_slash))
                moves.push_back({r, c, cell_type::forward_slash});
            if(board->is_valid_move(r, c, cell_type::back_slash))
                moves.push_back({r, c, cell_type::back_slash});
        }
    }
    return moves;
}

int greedy_cpu::calculate_move_score(game_move m) {
    int score = 0;
    int r1, c1, r2, c2;
    
    if(m.type == cell_type::forward_slash) {
        r1 = m.row + 1; c1 = m.col;
        r2 = m.row;     c2 = m.col + 1;
    } else {
        r1 = m.row;     c1 = m.col;
        r2 = m.row + 1; c2 = m.col + 1;
    }
    
    // Heuristic 1: If this move completes a required constraint, do it!
    int lim1 = board->get_constraint(r1, c1);
    int deg1 = board->get_degree(r1, c1);
    if(lim1 != -1 && deg1 + 1 == lim1) score += 100;
    
    int lim2 = board->get_constraint(r2, c2);
    int deg2 = board->get_degree(r2, c2);
    if(lim2 != -1 && deg2 + 1 == lim2) score += 100;
    
    // Heuristic 2: Prefer touching nodes that have constraints
    if(lim1 != -1) score += 20;
    if(lim2 != -1) score += 20;
    
    // Heuristic 3: Prefer busy areas
    score += (deg1 + deg2) * 5;
    
    return score;
}
