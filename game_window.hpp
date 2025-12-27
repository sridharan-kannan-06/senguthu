#pragma once
#include <gtkmm.h>
#include "game_board.hpp"
#include "greedy_cpu.hpp"

class game_window : public Gtk::Window {
private:
    game_board* board;
    greedy_cpu* cpu;
    
    Gtk::Box main_box;
    Gtk::Box control_box;
    Gtk::Grid game_grid;
    Gtk::Frame info_frame;
    Gtk::Label info_label;
    Gtk::Button btn_5x5;
    Gtk::Button btn_8x8;
    Gtk::Label turn_label;
    
    // Stores pointers to the cell buttons
    std::vector<std::vector<Gtk::Button*>> cell_buttons;
    
    // Stores pointers to the constraint labels (the numbers)
    std::vector<std::vector<Gtk::Label*>> constraint_labels;
    
    bool player_turn;
    
    void create_game_grid();
    void clear_game_grid();
    void update_display();
    
    bool on_cell_button_press(GdkEventButton* event, int row, int col);
    
    void on_new_game(int size);
    void cpu_move();
    void check_game_over();
    
public:
    game_window();
    ~game_window();
};