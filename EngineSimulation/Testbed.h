#pragma once

#include<iostream>

#include "Engine.h"

class Testbed
{
public:
    Testbed(std::ostream& logStream = std::cout, double timeStep = 0.01);
    ~Testbed();

    // Returns true if the engine has overheated, and places the elapsed time in result.
    // Returns false if the engine's temperature has stabilised,
    // and places the stable temperature in result.
    bool OverheatTest(Engine* engine, double ambientTemp, double& result);

private:
    std::ostream& logStream;
    double currentTime;
    const double timeStep;
};
