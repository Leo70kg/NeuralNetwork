#ifndef __BSDEMODEL_H__
#define __BSDEMODEL_H__

#include <memory>
#include "Vector.h"
#include "Equation.h"
#include "BSDEConfiguration.h"
#include <string>

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
    bool Save();


private:
    const BSDEConfiguration& m_config;
    std::vector<Matrix<float>> weights;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
    std::vector<Vector<float>> y_layers;
 /*   std::vector<Vector<float>> z_layers;*/

    float y_init;
    float y_init_diff;

    Vector<float> z_init;
    Vector<float> z_init_diff;
    Vector<float> z_init_diff_mean;

    float delta_t;
    float y_hat;
};



#endif