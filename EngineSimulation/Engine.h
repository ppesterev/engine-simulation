#pragma once

struct EngineState
{
    double rpm;
    double torque;
    double temperature;
    bool overheated;
};

class Engine
{
public:
    virtual EngineState Reset(double ambientTemp) = 0;
    virtual EngineState simulationStep(double timeStep, double ambientTemp) = 0;
};
