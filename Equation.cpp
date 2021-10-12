#include "Equation.h"


Equation::Equation(const BSDEConfiguration& config)
{
    num_sample = config.sampleSize;
    total_time = config.totalTime;
    num_time_interval = config.numTimeInterval;
    delta_t = config.totalTime / config.numTimeInterval;
    sqrt_delta_t = sqrt(delta_t);
    dim = config.feature_number;

    x_sample.resize(num_sample);
    dw_sample.resize(num_sample);
}

const std::vector<std::vector<Vector<float>>>& Equation::GetXSample() const
{
    return x_sample;
}

const std::vector<std::vector<Vector<float>>>& Equation::GetDwSample() const
{
    return dw_sample;
}