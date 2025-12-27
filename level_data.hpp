#pragma once
#include <vector>

struct level_grid {
    int rows;
    int cols;
    // -1 means no constraint, 0-4 is the required degree
    std::vector<std::vector<int>> constraints; 
    
    // The maximum allowed length for a continuous line
    int saturation_limit; 
};

class level_data {
public:
    static level_grid get_random_level(int size);
    
private:
    static const std::vector<level_grid> levels_5x5;
    static const std::vector<level_grid> levels_8x8;
};