#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>

class Utility
{
public:
    static int EndiannessSwap(int value);
    static void ToLower(std::string& str);
};



#endif