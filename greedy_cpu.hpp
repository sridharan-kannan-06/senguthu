#pragma once
#include "game_board.hpp"
#include <vector>

struct ranked_move {
    game_move move;
    int score;
};

class greedy_cpu {
private:
    game_board* board;
    
    std::vector<game_move> get_all_valid_moves();
    int calculate_move_score(game_move m);
    std::vector<ranked_move> rank_and_sort_moves();
    
public:
    greedy_cpu(game_board* b);
    game_move decide_move();
};