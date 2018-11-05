#ifndef MYBSPLINECURVE_H
#define MYBSPLINECURVE_H

#include <gmParametricsModule>

class mybsplinecurve : public GMlib::PBSplineCurve<float>
{
public:
    using PBSplineCurve::PBSplineCurve;

protected:
    void localSimulate(double dt) override;
};

#endif // MYBSPLINECURVE_H
