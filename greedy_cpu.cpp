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