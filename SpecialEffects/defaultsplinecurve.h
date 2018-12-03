#ifndef DEFAULTSPLINECURVE_H
#define DEFAULTSPLINECURVE_H

#include <gmParametricsModule>

class defaultsplinecurve : public GMlib::PBSplineCurve<float>
{
public:
    using PBSplineCurve::PBSplineCurve;

protected:
    void localSimulate(double dt) override;
};

#endif // DEFAULTSPLINECURVE_H
