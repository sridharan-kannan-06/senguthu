#pragma once

enum class cell_type {
    empty,
    forward_slash,
    back_slash  
};

struct game_move {
    int row;
    int col;
    cell_type type;
};