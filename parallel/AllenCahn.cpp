#include <random>
#include <chrono>
#include "AllenCahn.h"

AllenCahn::AllenCahn(const BSDEConfiguration& config) : Equation(config)
{   
	sigma = (float) sqrt(2.0);   
	DwSample();  
	XSample();
}

AllenCahn::~AllenCahn() {}  

float AllenCahn::f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const 
{   
	return y - pow(y, 3); 
}  

float AllenCahn::f_tf_diff_y(float y) const  
{  
	return 1.0 - 3 * pow(y, 2); 
}

void AllenCahn::f_tf_diff_z(Vector<float>& diff_z, const Vector<float>& z) const
{
	diff_z.Resize(0);
}

float AllenCahn::g_tf(float t, const Vector<float>& x) const  
{  
	return 0.5 / (1.0 + 0.2 * x.SumSquare());
} 

bool AllenCahn::XSample()
{ 
	for (int i = 0; i < num_sample; i++)  
	{       
		x_sample[i].resize(num_time_interval + 1);  
		x_sample[i][0].Resize(dim);  
		
		for (int j = 1; j < num_time_interval + 1; j++) 
		{
			x_sample[i][j].Resize(dim);  
			x_sample[i][j].Add(x_sample[i][j - 1]);  
			x_sample[i][j].AddMul(sigma, dw_sample[i][j - 1]); 
		}
	}
	
	return true;  
}

bool AllenCahn::DwSample()  
{  
	for (int i = 0; i < num_sample; i++)  
	{  
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();    
		std::default_random_engine gen(seed);  
		std::normal_distribution<float> dis(0.0, 1.0);
	
		dw_sample[i].resize(num_time_interval);
		
		for (int j = 0; j < num_time_interval; j++) 
		{
			dw_sample[i][j].Resize(dim);
			
			for (int k = 0; k < dim; k++)  
			{  
				dw_sample[i][j][k] = static_cast<float>(dis(gen)) * sqrt_delta_t;
			} 
		} 
	} 
	
	return true;  
}
