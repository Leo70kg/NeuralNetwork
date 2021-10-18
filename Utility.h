#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <iostream>
#include <algorithm>
#include <chrono>

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

	static std::string currentTimeStampToString()
	{
		std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());  
		std::tm now_tm = *std::localtime(&t);  
		char buff[70];   
		strftime(buff, sizeof buff, "%Y%m%d%H%M%S", &now_tm);    
		std::string str(buff);
		str += ".txt";

		return str;
	}

};

#endif
