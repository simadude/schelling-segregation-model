#pragma once
#include <vector>

enum class CELL_TYPE {
    EMPTY,
    RED,
    BLUE,
};

struct agent {
    int x, y;
    CELL_TYPE type;
    float draw_x, draw_y;
};

extern double tolerance;
extern int red_count;
extern int blue_count;
extern std::vector<std::vector<CELL_TYPE>> matrix;
extern std::vector<agent> agents;

void resize_matrix(int size);
void update_agents();
double get_segregation_coef();
bool animations_active();
void step_animations(float lerp);

