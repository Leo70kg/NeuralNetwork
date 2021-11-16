#ifndef __ALLENCAHN_H__
#define __ALLENCAHN_H__

#include "Vector.h"
#include "Equation.h"

class AllenCahn : public Equation
{
public:
    AllenCahn() = default;
	AllenCahn(const BSDEConfiguration& config);
	~AllenCahn();
	float f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const override;
    float f_tf_diff_y(float y) const override;
    void f_tf_diff_z(Vector<float>& diff_z, const Vector<float>& z) const override;
	float g_tf(float t, const Vector<float>& x) const override;
	bool XSample() override;
	bool DwSample() override;

private:
    float sigma;
    
};

#endif
