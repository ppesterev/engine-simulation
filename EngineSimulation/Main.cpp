#include "pch.h"
#include <iostream>
#include <fstream>

#include "Testbed.h"
#include "Engine.h"
#include "CombustionEngine.h"

int main(int argc, char** argv)
{
    std::ifstream config("engine_parameters.cfg");
    if (!config.is_open())
    {
        std::cerr << "Could not open configuration file" << std::endl;
        return 0;
    }

    try
    {
        Engine* engine = new CombustionEngine(config);
        Testbed* testbed = new Testbed();

        std::cout << "Please enter ambient temperature in degrees Celcius:" << std::endl;
        double ambientTemp;
        std::cin >> ambientTemp;
        std::cout << "Starting the test..." << std::endl;
        double result;
        bool overheat = testbed->OverheatTest(engine, ambientTemp, result);
        if (overheat)
            std::cout << "Test complete, engine overheated in "
                      << result << " seconds" << std::endl;
        else
            std::cout << "The temperature of the engine appears to have stabilised at "
                      << result << " degrees Celcius." << std::endl;
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what();
    }

    Testbed* testbed = new Testbed();

    return 0;
}
