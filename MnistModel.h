#ifndef __MnistMODEL_H__
#define __MnistMODEL_H__

#include "BaseModel.h"
#include <string>

class MnistModel : public BaseModel
{
public:
    MnistModel() = default;
    MnistModel(const BaseConfiguration& config);
    void Setup();
    void ComputeGradient(const Vector<float>&x, const OneHotVector & y) override;
    void Update() override;
    size_t Eval(const Vector<float>&data) override;
    void ClearGradient() override;
    bool Load(const std::string & filePath) override;
    bool Save() override;
private:
    const BaseConfiguration& m_config;
    std::vector<Vector<float>> biases;
    std::vector<Matrix<float>> weights;
    std::vector<Vector<float>> d_biases;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
};

#endif