#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include "Matrix.h"


template<class T>
class Matrix;

template<class T>
class Vector
{
public:
	Vector();
	Vector(size_t len);
	Vector(T* data, size_t len);

	virtual ~Vector();

	T* Data();
	size_t Size() const;
	
	void Print(std::ofstream& fout, int col = 0);
	void Print();

	Vector(const std::vector<T>& _data);
	Vector(const Vector<T>& other);
	Vector(Vector<T>&& other);

	void Resize(size_t len);
	void Resize(size_t len, T value);
	void ResizeUniformDist(size_t len, T low, T high);
	void Reset(T value);

	T operator[] (size_t index) const;
	T& operator[] (size_t index);
	Vector<T>& operator = (T val);
	Vector<T>& operator = (const Vector<T>& other);
	Vector<T>& operator = (Vector<T>&& other);

	Vector<T>& Add(const Vector<T>& other);
	Vector<T>& Add(T scale, const Vector<T>& other);
	Vector<T>& Add(const Vector<T>& other1, const Vector<T>& other2);
	Vector<T>& Add(const Vector<T>& other, T val);
	Vector<T>& Add(T val);
	Vector<T>& Sub(const Vector<T>& other);
	Vector<T>& Sub(T scale, const Vector<T>& other);
	Vector<T>& Sub(const Vector<T>& other, T val);
	Vector<T>& Mul(const Vector<T>& other);
	Vector<T>& Mul(T val);
	Vector<T>& Mul(T scale, const Vector<T>& other);
	Vector<T>& Div(T val);
	Vector<T>& AddMul(T scale, const Vector<T>& other);
	Vector<T>& AddMul(const Matrix<T>& m, const Vector<T>& v);
	Vector<T>& AddMul(T scale, const Vector<T>& other1, const Vector<T>& other2);
	Vector<T>& AssignMul(const Matrix<T>& W, const Vector<T>& v);
	Vector<T>& MulScalarVecSub(T value, const Vector<T>& other);
	Vector<T>& AssignMulTMat(const Matrix<T>& W, const Vector<T>& v);
	Vector<T>& ReduceSum(const Matrix<T>& m);
	T Sum();
	T SumSquare() const;
	T Min() const;

	void FanInFanOutRandomize();
	Vector<T>& Normalised(T scale);
	Vector<T>& UniformDist(T low, T high);

	Vector<T>& operator += (const Vector<T>& other);
	Vector<T>& operator -= (const Vector<T>& other);
	Vector<T>& operator *= (T val);
	Vector<T>& SoftMax();
	Vector<T>& Sigmod();
	Vector<T>& Tanh();
	Vector<T>& Relu();
	Vector<T>& ReluDiff();
	std::pair<T, size_t> Max() const;

	template<class U>
	friend std::ofstream& operator << (std::ofstream& fout, const Vector<U>& v);
	template<class U>
	friend std::ifstream& operator >> (std::ifstream& fin, Vector<U>& v);

	template<class U>
	friend class Matrix;

	static T ReduceSum(const Vector<T>& other1, const Vector<T>& other2) {
		T res = 0;
		for (int i = 0; i < other1.v_len; ++i)
		{
			res += other1.v_data[i] * other2.v_data[i];
		}

		return res;
	}

private:
	size_t v_len;
	T* v_data;
};


template<class T>
Vector<T>::Vector()
{
	v_len = 1;
	v_data = new T[v_len];
	memset(v_data, 0, sizeof(T) * v_len);
}


template<class  T>
Vector<T>::~Vector()
{
	if (v_data != nullptr)
	{
		delete[] v_data;
	}
}

template<class T>
Vector<T>::Vector(size_t len)
{
	v_len = len;
	v_data = new T[len];
	memset(v_data, 0, sizeof(T) * len);
}

template<class T>
Vector<T>::Vector(const std::vector<T>& _data)
{
	v_len = _data.size();
	v_data = nullptr;
	if (v_len > 0)
	{
		v_data = new T[v_len];
		std::copy(_data.begin(), _data.end(), v_data);
	}
}

template<class T>
Vector<T>::Vector(const Vector<T>& other)
{
	v_len = other.v_len;
	v_data = new T[v_len];
	std::copy(other.v_data, other.v_data + v_len, v_data);
}

template<class T>
Vector<T>::Vector(Vector<T>&& other)
{
	v_len = other.v_len;
	v_data = other.v_data;
	other.v_data = nullptr;
}

template<class T>
Vector<T>::Vector(T* data, size_t len)
{
	v_len = len;
	v_data = new T[len];
	for (int i = 0; i < len; ++i)
	{
		v_data[i] = (T)data[i];
	}
}

template<class T>
T* Vector<T>::Data()
{
	return v_data;
}

template<class T>
size_t Vector<T>::Size() const
{
	return v_len;
}

template<class T>
T Vector<T>::operator[] (size_t index) const
{
	assert(index < v_len);
	return v_data[index];
}

