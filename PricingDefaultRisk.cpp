#include "PricingDefaultRisk.h"
#include "Utility.h"

PricingDefaultRisk::PricingDefaultRisk(const BSDEConfiguration& config) : Equation(config) 
{ 
	sigma = 0.2;
    rate = 0.02;   // interest rate R
    delta = 2.0 / 3;
    gammah = 0.2;
    gammal = 0.02;
    mu_bar = 0.02;
    vh = 50.0;
    vl = 70.0;
    slope = (gammah - gammal) / (vh - vl);

    DwSample();
    XSample();
}


float PricingDefaultRisk::f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const
{
    float piecewise_linear = Utility<float>::Relu(Utility<float>::Relu(y - vh) * slope + gammah - gammal) + gammal;
    
    return (-(1 - delta) * piecewise_linear - rate) * y;

}

float PricingDefaultRisk::f_tf_diff_y(float y) const 
{
    float relu = Utility<float>::Relu(y - vh);

    return slope * Utility<float>::ReluDiff(relu * slope + gammah - gammal)* Utility<float>::ReluDiff(relu);
}



float PricingDefaultRisk::g_tf(float t, const Vector<float>& x) const
{
    return x.Min();
}

bool PricingDefaultRisk::XSample()
{
   
    for (int i = 0; i < num_sample; i++)
    {
        x_sample[i].resize(num_time_interval + 1);
        x_sample[i][0].Resize(dim, 100.0);

        for (int j = 1; j < num_time_interval + 1; j++) {
            x_sample[i][j].Resize(dim);
            x_sample[i][j].Add(1 + mu_bar * delta_t, x_sample[i][j - 1]);
            x_sample[i][j].AddMul(sigma, x_sample[i][j - 1], dw_sample[i][j - 1]);
        }
    }

    return true;
}

bool PricingDefaultRisk::DwSample()
{
    for (int i = 0; i < num_sample; i++)
    {
        dw_sample[i].resize(num_time_interval);
        for (int j = 0; j < num_time_interval; j++) {
            dw_sample[i][j].Resize(dim);
            dw_sample[i][j].Normalised(sqrt_delta_t, (i + j) * (j + dim));
        }

    }

    return true;
}