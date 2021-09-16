#ifndef __LAYER_H__
#define __LAYER_H__

#include <vector>
#include <memory>
#include "LayerBase.h"
#include "Vector.h"
#include "Matrix.h"

template<typename T>
class Layer : public LayerBase
{
public:
	Layer() {}
	Layer(typename T::DT dimension) {}
	void Initialize(typename T::DT dim);
	virtual void Initialize();
	size_t Length();
	void SetData(const typename T::DataType& _d);

protected:
	typename T::DT dim;
	typename T::DataType data;
	typename T::DataType gradient;
};

template<typename T>
void Layer<T>::SetData(const typename T::DataType& _d)
{
	data = _d;
}

template<>
Layer<Tensor1>::Layer(size_t dimension) : dim(dimension)
{
	Initialize(dimension);
}

template<>
void Layer<Tensor1>::Initialize()
{
    assert(previousLayer != nullptr);
    auto _dim = previousLayer->Dim1();
    Initialize(_dim);
}

template<>
void Layer<Tensor1>::Initialize(size_t _dim)
{
    dim = _dim;
    data.Resize(dim);
    gradient.Resize(dim);
}

template<>
size_t Layer<Tensor1>::Length()
{
    return dim;
}


#endif