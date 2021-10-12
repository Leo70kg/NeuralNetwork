#ifndef __BSDEDATASET_H__
#define __BSDEDATASET_H__

#include "Vector.h"
#include "BSDEConfiguration.h"

class BSDEDataSet
{
public:
    BSDEDataSet() = default;
    BSDEDataSet(const BSDEConfiguration& config);
    bool GetXSample();
    bool GetDwSample();
    bool GetYTerminal();

    // size is the number of time intervals
    std::vector<std::vector<Vector<float>>> x_sample;
    std::vector<std::vector<Vector<float>>> dw_sample;
    std::vector<Vector<float>> y_terminal;

    float sigma;
    float lambd;
    float sqrt_delta_t;
    size_t num_sample;
    size_t dim;
    size_t num_time_interval;

    Matrix<float> x_init;
};

#endif