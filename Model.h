#ifndef __MODEL_H__
#define __MODEL_H__
#include <string>
#include <memory>
#include "Vector.h"
#include "DataSet.h"
#include "Configuration.h"


class Model
{
public:
    Model(const Configuration& _config) : m_config(_config) {}
	void Fit(const DataSet& trainingSet, const DataSet& validateSet);
    float Test(const DataSet& dataSet, float& loss);
    virtual size_t Eval(const Vector<float>& data) = 0;
    virtual bool Load(const std::string& filePath) = 0;
    virtual bool Save() = 0;
    virtual void Update() = 0;
    virtual float Loss(const Vector<float>& y);
    virtual void ClearGradient() = 0;
    virtual void ComputeGradient(const Vector<float>& x, const Vector<float>& y) = 0;

protected:
	const Configuration& m_config;
	Vector<float> y_hat;
};


#endif