template<class T>
T& Vector<T>::operator[] (size_t index)
{
	assert(index < v_len);
	return v_data[index];
}

template<class T>
Vector<T>& Vector<T>::operator = (T val)
{
	for (int i = 0; i < v_len; ++i)
		v_data[i] = val;
	return *this;
}

template<class T>
Vector<T>& Vector<T>::operator = (const Vector<T>& other)
{
	if (v_len != other.v_len)
	{
		delete[]v_data;
		v_data = new T[other.v_len];
		v_len = other.v_len;
	}

	std::copy(other.v_data, other.v_data + v_len, v_data);
	return *this;
}

template<class T>
Vector<T>& Vector<T>::operator = (Vector<T>&& other)
{
	std::swap(v_len, other.v_len);
	std::swap(v_data, other.v_data);
	return *this;
}


template<class T>
void Vector<T>::Resize(size_t len)
{
	if (v_len != len)
	{
		v_len = len;
		delete[]v_data;
		v_data = new T[v_len];
		memset(v_data, 0, sizeof(T) * v_len);
	}
}

template<class T>
void Vector<T>::Resize(size_t len, T value)
{
	if (v_len != len)
	{
		Resize(len);
	}
	
	for (size_t i = 0; i < v_len; ++i)
	{
		v_data[i] = value;
	}
}

template<class T>
void Vector<T>::ResizeUniformDist(size_t len, T low, T high)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(low, high);

	if (v_len != len)
	{
		v_len = len;
		delete[]v_data;
		v_data = new T[v_len];
		memset(v_data, distribution(generator), sizeof(T) * v_len);
	}
}

template<class T>
void Vector<T>::Reset(T value)
{
	for (int i = 0; i < v_len; i++) {
		v_data[i] = value;
	}
}

template<class T>
Vector<T>& Vector<T>::AddMul(T scale, const Vector<T>& other)
{
	for (size_t i = 0; i < v_len; ++i)
	{
		v_data[i] += scale * other.v_data[i];
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::AddMul(const Matrix<T>& m, const Vector<T>& v)
{
	T* mdata = m.m_data;
	for (int i = 0; i < v_len; i++)
	{
		for (int j = 0; j < v.v_len; j++, mdata++)
		{	
			v_data[i] += *mdata + v.v_data[j];
		}
	}
	return *this;
}

template<class T>
Vector<T>& Vector<T>::AddMul(T scale, const Vector<T>& other1, const Vector<T>& other2)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] += other1.v_data[i] * other2.v_data[i] * scale;
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::AssignMul(const Matrix<T>& W, const Vector<T>& v)
{
	Resize(W.row);
	T* wdata = W.m_data;
	T* data = v_data;
	for (size_t i = 0; i < v_len; i++, data++)
	{
		T* vdata = v.v_data;
		T sum = 0;
		for (size_t j = 0; j < W.col; j++, vdata++, wdata++)
		{
			sum += *wdata * *vdata;
		}
		*data = sum;
	}
	return *this;
}

template<class T>
Vector<T>& Vector<T>::MulScalarVecSub(T value, const Vector<T>& other)
{
	for (int i = 0; i < v_len; i++)
		v_data[i] *= value - other.v_data[i];
	return *this;
}

template<class T>
Vector<T>& Vector<T>::AssignMulTMat(const Matrix<T>& W, const Vector<T>& v)
{
	Resize(W.col);
	*this = 0;
	T* wdata = W.m_data;
	T* data = v_data;
	T* vdata = v.v_data;
	for (size_t i = 0; i < W.row; i++, vdata++)
	{
		for (size_t j = 0; j < W.col; j++, wdata++)
		{
			data[j] += *wdata * *vdata;
		}
	}

	return *this;
}

// Each row of Matrix summation
template<class T>
Vector<T>& Vector<T>::ReduceSum(const Matrix<T>& m)
{
	if (v_len != m.row)
	{
		Resize(m.row);
		*this = 0;
	}
	T* data = v_data;
	T* mdata = m.m_data;
	for (size_t i = 0; i < m.row; i++, mdata++)
	{
		for (size_t j = 0; j < m.col; j++, mdata++)
		{
			data[i] += *mdata;
		}
	}
	
	return *this;
}

template<class T>
T Vector<T>::Sum()
{	
	T sum = 0;
	for (int i = 0; i < v_len; ++i)
	{
		sum += v_data[i];
	}

	return sum;
}

template<class T>
T Vector<T>::SumSquare() const
{
	T sum = 0;
	for (int i = 0; i < v_len; ++i)
	{
		sum += v_data[i] * v_data[i];
	}

	return sum;
}

template<class T>
T Vector<T>::Min() const
{
	T _min = v_data[0];
	for (int i = 1; i < v_len; ++i)
	{
		_min = _min < v_data[i] ? _min : v_data[i];
	}

	return _min;
}

template<class T>
Vector<T>& Vector<T>::operator += (const Vector<T>& other)
{
	for (size_t i = 0; i < v_len; ++i)
	{
		v_data[i] += other[i];
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::operator -= (const Vector<T>& other)
{
	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] -= other[i];
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::operator *= (T val)
{
	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] *= val;
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::SoftMax()
{
	T m = *std::max_element(v_data, v_data + v_len);
	float sum = 0;
	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] = exp(v_data[i] - m);
		sum += v_data[i];
	}

	sum = 1.0f / sum;

	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] *= sum;
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Sigmod()
{
	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] = static_cast<T>(1.0 / (1 + exp(-v_data[i])));
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Tanh()
{
	for (size_t i = 0; i < v_len; i++)
	{
		float value1 = exp(v_data[i]);
		float value2 = exp(-v_data[i]);
		v_data[i] = static_cast<T>((value1 - value2) / (value1 + value2));
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Relu()
{
	for (size_t i = 0; i < v_len; i++)
	{
		if (v_data[i] < 0)
		{
			v_data[i] = 0;
		}
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::ReluDiff()
{
	for (size_t i = 0; i < v_len; i++)
	{
		if (v_data[i] <= 0)
		{
			v_data[i] = (T)0;
		}
		else {
			v_data[i] = (T)1;
		}
	}

	return *this;
}

template<class T>
std::pair<T, size_t> Vector<T>::Max() const
{
	auto res = std::make_pair<T, size_t>(0, -1);
	if (v_len > 0)
	{
		T* p = std::max_element(v_data, v_data + v_len);
		res.first = *p;
		res.second = p - v_data;
	}

	return res;
}

template<class T>
std::ofstream& operator << (std::ofstream& fout, const Vector<T>& v)
{
	fout << v.v_len << std::endl;
	for (int i = 0; i < v.v_len; i++)
		fout << v.v_data[i] << " ";
	fout << std::endl;
	return fout;
}

template<class T>
std::ifstream& operator >> (std::ifstream& fin, Vector<T>& v)
{
	fin >> v.v_len;
	v.Resize(v.v_len);
	for (int i = 0; i < v.v_len; i++)
		fin >> v.v_data[i];
	return fin;
}

template<class T>
void Vector<T>::Print(std::ofstream& fout, int col)
{
	if (col == 0)
	{
		for (int i = 0; i < v_len; ++i)
		{
			fout << v_data[i] << " ";
		}
		fout << std::endl;
		return;
	}

	for (int i = 0; i < v_len; ++i)
	{
		fout << v_data[i] << " ";
		if (i % col == col - 1)
		{
			fout << std::endl;
		}
	}
}

template<class T>
void Vector<T>::Print()
{
	for (int i = 0; i < v_len; ++i)
	{
		std::cout << v_data[i] << " ";
	}
	std::cout << std::endl;
	return;
}

template<class T>
Vector<T>& Vector<T>::Add(const Vector<T>& other)
{
	*this += other;
	return *this;
}

template<class T>
Vector<T>& Vector<T>::Add(T scale, const Vector<T>& other)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] += scale * other.v_data[i];
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Add(const Vector<T>& other1, const Vector<T>& other2)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] = other1.v_data[i] + other2.v_data[i];
	}

	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::Add(const Vector<T>& other, T val)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] = other.v_data[i] + val;
	}

	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::Add(T val)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] += val;
	}

	return *this;
}


