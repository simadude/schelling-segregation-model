#include "agentgrid.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    const int grid_size = 16;
    const int total_cells = grid_size * grid_size;
    const double empty_pct = 0.30;
    const int total_agents = static_cast<int>(total_cells * (1.0 - empty_pct));
    const int half = total_agents / 2;
    
    red_count = half;
    blue_count = total_agents - half;
    
    const int max_generations = 2048;
    
    std::ofstream out("batch_results.csv");
    out << "tolerance,segregation,generations" << std::endl;
    
    for (int t_pct = 0; t_pct <= 100; t_pct += 5) {
        tolerance = t_pct / 100.0;
        
        resize_matrix(grid_size);
        segregation_history.clear();
        
        int generations = 0;
        for (int gen = 0; gen < max_generations; ++gen) {
            auto prev_matrix = matrix;
            
            update_agents();
            generations++;
            
            if (matrix == prev_matrix) {
                break;
            }
        }
        
        double final_seg = get_segregation_coef();
        out << std::fixed << std::setprecision(2) << tolerance 
            << "," << std::setprecision(6) << final_seg 
            << "," << generations << std::endl;
        
        std::cout << "tolerance=" << tolerance << " seg=" << final_seg 
                  << " gens=" << generations << std::endl;
    }
    std::cout << "saved to batch_results.csv" << std::endl;
    
    out.close();
    return 0;
}
