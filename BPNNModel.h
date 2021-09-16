#ifndef __BPNNMODEL_H__
#define __BPNNMODEL_H__

#include "Model.h"
#include "BPNNConfiguration.h"
#include <string>

class BPNNModel : public Model
{
public:
	BPNNModel() = default;
	BPNNModel(const BPNNConfiguration& config);
    void Setup();
    void ComputeGradient(const Vector<float>& x, const Vector<float>& y) override;
    void Update() override;
    size_t Eval(const Vector<float>& data) override;
    void ClearGradient() override;
    bool Load(const std::string& filePath) override;
    bool Save() override;

private:
    const BPNNConfiguration& m_config;
    std::vector<Vector<float>> biases;
    std::vector<Matrix<float>> weights;
    std::vector<Vector<float>> d_biases;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
};


#endif