#include "game_window.hpp"
#include "level_data.hpp"

game_window::game_window() : 
    main_box(Gtk::ORIENTATION_VERTICAL),
    control_box(Gtk::ORIENTATION_HORIZONTAL),
    btn_5x5("New 5x5 Game"),
    btn_8x8("New 8x8 Game"),
    turn_label("") {
    
    board = nullptr;
    cpu = nullptr;
    player_turn = true;
    
    set_title("Slant Puzzle - Senguthu");
    set_border_width(15);
    set_default_size(600, 700);
    
    control_box.set_spacing(10);
    control_box.pack_start(btn_5x5, Gtk::PACK_SHRINK);
    control_box.pack_start(btn_8x8, Gtk::PACK_SHRINK);
    control_box.pack_start(turn_label, Gtk::PACK_EXPAND_WIDGET);
    
    btn_5x5.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &game_window::on_new_game), 5));
    btn_8x8.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &game_window::on_new_game), 8));
    
    turn_label.set_markup("<big><b>Start a New Game</b></big>");
    turn_label.set_halign(Gtk::ALIGN_CENTER);
    
    info_frame.set_label("Game Status");
    info_frame.add(info_label);
    info_label.set_margin_top(10);
    info_label.set_margin_bottom(10);
    info_label.set_line_wrap(true);
    info_label.set_text("Welcome! Click a 'New Game' button to begin.");
    
    main_box.set_spacing(15);
    main_box.pack_start(control_box, Gtk::PACK_SHRINK);
    main_box.pack_start(game_grid, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(info_frame, Gtk::PACK_SHRINK);
    
    add(main_box);
    show_all_children();
}

game_window::~game_window() {
    clear_game_grid();
    delete cpu;
    delete board;
}

void game_window::clear_game_grid() {
    for(auto& row : cell_buttons) {
        for(auto& btn : row) delete btn;
    }
    cell_buttons.clear();
    

    for(auto& row : constraint_labels) {
        for(auto& lbl : row) delete lbl;
    }
    constraint_labels.clear();
    
    auto children = game_grid.get_children();
    for(auto child : children) game_grid.remove(*child);
}

void game_window::create_game_grid() {
    clear_game_grid();
    
    int n_rows = board->get_rows();
    int n_cols = board->get_cols();
    
    game_grid.set_row_spacing(0);
    game_grid.set_column_spacing(0);
    game_grid.set_halign(Gtk::ALIGN_CENTER);
    game_grid.set_valign(Gtk::ALIGN_CENTER);
    
  
    constraint_labels.resize(n_rows + 1);
    for(int r = 0; r <= n_rows; r++) {
        constraint_labels[r].resize(n_cols + 1);
        for(int c = 0; c <= n_cols; c++) {
            Gtk::Label* lbl = new Gtk::Label();
            lbl->set_size_request(30, 30);
            
            int cons = board->get_constraint(r, c);
            if(cons != -1) {
                std::string mk = "<span color='#d32f2f' size='x-large'><b>" + 
                                std::to_string(cons) + "</b></span>";
                lbl->set_markup(mk);
            } else {
                lbl->set_text("+");
            }
            
            constraint_labels[r][c] = lbl;
            game_grid.attach(*lbl, c * 2, r * 2, 1, 1);
        }
    }
    
    cell_buttons.resize(n_rows);
    for(int r = 0; r < n_rows; r++) {
        cell_buttons[r].resize(n_cols);
        for(int c = 0; c < n_cols; c++) {
            Gtk::Button* btn = new Gtk::Button();
            btn->set_size_request(60, 60);
            
            btn->signal_button_press_event().connect(
                sigc::bind(sigc::mem_fun(*this, &game_window::on_cell_button_press), r, c),
                false); 
            
            cell_buttons[r][c] = btn;
            game_grid.attach(*btn, c * 2 + 1, r * 2 + 1, 1, 1);
        }
    }
    
    game_grid.show_all_children();
    

    update_display();
}

