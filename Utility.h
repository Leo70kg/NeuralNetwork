#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Vector.h"

template<class T>
class Utility
{
public:
	static T Relu(T value) {
		if (value < 0)
		{
			value = 0;
		}

		return value;
	}

	static T ReluDiff(T value) {
		
		if (value <= 0)
		{
			return (T)0.0;
		}
		else {
			return (T)1.0;
		}
		
	}

	/*static T ReduceSum(const Vector<T>& other1, const Vector<T>& other2) {
		T res = 0;
		for (int i = 0; i < other1.Size(); ++i)
		{
			res += other1.Data()[i] * other2.Data()[i];
		}

		return res;
	}*/

};

#endif