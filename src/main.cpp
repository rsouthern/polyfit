#include <gtest/gtest.h>
#include <polynomial.h>
#include <tclap/CmdLine.h>

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
        }
    }
    catch (TCLAP::ArgException &e) // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
