#include <iostream>
#include "ui.h"

int main(int argc, char **argv)
{
    Fl_Double_Window* w = launch_menu();
    w->show();
    return Fl::run();
}
