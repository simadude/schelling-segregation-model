#include "ui.h"
#include <cstdlib>

Fl_Double_Window *window_grid;
Fl_Double_Window *window_menu;
Fl_Value_Input *input_size;
Fl_Value_Input *red_count_input;
Fl_Value_Input *blue_count_input;
Fl_Value_Input *tolerance_percentage_input;
Fl_Button *button_start;
Fl_Button *button_reset;
Fl_Button *button_exit;

static void exit_cb(Fl_Widget*, void*) {
    std::exit(0);
}

Fl_Double_Window* launch_menu() {
    // Grid window
    window_grid = new Fl_Double_Window(343, 266, "Grid");
    window_grid->end();

    // Menu window
    window_menu = new Fl_Double_Window(256, 256, "Menu");
    window_menu->size_range(256, 256, 256, 256);

    // Grid label
    new Fl_Box(5, 5, 100, 30, "Grid");

    input_size = new Fl_Value_Input(45, 38, 50, 27, "Size:");
    input_size->minimum(2);
    input_size->maximum(1024);
    input_size->step(1);
    input_size->value(16);

    // Agents label
    new Fl_Box(5, 75, 100, 30, "Agents");

    red_count_input = new Fl_Value_Input(40, 108, 50, 27, "Red:");
    red_count_input->minimum(1);
    red_count_input->maximum(1024);
    red_count_input->step(1);
    red_count_input->value(1);

    blue_count_input = new Fl_Value_Input(165, 108, 50, 27, "Blue:");
    blue_count_input->minimum(1);
    blue_count_input->maximum(1024);
    blue_count_input->step(1);
    blue_count_input->value(1);

    tolerance_percentage_input = new Fl_Value_Input(85, 143, 50, 27, "Tolerance:");
    tolerance_percentage_input->minimum(1);
    tolerance_percentage_input->maximum(100);
    tolerance_percentage_input->step(1);
    tolerance_percentage_input->value(1);

    new Fl_Box(140, 140, 20, 30, "%");

    button_start = new Fl_Button(5, 190, 80, 30, "Start");
    button_start->deactivate();

    button_reset = new Fl_Button(135, 35, 80, 30, "Reset");

    button_exit = new Fl_Button(150, 190, 80, 30, "Exit");
    button_exit->callback(exit_cb);

    window_menu->end();

    return window_menu;
}
