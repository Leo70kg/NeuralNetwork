#ifndef __PRICINGDEFAULTRISK_H__
#define __PRICINGDEFAULTRISK_H__

#include "Vector.h"
#include "Equation.h"

class PricingDefaultRisk : public Equation
{
public:
    PricingDefaultRisk() = default;
    PricingDefaultRisk(const BSDEConfiguration& config);
    ~PricingDefaultRisk();
	float f_tf(float t, const Vector<float>&x, float y, const Vector<float>&z) const override;
    float f_tf_diff_y(float y) const override;
    void f_tf_diff_z(Vector<float>& diff_z, const Vector<float>& z) const override;
	float g_tf(float t, const Vector<float>&x) const override;
    bool XSample() override;
    bool DwSample() override;

private:
    float sigma;
    float rate;
    float delta;
    float gammah;
    float gammal;
    float mu_bar;
    float vh;
    float vl;
    float slope;

};

#endif
