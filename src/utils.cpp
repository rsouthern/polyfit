#include <utils.h>
#include <csvparse.h>
#include <sciplot/sciplot.hpp>
#include <functional>
#include <rapidcsv.h>

/**
 * @brief Read from a CSV file using the CSVparse library (extern). However this fails if the external file ends on a comma, so be wary!
 * Data is expected in x1,y1,x2,y2,... format.
 * @param filename CSV file
 * @param _x output x values
 * @param _y output y values
 * @return bool true if it worked.
 */
bool Utils::readSamplesFromCSV(std::vector<double> &_x,
                               std::vector<double> &_y,
                               const std::string &filename) noexcept
{
    rapidcsv::Document doc(filename, rapidcsv::LabelParams(-1, -1));
    _x = doc.GetColumn<double>(0);
    _y = doc.GetColumn<double>(1);
    return true;
}

/**
 * @brief Populate x,y vectors from a give polynomial. Note that I am aware that this is probably replaceble with a call to linspace.
 *
 * @param x_vec Vector of X values to be populated
 * @param y_vec Vector of Y values to be populated
 * @param func The function to apply - needs a double(double) signature or equivalent
 * @param x_min Minimum x-value to sample (default 0)
 * @param x_max Maximum x-value to sample (default 1)
 * @param num_samples Number of samples to generate
 * @return bool true if it worked
 */
bool Utils::generateTestData(std::vector<double> &x_vec,
                             std::vector<double> &y_vec,
                             std::function<double(const double &)> func,
                             const double &x_min,
                             const double &x_max,
                             const unsigned int &num_samples) noexcept
{
    if (num_samples <= 1)
    {
        std::cerr << "Utils::generateTestData() - num_samples must be 2 or greater!\n";
        return false;
    }

    // Calculate the x increment, which is num_samples+1 to account for if num_samples is 0
    double x_inc = (x_max - x_min) / double(num_samples - 1);

    // Resize the two vectors - note that the size is num_samples + the end points
    x_vec.resize(num_samples);
    y_vec.resize(num_samples);

    double x = x_min;
    for (unsigned int i = 0; i < num_samples; ++i, x += x_inc)
    {
        x_vec[i] = x;
        y_vec[i] = func(x);
    }
    return true;
}

/**
 * @brief Write out data to a CSV file for future loading is format x1,y1,x2,y2,...
 *
 * @param x_vec Input x vector
 * @param y_vec Input y vector (note, same size as x)
 * @param filename Filename to write to
 * @return bool true if it worked
 */
bool Utils::writeDataToCSV(const std::vector<double> &x_vec,
                           const std::vector<double> &y_vec,
                           const std::string &filename) noexcept
{
    // Clunky error checking - could use exceptions
    if (x_vec.size() != y_vec.size())
        return false;

    std::ofstream myfile;
    myfile.open(filename, std::ios::out);
    if (myfile.is_open())
    {
        auto xit = x_vec.begin();
        auto yit = y_vec.begin();
        for (; xit != x_vec.end(); ++xit, ++yit)
        {
            myfile << *xit << "," << *yit << "\n";
        }

        // Close up the input file
        myfile.close();
    }
    else
    {
        return false;
    }
    return true;
}

/**
 * @brief Plots two x,y vector curves for comparison - used to visually assess polynomial fitting. Uses sciplot, and requires gnuplot executable (will fail if this is not present)
 *
 * @param x_vec First curve x
 * @param y_vec First curve y
 * @param x_vec2 Second curve x
 * @param y_vec2 Second curve y
 * @return bool true if it works.
 */
bool Utils::plotComparison(const std::vector<double> &x_orig,
                           const std::vector<double> &y_orig,
                           const std::vector<double> &x_fit,
                           const std::vector<double> &y_fit) noexcept
{
    // Create a Plot object
    sciplot::Plot plot;

    // Set color palette
    plot.palette("set2");

    // Draw a sine graph putting x on the x-axis and sin(x) on the y-axis
    plot.drawCurve(x_orig, y_orig).label("Original").lineWidth(2);

    // Draw a cosine graph putting x on the x-axis and cos(x) on the y-axis
    plot.drawCurve(x_fit, y_fit).label("Reconstruction").lineWidth(2);

    // Show the plot in a pop-up window
    plot.show();

    // Save the plot to a PDF file
    // plot.save("plot.pdf");
    return true;
}
