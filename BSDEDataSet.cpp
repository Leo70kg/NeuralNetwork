#include "BSDEDataSet.h"

#include <math.h> 
#include <iostream>
#include <sstream>

BSDEDataSet::BSDEDataSet(const BSDEConfiguration& config)
{
    sigma = sqrt(2.0);
    lambd = 1.0;
    num_sample = config.sampleSize;
    num_time_interval = config.numTimeInterval;
    sqrt_delta_t = sqrt(config.totalTime / config.numTimeInterval);
    dim = config.feature_number;
    x_init.Resize(num_sample, dim);

    GetDwSample();
    GetXSample();
    GetYTerminal();
    
}

bool BSDEDataSet::GetXSample()
{
    x_sample.resize(num_sample);
    for (int i = 0; i < num_sample; i++)
    {
        x_sample[i].resize(num_time_interval + 1);
        x_sample[i][0].Resize(dim);

        for (int j = 1; j < num_time_interval + 1; j++) {
            x_sample[i][j].Resize(dim);
            x_sample[i][j].Add(x_sample[i][j-1], dw_sample[i][j-1].Mul(sigma));

        }
    }

    return true;
}

bool BSDEDataSet::GetDwSample()
{
    dw_sample.resize(num_sample);
    for (int i = 0; i < num_sample; i++)
    {
        dw_sample[i].resize(num_time_interval);
        for (int j = 0; j < num_time_interval; j++) {
            dw_sample[i][j].Resize(dim);
            dw_sample[i][j].Normalised(sqrt_delta_t, (i+j) * (j+dim));
        }

    }

    return true;
}

bool BSDEDataSet::GetYTerminal()
{
    y_terminal.resize(num_sample);
    for (int i = 0; i < num_sample; i++)
    {
        float res = log((x_sample[i][num_time_interval].SumSquare() + 1.0) * 0.5);
        y_terminal[i].Resize(1, res);
    }

    return true;
}
