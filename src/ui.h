#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Value_Input.H>

extern Fl_Double_Window *window_grid;
extern Fl_Double_Window *window_menu;
extern Fl_Value_Input *input_size;
extern Fl_Value_Input *red_count_input;
extern Fl_Value_Input *blue_count_input;
extern Fl_Value_Input *tolerance_percentage_input;
extern Fl_Value_Input *speed_input;
extern Fl_Button *button_start;
extern Fl_Button *button_reset;
extern Fl_Button *button_exit;

Fl_Double_Window* launch_menu();
