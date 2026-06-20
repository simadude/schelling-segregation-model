#include "agentgrid.h"
#include <iostream>
#include <cmath>

static int passed = 0;
static int failed = 0;

void check(bool condition, const char* name) {
    if (condition) {
        std::cout << "  " << name << "... PASS" << std::endl;
        passed++;
    } else {
        std::cout << "  " << name << "... FAIL" << std::endl;
        failed++;
    }
}

// --- resize_matrix ---

void test_resize_matrix_dimensions() {
    red_count = 5;
    blue_count = 5;
    resize_matrix(10);
    check(matrix.size() == 10 && matrix[0].size() == 10, "resize_matrix creates correct dimensions");
}

void test_resize_matrix_agent_count() {
    red_count = 8;
    blue_count = 12;
    resize_matrix(10);
    check(agents.size() == 20, "resize_matrix places correct number of agents");
}

void test_resize_matrix_agent_types() {
    red_count = 3;
    blue_count = 7;
    resize_matrix(10);
    int r = 0, b = 0;
    for (auto& a : agents) {
        if (a.type == CELL_TYPE::RED) r++;
        else b++;
    }
    check(r == 3 && b == 7, "resize_matrix assigns correct types");
}

void test_resize_matrix_agents_in_matrix() {
    red_count = 5;
    blue_count = 5;
    resize_matrix(8);
    bool ok = true;
    for (auto& a : agents) {
        if (matrix[a.x][a.y] != a.type) ok = false;
    }
    check(ok, "resize_matrix agents match matrix cells");
}

void test_resize_matrix_overflow() {
    red_count = 50;
    blue_count = 50;
    resize_matrix(5); // 25 cells, 100 requested
    check(agents.size() == 25, "resize_matrix caps agents to grid size");
}

// --- is_satisfied ---

void test_is_satisfied_empty_cell() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(5);
    check(is_satisfied(0, 0), "is_satisfied true for empty cell");
}

void test_is_satisfied_no_neighbors() {
    red_count = 1;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.5;
    bool ok = true;
    for (auto& a : agents) {
        if (a.type == CELL_TYPE::RED) {
            if (!is_satisfied(a.x, a.y)) ok = false;
        }
    }
    check(ok, "is_satisfied true when no neighbors");
}

void test_is_satisfied_all_same_neighbors() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(3);
    tolerance = 0.5;

    matrix[0][0] = CELL_TYPE::RED;
    matrix[0][1] = CELL_TYPE::RED;
    matrix[0][2] = CELL_TYPE::RED;
    matrix[1][0] = CELL_TYPE::RED;
    matrix[1][1] = CELL_TYPE::RED;

    check(is_satisfied(0, 0), "is_satisfied true when all neighbors same");
}

void test_is_satisfied_mixed_neighbors_high_tolerance() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(2);
    tolerance = 0.8;

    matrix[0][0] = CELL_TYPE::RED;
    matrix[0][1] = CELL_TYPE::BLUE;
    matrix[1][0] = CELL_TYPE::BLUE;

    check(!is_satisfied(0, 0), "is_satisfied false with mixed neighbors and high tolerance");
}

void test_is_satisfied_mixed_neighbors_low_tolerance() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(2);
    tolerance = 0.1;

    matrix[0][0] = CELL_TYPE::RED;
    matrix[0][1] = CELL_TYPE::BLUE;
    matrix[1][0] = CELL_TYPE::BLUE;

    // 0/2 same = 0% < 10% tolerance => not satisfied
    check(!is_satisfied(0, 0),
          "is_satisfied false with 0% same even low tolerance");
}

void test_is_satisfied_half_same() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(3);
    tolerance = 0.5;

    matrix[0][0] = CELL_TYPE::RED;
    matrix[0][1] = CELL_TYPE::BLUE;
    matrix[0][2] = CELL_TYPE::RED;
    matrix[1][1] = CELL_TYPE::RED;

    // (1,1)=R: neighbors R,B,R => 2/3=66% >= 50% => satisfied
    // (0,1)=B: neighbors R,R,R => 0/3=0% < 50% => not satisfied
    check(is_satisfied(1, 1) && !is_satisfied(0, 1),
          "is_satisfied with ~50% same neighbors");
}

// --- get_segregation_coef ---

void test_seg_coef_empty() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(5);
    check(std::abs(get_segregation_coef()) < 0.001, "get_segregation_coef 0 for empty grid");
}

void test_seg_coef_fully_segregated() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.3;

    // R R . . .
    // R R . . .
    // . . . . .
    // . . B B .
    // . . B B .
    matrix[0][0] = CELL_TYPE::RED; matrix[0][1] = CELL_TYPE::RED;
    matrix[1][0] = CELL_TYPE::RED; matrix[1][1] = CELL_TYPE::RED;
    matrix[3][2] = CELL_TYPE::BLUE; matrix[3][3] = CELL_TYPE::BLUE;
    matrix[4][2] = CELL_TYPE::BLUE; matrix[4][3] = CELL_TYPE::BLUE;

    check(std::abs(get_segregation_coef() - 1.0) < 0.001, "get_segregation_coef ~1.0 for segregated grid");
}

