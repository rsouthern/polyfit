#include <gtest/gtest.h>
#include <polynomial.h>
#include <tclap/CmdLine.h>

#include <iostream>
#include <fstream>
#include <utils.h>
#include <cmath>
#include <functional>

/**
 * @brief Command line application to fit, evaluate and plot a polynomial of a given order. polyfit --help for instructions.
 *
 * @param argc
 * @param argv
 * @return int EXIT_SUCCESS if everything groovy.
 */
int main(int argc, char **argv)
{
    try
    {
        // Setup our command line parsing arguments and switches
        TCLAP::CmdLine cmd("Polyfit: a command line application to fit a univariate polynomial to arbitrary data. Requires gluplot executable in your path to plot. Typical use case:\
        \n\t./polyfit -g (generate some data to fit)\n\t./polyfit -o 3 -p -d sin.csv (fit an order 5 polygon and plot)\
        \nTry increasing the order to improve the fit.",
                           ' ', "0.1");
        TCLAP::ValueArg<std::string> dataArg("d", "data", "CSV data file", false, "", "string");
        TCLAP::ValueArg<std::string> orderArg("o", "order", "Order / degree of polynomial (default 3)", false, "", "unsigned int");
        TCLAP::SwitchArg plotSwitch("p", "plot", "generate comparison plot from data", cmd, false);
        TCLAP::SwitchArg testSwitch("t", "test", "run tests", cmd, false);
        TCLAP::SwitchArg genSwitch("g", "generate", "generate sin data in .csv file (will use name specified in data arg, or sin.csv if none specified)", cmd, false);
        cmd.add(dataArg);
        cmd.add(orderArg);

        // Parse the argv array.
        cmd.parse(argc, argv);

        // Run the suite of tests and exit
        if (testSwitch.getValue())
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }

        // If the user wants to generate data, generate a sin function and dump to CSV
        if (genSwitch.getValue())
        {
            // Give our output a filename
            std::string filename = (dataArg.getValue().empty()) ? "sin.csv" : dataArg.getValue();
            // Some vectors to put stuff in
            std::vector<double> x_vec, y_vec;
            // Generate some data from our sin function (note it requires disambiguation, hence the clunky definition)
            std::function<double(double)> d_sin = (double (*)(double)) & std::sin;
            Utils::generateTestData(x_vec, y_vec, d_sin, 0.0, 2 * M_PI);
            // Write this out to a filename
            Utils::writeDataToCSV(x_vec, y_vec, filename);
            std::cout << "Generated " << filename << " with sin data.\n";
            return EXIT_SUCCESS;
        }

        // If neither of the switches was used we get cracking on fitting a polynomial
        int order = (orderArg.getValue().empty()) ? 3 : std::stoi(orderArg.getValue());

        // Create a polynomial and set it's order
        Polynomial p(order);

        if (dataArg.getValue().empty())
        {
            TCLAP::ArgException e("No data file specified", "-d");
            throw e;
        }

        // Fit polydata from CSV File
        std::vector<double> x_orig, y_orig;
        Utils::readSamplesFromCSV(x_orig, y_orig, dataArg.getValue());
        p.fitData(x_orig, y_orig);

        // Test the polynomial by evaluating a bunch of values
        std::cout << "Fitted order " << order << " polynomial with RMS " << p.getLastFittingError() << "\n";

        if (plotSwitch.getValue())
        {
            // If all goes well, we can generate some data from our polynomial, using the bounds in our input data
            std::vector<double> x_fit, y_fit;

            // Create a function for our integrator (thank goodness for auto)
            auto p_eval = std::bind(&Polynomial::eval,      // This is our member function to us
                                    p,                      // First parameter is a pointer to the object
                                    std::placeholders::_1); // 2nd parameter will be the 1st parameter passed to it

            // Fill up the data vectors
            Utils::generateTestData(x_fit, y_fit, p_eval, *(x_orig.begin()), *(std::prev(x_orig.end())), x_orig.size());

            // Plot the comparison of the fitted and original data
            Utils::plotComparison(x_orig, y_orig, x_fit, y_fit);
        }
    }
    catch (TCLAP::ArgException &e) // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
