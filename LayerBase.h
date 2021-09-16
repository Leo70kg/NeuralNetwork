#ifndef __LAYERBASE_H__
#define __LAYERBASE_H__

#include "Vector.h"
#include "Matrix.h"

class Tensor1
{
public:
    typedef size_t DT;
    typedef Vector<float> DataType;
};

class LayerBase
{
public:
	LayerBase() : previousLayer(nullptr), nextLayer(nullptr) {}
    virtual void forward()
    {
        PrintDim();
    }
    virtual void backward()
    {
        PrintDim();
    }
    virtual LayerBase* Previous();
    virtual LayerBase& Flatten();
    virtual std::shared_ptr<LayerBase> Next();
    virtual void Initialize() {}
    LayerBase& Add(std::shared_ptr<LayerBase> _nextLayer);
    virtual size_t Dim1();
    virtual void PrintDim();
    virtual typename Tensor1::DataType& GetData1D();
    virtual typename Tensor1::DataType& GetGradient1D();
  

protected:
	LayerBase* previousLayer;
	std::shared_ptr<LayerBase> nextLayer;
};




#endif