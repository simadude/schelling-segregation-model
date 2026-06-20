#include "agentgrid.h"
#include <algorithm>
#include <random>
#include <ctime>

double tolerance = 0.3;
int red_count;
int blue_count;
std::vector<std::vector<CELL_TYPE>> matrix;
std::vector<agent> agents;

static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));

void resize_matrix(int size) {
    matrix.assign(size, std::vector<CELL_TYPE>(size, CELL_TYPE::EMPTY));
    agents.clear();

    std::vector<std::pair<int, int>> positions;
    positions.reserve(size * size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            positions.emplace_back(i, j);

    std::shuffle(positions.begin(), positions.end(), rng);

    int total = red_count + blue_count;
    if (total > size * size) total = size * size;

    for (int k = 0; k < total; ++k) {
        auto [i, j] = positions[k];
        CELL_TYPE type = (k < red_count) ? CELL_TYPE::RED : CELL_TYPE::BLUE;
        matrix[i][j] = type;
        agents.push_back(agent{i, j, type});
    }
}

static bool is_satisfied(int x, int y) {
    if (matrix[x][y] == CELL_TYPE::EMPTY) return true;

    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    CELL_TYPE self = matrix[x][y];

    int same = 0, total = 0;
    for (int k = 0; k < 8; ++k) {
        int ni = x + dx[k], nj = y + dy[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && matrix[ni][nj] != CELL_TYPE::EMPTY) {
            total++;
            if (matrix[ni][nj] == self) same++;
        }
    }

    if (total == 0) return true;
    return static_cast<double>(same) / total >= tolerance;
}

void update_agents() {
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());

    std::vector<std::pair<int, int>> empty;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (matrix[i][j] == CELL_TYPE::EMPTY)
                empty.emplace_back(i, j);

    std::shuffle(empty.begin(), empty.end(), rng);

    size_t empty_idx = 0;

    for (auto& a : agents) {
        if (is_satisfied(a.x, a.y)) continue;
        if (empty_idx >= empty.size()) break;

        auto [nx, ny] = empty[empty_idx++];
        matrix[a.x][a.y] = CELL_TYPE::EMPTY;
        matrix[nx][ny] = a.type;
        a.x = nx;
        a.y = ny;
    }
}

double get_segregation_coef() {
    if (matrix.empty() || matrix[0].empty()) return 0.0;

    int rows = matrix.size();
    int cols = matrix[0].size();
    double total_similarity = 0.0;
    int occupied_count = 0;

    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            if (matrix[i][j] == CELL_TYPE::EMPTY) continue;

            occupied_count++;
            int same_neighbors = 0;
            int valid_neighbors = 0;

            for (int k = 0; k < 8; ++k) {
                int ni = i + dx[k];
                int nj = j + dy[k];

                // Boundary check
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                    if (matrix[ni][nj] != CELL_TYPE::EMPTY) {
                        valid_neighbors++;
                        if (matrix[ni][nj] == matrix[i][j]) {
                            same_neighbors++;
                        }
                    }
                }
            }

            // Add local similarity ratio to total
            if (valid_neighbors > 0) {
                total_similarity += static_cast<double>(same_neighbors) / valid_neighbors;
            }
        }
    }

    // Return average similarity across all agents
    return occupied_count > 0 ? total_similarity / occupied_count : 0.0;
}