#ifndef __HJBLQ_H__
#define __HJBLQ_H__

#include "Vector.h"
#include "Equation.h"

class HJBLQ : public Equation
{
public:
    HJBLQ() = default;
    HJBLQ(const BSDEConfiguration& config);
	~HJBLQ();
	float f_tf(float t, const Vector<float>& x, float y, const Vector<float>& z) const override;
    float f_tf_diff_y(float y) const override;
    void f_tf_diff_z(Vector<float>& diff_z, const Vector<float>& z) const override;
	float g_tf(float t, const Vector<float>& x) const override;
	bool XSample() override;
	bool DwSample() override;

private:
    float sigma;
    float lambd;
};

#endif
