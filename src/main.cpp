#include <gtest/gtest.h>
#include <polynomial.h>
#include <tclap/CmdLine.h>

#include <sciplot/sciplot.hpp>
using namespace sciplot;

int plotTest()
{
    // Create values for your x-axis
    Vec x = linspace(0.0, 5.0, 100);

    // Create a Plot object
    Plot plot;

    // Set color palette
    plot.palette("set2");

    // Draw a sine graph putting x on the x-axis and sin(x) on the y-axis
    plot.drawCurve(x, std::sin(x)).label("sin(x)").lineWidth(4);

    // Draw a cosine graph putting x on the x-axis and cos(x) on the y-axis
    plot.drawCurve(x, std::cos(x)).label("cos(x)").lineWidth(4);

    // Show the plot in a pop-up window
    plot.show();

    // Save the plot to a PDF file
    plot.save("plot.pdf");
}



int main(int argc, char **argv)
{

    try
    {
        // tclap example originally from https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
        TCLAP::CmdLine cmd("polyfit command line", ' ', "1.0");
        TCLAP::ValueArg<std::string> dataArg("d", "data", "CSV data file", false, "", "string");
        TCLAP::ValueArg<std::string> orderArg("o", "order", "Order / degree of polynomial", false, "", "unsigned int");
        TCLAP::SwitchArg testSwitch("t", "test", "run tests", cmd, false);
        cmd.add(dataArg);
        cmd.add(orderArg);

        // Parse the argv array.
        cmd.parse(argc, argv);

        if (testSwitch.getValue())
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }
        else
        {
            // Create a polynomial and set it's order
            Polynomial p(std::stoi(orderArg.getValue()));

            // Fit polydata from CSV File
            p.fitData(dataArg.getValue());

            // Test the polynomial by evaluating a bunch of values
            std::cout << "Last fitting error=" << p.getLastFittingError() << ", p(1)=" << p(1) << ", p(3)=" << p(3) << "\n";

	    plotTest();
        }
    }
    catch (TCLAP::ArgException &e) // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