void test_seg_coef_mixed() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(2);
    tolerance = 0.3;

    // R B
    // B R  => each agent has 1/3 same neighbors
    matrix[0][0] = CELL_TYPE::RED;
    matrix[0][1] = CELL_TYPE::BLUE;
    matrix[1][0] = CELL_TYPE::BLUE;
    matrix[1][1] = CELL_TYPE::RED;

    check(std::abs(get_segregation_coef() - 1.0/3.0) < 0.001, "get_segregation_coef ~0.33 for checkerboard");
}

// --- step_animations ---

void test_step_animations_moves() {
    red_count = 1;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.3;

    agents[0].x = 3;
    agents[0].y = 4;
    agents[0].draw_x = 0.0f;
    agents[0].draw_y = 0.0f;

    step_animations(0.5f);

    check(std::abs(agents[0].draw_x - 1.5f) < 0.01f &&
          std::abs(agents[0].draw_y - 2.0f) < 0.01f,
          "step_animations interpolates draw position");
}

void test_step_animations_snap() {
    red_count = 1;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.3;

    agents[0].x = 2;
    agents[0].y = 3;
    agents[0].draw_x = 2.005f;
    agents[0].draw_y = 2.995f;

    step_animations(0.5f);

    check(std::abs(agents[0].draw_x - 2.0f) < 0.001f &&
          std::abs(agents[0].draw_y - 3.0f) < 0.001f,
          "step_animations snaps when close");
}

// --- animations_active ---

void test_animations_active_false() {
    red_count = 1;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.3;

    agents[0].x = 2;
    agents[0].y = 3;
    agents[0].draw_x = 2.0f;
    agents[0].draw_y = 3.0f;

    check(!animations_active(), "animations_active false when all at target");
}

void test_animations_active_true() {
    red_count = 1;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.3;

    agents[0].x = 4;
    agents[0].y = 4;
    agents[0].draw_x = 0.0f;
    agents[0].draw_y = 0.0f;

    check(animations_active(), "animations_active true when agent moving");
}

// --- update_agents ---

void test_update_agents_records_history() {
    red_count = 5;
    blue_count = 5;
    resize_matrix(8);
    tolerance = 0.3;
    segregation_history.clear();

    update_agents();
    update_agents();

    check(segregation_history.size() == 2, "update_agents records segregation in history");
}

void test_update_agents_moves_unsatisfied() {
    red_count = 0;
    blue_count = 0;
    resize_matrix(5);
    tolerance = 0.99;

    matrix[2][2] = CELL_TYPE::RED;
    matrix[1][1] = CELL_TYPE::BLUE; matrix[1][2] = CELL_TYPE::BLUE; matrix[1][3] = CELL_TYPE::BLUE;
    matrix[2][1] = CELL_TYPE::BLUE; matrix[2][3] = CELL_TYPE::BLUE;
    matrix[3][1] = CELL_TYPE::BLUE; matrix[3][2] = CELL_TYPE::BLUE; matrix[3][3] = CELL_TYPE::BLUE;

    agents.clear();
    agents.push_back(agent{2, 2, CELL_TYPE::RED, 2.0f, 2.0f});
    int dx[] = {-1,-1,-1,0,0,1,1,1};
    int dy[] = {-1,0,1,-1,1,-1,0,1};
    for (int k = 0; k < 8; k++) {
        int nx = 2+dx[k], ny = 2+dy[k];
        agents.push_back(agent{nx, ny, CELL_TYPE::BLUE, (float)nx, (float)ny});
    }

    int old_x = agents[0].x, old_y = agents[0].y;
    update_agents();
    check(agents[0].x != old_x || agents[0].y != old_y,
          "update_agents moves unsatisfied agents");
}

// --- main ---

int main() {
    std::cout << "=== Schelling Model Tests ===" << std::endl;

    test_resize_matrix_dimensions();
    test_resize_matrix_agent_count();
    test_resize_matrix_agent_types();
    test_resize_matrix_agents_in_matrix();
    test_resize_matrix_overflow();

    test_is_satisfied_empty_cell();
    test_is_satisfied_no_neighbors();
    test_is_satisfied_all_same_neighbors();
    test_is_satisfied_mixed_neighbors_high_tolerance();
    test_is_satisfied_mixed_neighbors_low_tolerance();
    test_is_satisfied_half_same();

    test_seg_coef_empty();
    test_seg_coef_fully_segregated();
    test_seg_coef_mixed();

    test_step_animations_moves();
    test_step_animations_snap();

    test_animations_active_false();
    test_animations_active_true();

    test_update_agents_records_history();
    test_update_agents_moves_unsatisfied();

    std::cout << std::endl;
    std::cout << "Results: " << passed << " passed, " << failed << " failed" << std::endl;
    return failed > 0 ? 1 : 0;
}