template<class T>
Vector<T>& Vector<T>::Sub(const Vector<T>& other)
{
	*this -= other;
	return *this;
}

template<class T>
Vector<T>& Vector<T>::Sub(T scale, const Vector<T>& other)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] -= scale * other.v_data[i];
	}

	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::Sub(const Vector<T>& other, T val)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] = other.v_data[i] - val;
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Mul(const Vector<T>& other)
{
	for (int i = 0; i < v_len; ++i)
		v_data[i] = v_data[i] * other.v_data[i];
	return *this;
}

template<class T>
Vector<T>& Vector<T>::Mul(T val)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] *= val;
	}

	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::Mul(T scale, const Vector<T>& other)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] = scale * other.v_data[i];
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::Div(T val)
{
	for (int i = 0; i < v_len; ++i)
	{
		v_data[i] /= val;
	}

	return *this;
}


template<class T>
void Vector<T>::FanInFanOutRandomize()
{
	int seed = 3; 
	std::default_random_engine gen(seed);
	std::normal_distribution<T> dis(0, 1);
	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] = static_cast<T>(dis(gen));
	}
}

template<class T>
Vector<T>& Vector<T>::Normalised(T scale)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::normal_distribution<T> dis(0.0, 1.0);
	
	for (int i = 0; i < v_len; i++)
	{
		v_data[i] = static_cast<T>(dis(gen)) * scale;
	}

	return *this;
}

template<class T>
Vector<T>& Vector<T>::UniformDist(T low, T high)
{	
	std::default_random_engine generator;
	std::uniform_real_distribution<T> distribution(low, high);

	for (size_t i = 0; i < v_len; i++)
	{
		v_data[i] = static_cast<T>(distribution(generator));
	}

	return *this;
}

#endif
