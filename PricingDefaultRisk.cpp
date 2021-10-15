#include <random>
#include <chrono>
#include "PricingDefaultRisk.h"
#include "Utility.h"

PricingDefaultRisk::PricingDefaultRisk(const BSDEConfiguration& config) : Equation(config) 
{ 
	sigma = (float)0.2;
    rate = (float)0.02;   // interest rate R
    delta = (float)(2.0 / 3);
    gammah = (float)0.2;
    gammal = (float)0.02;
    mu_bar = (float)0.02;
    vh = (float)50.0;
    vl = (float)70.0;
    slope = (gammah - gammal) / (vh - vl);

    DwSample();
    XSample();
}

PricingDefaultRisk::~PricingDefaultRisk() {}

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
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();  
		std::default_random_engine gen(seed);  
		std::normal_distribution<float> dis(0.0, 1.0);
		
		dw_sample[i].resize(num_time_interval);
        
		for (int j = 0; j < num_time_interval; j++) {
            dw_sample[i][j].Resize(dim);
			
			for (int k = 0; k < dim; k++)
			{
				dw_sample[i][j][k] = static_cast<float>(dis(gen)) * sqrt_delta_t;
			}
		
			//dw_sample[i][j].Normalised(sqrt_delta_t);
		}
	}
    return true;
}

