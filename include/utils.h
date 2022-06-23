#ifndef __UTILS_H_
#define __UTILS_H_

#include <string>
#include <vector>
#include <functional>

namespace Utils
{
    /// Loads data in CSV file into x and y vectors. Data in format x1,y1,x2,y2,...
    bool readSamplesFromCSV(std::vector<double> &_x,
                            std::vector<double> &_y,
                            const std::string &filename) noexcept;

    /// Uses a given generator function to fill x and y vectors with data
    bool generateTestData(std::vector<double> &x_vec,
                          std::vector<double> &y_vec,
                          std::function<double(const double &)> func,
                          const double &x_min = 0.0,
                          const double &x_max = 1.0,
                          const unsigned int &num_samples = 100) noexcept;

    /// Write out the CSV file in format x1,y1,x2,y2,...
    bool writeDataToCSV(const std::vector<double> &x_vec,
                        const std::vector<double> &y_vec,
                        const std::string &filename = "data.csv") noexcept;

    /// Plots the comparison to the screen (PDF option also available)
    bool plotComparison(const std::vector<double> &x_vec,
                        const std::vector<double> &y_vec,
                        const std::vector<double> &x_vec2,
                        const std::vector<double> &y_vec2) noexcept;
}

#endif //__UTILS_H_