#include "agentgrid.h"

double tolerance;
int red_count;
int blue_count;
std::vector<std::vector<CELL_TYPE>> matrix;
std::vector<agent> agents;

void resize_matrix(int size) {
    matrix.assign(size, std::vector<CELL_TYPE>(size, CELL_TYPE::EMPTY));
    agents.clear();
    int rows = matrix.size();
    int cols = matrix[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] != CELL_TYPE::EMPTY) agents.push_back(agent{i, j, matrix[i][j]});
        }
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