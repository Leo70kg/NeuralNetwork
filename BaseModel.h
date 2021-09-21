#ifndef __BASEMODEL_H__
#define __BASEMODEL_H__
#include <string>
#include <memory>
#include "Vector.h"
#include "BaseDataSet.h"
#include "BaseConfiguration.h"
class BaseModel
{
public:
    BaseModel(const BaseConfiguration& _config) : m_config(_config) {}
    void Fit(const BaseDataSet& trainingSet, const BaseDataSet& validateSet);
    float Test(const BaseDataSet& dataSet, float& loss);
    virtual size_t Eval(const Vector<float>& data) = 0;
    virtual bool Load(const std::string& filePath) = 0;
    virtual bool Save() = 0;
    virtual void Update() = 0;
    virtual float Loss(const OneHotVector& y);
    virtual void ClearGradient() = 0;
    virtual void ComputeGradient(const Vector<float>& x, const OneHotVector& y) = 0;

protected:
    const BaseConfiguration& m_config;
    Vector<float> y_hat;
};

#endif