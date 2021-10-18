#ifndef __BSDEMODEL_H__
#define __BSDEMODEL_H__

#include <memory>
#include <time.h>
#include <string>
#include "Vector.h"
#include "Equation.h"
#include "BSDEConfiguration.h"


class BSDEModel
{
public:
    BSDEModel() = default;
    BSDEModel(const BSDEConfiguration& _config);
    void Fit(const Equation& equation);
    void Setup();
    void ComputeGradient(const Equation& equation, const float y, size_t index);
    void Update();
	float Eval(const Equation& equation, size_t index);
    void ClearGradient();
    float Loss(float y, float y_pred);
    bool Save(int epoch);
	bool SaveInit();

private:
    const BSDEConfiguration& m_config;
    std::vector<Matrix<float>> weights;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
    std::vector<Vector<float>> y_layers;

	float dy;
	Vector<float> dz;

    float y_init;
    float y_init_diff;

    Vector<float> z_init;

    float delta_t;
    float y_hat;
    float loss;
	
	std::string save_file_path;
	clock_t start, end;
};



#endif
