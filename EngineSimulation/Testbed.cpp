#include "pch.h"
#include "Testbed.h"

#include <iostream>

#include "Engine.h"

Testbed::~Testbed()
{
}

Testbed::Testbed(std::ostream& logStream_, double timeStep_) :
    logStream(logStream_), timeStep(timeStep_), currentTime(0)
{
}

bool Testbed::OverheatTest(Engine* engine, double ambientTemp, double& result)
{
    EngineState state = engine->Reset(ambientTemp);

    //detect oscillations to terminate the test in case of stable operation
    double tempLog[2] = { ambientTemp, ambientTemp };
    double extremes[2] = { 0, 0 };
    bool extremesReached[2] = { false, false };

    while (!state.overheated)
    {
        state = engine->simulationStep(timeStep, ambientTemp);
        currentTime += timeStep;

        tempLog[0] = tempLog[1];
        tempLog[1] = state.temperature;

        if (tempLog[0] == tempLog[1] && tempLog[0] == state.temperature) // stabilised
        {
            result = state.temperature;
            return false;
        }
        else
        {

            if (tempLog[1] - tempLog[0] > 0 && state.temperature - tempLog[1] < 0) // high point
            {
                extremes[1] = tempLog[1];
                extremesReached[1] = true;
            }
            else if (tempLog[1] - tempLog[0] < 0 && state.temperature - tempLog[1] > 0) //low point
            {
                extremes[0] = tempLog[1];
                extremesReached[0] = true;
            }
        }

        if (extremesReached[0] && extremesReached[1]) // oscillating
        {
            result = (extremes[0] + extremes[1]) / 2;
            return false;
        }
    }
    
    result = currentTime;
    return true;
}
