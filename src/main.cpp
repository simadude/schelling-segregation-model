#include "ui.h"
#include "agentgrid.h"
#include <FL/fl_draw.H>
#include <cstdio>

static bool animating = false;
static int grid_size = 16;
static const int CELL_PX = 16;

class GridWidget : public Fl_Box {
public:
    GridWidget(int X, int Y, int W, int H) : Fl_Box(X, Y, W, H) {
        box(FL_FLAT_BOX);
        color(FL_WHITE);
    }
    void draw() override {
        fl_rectf(x(), y(), w(), h(), FL_WHITE);
        if (matrix.empty()) return;
        bool smooth = check_smooth->value();
        for (auto& a : agents) {
            Fl_Color c = (a.type == CELL_TYPE::RED) ? FL_RED : FL_BLUE;
            float px = smooth ? a.draw_y : a.y;
            float py = smooth ? a.draw_x : a.x;
            fl_rectf(x() + static_cast<int>(px * CELL_PX),
                     y() + static_cast<int>(py * CELL_PX),
                     CELL_PX, CELL_PX, c);
            fl_rect(x() + static_cast<int>(px * CELL_PX),
                    y() + static_cast<int>(py * CELL_PX),
                    CELL_PX, CELL_PX, FL_BLACK);
        }
    }
};

static GridWidget* grid_widget = nullptr;

static void update_opt_label() {
    int size_val = static_cast<int>(input_size->value());
    int total = size_val * size_val;
    int opt_r = total * 4 / 10;
    int opt_b = total * 4 / 10;
    int opt_e = total - opt_r - opt_b;
    char buf[64];
    std::snprintf(buf, sizeof(buf), "opt: %d,%d,%d", opt_r, opt_b, opt_e);
    label_opt->copy_label(buf);
}

static void update_seg_label() {
    if (agents.empty()) return;
    char buf[32];
    std::snprintf(buf, sizeof(buf), "Seg: %.3f", get_segregation_coef());
    label_seg->copy_label(buf);
}

static double get_speed() {
    return speed_input->value() / 1000.0;
}

static void render_timer_cb(void*) {
    if (!animating) return;
    step_animations(0.3f);
    grid_widget->redraw();
    if (animations_active()) {
        Fl::repeat_timeout(0.016, render_timer_cb);
    }
}

static void sim_timer_cb(void*) {
    if (!animating) return;
    bool smooth = check_smooth->value();
    if (smooth && animations_active()) {
        Fl::repeat_timeout(get_speed(), sim_timer_cb);
        return;
    }
    update_agents();
    update_seg_label();
    if (smooth) {
        Fl::add_timeout(0.016, render_timer_cb);
    } else {
        grid_widget->redraw();
    }
    Fl::repeat_timeout(get_speed(), sim_timer_cb);
}

static void start_cb(Fl_Widget*, void*) {
    if (animating) {
        animating = false;
        button_start->label("Start");
        button_start->redraw();
        return;
    }
    animating = true;
    button_start->label("Stop");
    button_start->redraw();
    Fl::add_timeout(get_speed(), sim_timer_cb);
    if (check_smooth->value()) {
        Fl::add_timeout(0.016, render_timer_cb);
    }
}

static void reset_cb(Fl_Widget*, void*) {
    animating = false;
    button_start->label("Start");

    grid_size = static_cast<int>(input_size->value());
    red_count = static_cast<int>(red_count_input->value());
    blue_count = static_cast<int>(blue_count_input->value());
    tolerance = tolerance_percentage_input->value() / 100.0;

    segregation_history.clear();
    resize_matrix(grid_size);

    update_opt_label();
    update_seg_label();

    int win_w = grid_size * CELL_PX;
    int win_h = grid_size * CELL_PX;
    window_grid->resize(window_grid->x(), window_grid->y(), win_w, win_h);

    if (grid_widget) {
        grid_widget->resize(0, 0, win_w, win_h);
    }

    button_start->activate();
    window_grid->redraw();
}

int main(int argc, char **argv) {
    Fl_Double_Window* w = launch_menu();

    grid_size = static_cast<int>(input_size->value());
    int win_w = grid_size * CELL_PX;
    int win_h = grid_size * CELL_PX;
    window_grid->resize(window_grid->x(), window_grid->y(), win_w, win_h);

    grid_widget = new GridWidget(0, 0, win_w, win_h);
    window_grid->add(grid_widget);
    window_grid->end();

    button_reset->callback(reset_cb);
    button_start->callback(start_cb);
    button_save_seg->callback([](Fl_Widget*, void*) {
        save_segregation_history();
    });

    update_opt_label();

    w->show();
    window_grid->show();
    return Fl::run();
}
