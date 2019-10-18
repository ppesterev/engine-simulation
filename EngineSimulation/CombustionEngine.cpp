#include "pch.h"
#include "CombustionEngine.h"

#include <cstdlib>
#include <istream>
#include <string>

CombustionEngine::CombustionEngine(std::istream& config)
{
    int parameterIndex = 0;
    for (std::string configLine; std::getline(config, configLine); )
    {
        if (configLine.empty() || configLine[0] == '#')
        {
            continue;
        }
        
        if (parameterIndex < paramCount) // read single value parameters
        {
            parameters[parameterIndex] = std::strtod(configLine.data(), nullptr);
            ++parameterIndex;
        }
        else // read dyno curve
        {
            DataPoint newPoint;
            char* midpoint;
            newPoint.x = std::strtod(configLine.data(), &midpoint);
            newPoint.y = std::strtod(midpoint, nullptr);
            dynoCurve.push_back(newPoint);
        }
    }
}

EngineState CombustionEngine::Reset(double ambientTemp)
{
    state.rpm = dynoCurve.front().x;
    state.torque = dynoCurve.front().y;
    state.temperature = ambientTemp;
    state.overheated = false;
    return state;
}

EngineState CombustionEngine::simulationStep(double timeStep, double ambientTemp)
{
    if (state.overheated)
        return this->state;

    // apply torque
    state.rpm += (state.torque / parameters[momentOfInertia]) * timeStep;

    // interpolate dyno curve
    int curveIndex = 0;
    while (state.rpm >= dynoCurve[curveIndex].x && curveIndex <= dynoCurve.size())
        ++curveIndex;
    if (curveIndex >= dynoCurve.size())
    {
        state.torque = dynoCurve.back().y;
    }
    else if (curveIndex == 0)
    {
        state.torque = dynoCurve.front().y;
    }
    else
    {
        DataPoint pointLow = dynoCurve[curveIndex - 1];
        DataPoint pointHigh = dynoCurve[curveIndex];
        double factor = (state.rpm - pointLow.x) / (pointHigh.x - pointLow.x);
        state.torque = pointLow.y + (pointHigh.y - pointLow.y) * factor;
    }

    // apply heat generation and dissipation
    double newTemp = state.temperature;
    newTemp += (state.torque * parameters[heatingTorqueQuotient]) * timeStep;
    newTemp += (state.rpm * state.rpm * parameters[heatingRpmQuotient]) * timeStep;
    newTemp -= parameters[coolingTempGradientQuotient] * (state.temperature - ambientTemp) * timeStep;
    state.temperature = newTemp;

    if (state.temperature > parameters[overheatTemp])
        state.overheated = true;
    return this->state;
}
