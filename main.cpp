#include <gtkmm/application.h>
#include "game_window.hpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.slant.senguthu");
    game_window window;
    return app->run(window);
}

// COMPILE COMMAND (Run this in MSYS2/Terminal):
// g++ -std=c++17 main.cpp level_data.cpp graph.cpp game_board.cpp greedy_cpu.cpp game_window.cpp -o slant.exe `pkg-config gtkmm-3.0 --cflags --libs`
// ./slant.exe
