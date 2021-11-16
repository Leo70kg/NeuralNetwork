#include <random>
#include <chrono>
#include "HJBLQ.h"

HJBLQ::HJBLQ(const BSDEConfiguration& config) : Equation(config)
{   
	sigma = (float) sqrt(2.0);   
	lambd = (float) 1.0;
	DwSample();  
	XSample();  
}

HJBLQ::~HJBLQ() {}  

float HJBLQ::f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const 
{   
	return -lambd * z.SumSquare(); 
}  

float HJBLQ::f_tf_diff_y(float y) const  
{  
	return 0.0; 
}

void HJBLQ::f_tf_diff_z(Vector<float>& diff_z, const Vector<float>& z) const
{
	diff_z.Resize(z.Size());
	diff_z.AddMul(2.0, z).Mul(-lambd);

}

float HJBLQ::g_tf(float t, const Vector<float>& x) const  
{  
	return log((1.0 + x.SumSquare()) / 2.0);
} 

bool HJBLQ::XSample()  
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

bool HJBLQ::DwSample()  
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
