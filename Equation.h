#ifndef __EQUATION_H__
#define __EQUATION_H__
#include "Vector.h"
#include "BSDEConfiguration.h"

class Equation
{
public:
    Equation() = default;
    Equation(const BSDEConfiguration& config);
   
    virtual float f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const = 0;
    virtual float f_tf_diff_y(float y) const = 0;
    virtual float g_tf(float t, const Vector<float>& x) const = 0;
    virtual bool XSample() = 0;
    virtual bool DwSample() = 0;
    const std::vector<std::vector<Vector<float>>>& GetXSample() const;
    const std::vector<std::vector<Vector<float>>>& GetDwSample() const;

protected:
    size_t dim;
    float total_time;
    size_t num_sample;
    size_t num_time_interval;
    float delta_t;
    float sqrt_delta_t;
    //float y_init;

    // size is the number of time intervals
    std::vector<std::vector<Vector<float>>> x_sample;
    std::vector<std::vector<Vector<float>>> dw_sample;
};

#endif