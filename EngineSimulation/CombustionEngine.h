#pragma once

#include <iostream>
#include <vector>

#include "Engine.h"

class CombustionEngine : public Engine
{
public:
    enum Parameters
    {
        momentOfInertia = 0,
        overheatTemp = 1,
        heatingTorqueQuotient = 2,
        heatingRpmQuotient = 3,
        coolingTempGradientQuotient = 4
    };

    CombustionEngine(std::istream& config);
    EngineState Reset(double ambientTemp) override;
    EngineState simulationStep(double timeStep, double ambientTemp) override;
private:
    EngineState state;
    static const int paramCount = 5;
    double parameters[paramCount];

    struct DataPoint
    {
        double x;
        double y;
    };
    std::vector<DataPoint> dynoCurve;
};