bool game_window::on_cell_button_press(GdkEventButton* event, int row, int col) {
    if(!player_turn || board == nullptr) return true;
    
    cell_type type;
    if(event->button == 1) type = cell_type::forward_slash; 
    else if(event->button == 3) type = cell_type::back_slash;
    else return false;
    
    if(board->is_valid_move(row, col, type)) {
        board->apply_move(row, col, type);
        update_display();
        
        player_turn = false;
        turn_label.set_markup("<big><b>CPU Thinking...</b></big>");
        info_label.set_text("CPU is calculating...");
        
        Glib::signal_timeout().connect_once(
            sigc::mem_fun(*this, &game_window::cpu_move), 600);
    } else {
        std::string rsn = board->get_invalid_reason(row, col, type);
        info_label.set_text("INVALID: " + rsn);
    }
    
    return true; 
}

void game_window::update_display() {
    for(int r = 0; r < board->get_rows(); r++) {
        for(int c = 0; c < board->get_cols(); c++) {
            cell_type cell = board->get_cell(r, c);
            auto btn = cell_buttons[r][c];
            
            if(cell != cell_type::empty) {
                std::string slash_char = (cell == cell_type::forward_slash) ? "/" : "\\";
                btn->set_label(slash_char);
                
                Gtk::Label* lbl = dynamic_cast<Gtk::Label*>(btn->get_child());
                if(lbl) {
                    std::string markup = "<span size='xx-large' weight='bold'>" + slash_char + "</span>";
                    lbl->set_markup(markup);
                }
                btn->set_sensitive(false);
            }
        }
    }


    for(int r = 0; r <= board->get_rows(); r++) {
        for(int c = 0; c <= board->get_cols(); c++) {
            int constraint = board->get_constraint(r, c);
            
            // Only update if there is a number constraint (-1 means no constraint)
            if(constraint != -1) {
                int current_degree = board->get_degree(r, c);
                
                std::string color;
                if(current_degree == constraint) {
                    color = "#2e7d32"; // Green (Success)
                } else if(current_degree > constraint) {
                    color = "#c62828"; // Red (Failed/Overfilled - strictly shouldn't happen with validation)
                } else {
                    color = "#d32f2f"; // Red (Incomplete)
                }

                std::string mk = "<span color='" + color + "' size='x-large'><b>" + 
                                std::to_string(constraint) + "</b></span>";
                constraint_labels[r][c]->set_markup(mk);
            }
        }
    }
}

void game_window::cpu_move() {
    check_game_over();
    if(!board->has_valid_moves()) return;
    
    game_move m = cpu->decide_move();
    board->apply_move(m.row, m.col, m.type);
    
    std::string move_char = (m.type == cell_type::forward_slash) ? "/" : "\\";
    info_label.set_text("CPU placed '" + move_char + "' at (" + 
        std::to_string(m.row) + "," + std::to_string(m.col) + "). Your turn!");
    
    update_display();
    player_turn = true;
    turn_label.set_markup("<big><b>👤 Your Turn</b></big>");
    check_game_over();
}

void game_window::check_game_over() {
    if(!board->has_valid_moves()) {
        if(player_turn) {
            turn_label.set_markup("<big><b>💀 GAME OVER</b></big>");
            info_label.set_text("CPU Wins! No valid moves left for you.");
        } else {
            turn_label.set_markup("<big><b>🏆 YOU WIN!</b></big>");
            info_label.set_text("Victory! CPU has no valid moves left.");
        }
    }
}

void game_window::on_new_game(int size) {
    if(board) { delete board; delete cpu; }
    
    level_grid level = level_data::get_random_level(size);
    board = new game_board(level);
    cpu = new greedy_cpu(board);
    
    player_turn = true;
    create_game_grid();
    
    turn_label.set_markup("<big><b>👤 Your Turn</b></big>");
    info_label.set_text("Left Click: Forward Slash (/)\nRight Click: Back Slash (\\)");
}