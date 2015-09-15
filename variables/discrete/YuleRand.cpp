#include "YuleRand.h"

YuleRand::YuleRand(double shape)
{
    setShape(shape);
}

std::string YuleRand::name()
{
    return "Yule(" + toStringWithPrecision(getShape()) + ")";
}

void YuleRand::setShape(double shape)
{
    ro = shape;
    if (ro <= 0)
        ro = MIN_POSITIVE;
    gamma1pRo = std::tgamma(ro + 1);
}

double YuleRand::P(int k) const
{
    return (k < 1) ? 0 : ro * gamma1pRo * std::tgamma(k) / std::tgamma(k + ro + 1);
}

double YuleRand::F(double x) const
{
    if (x < 1)
        return 0;
    double k = std::floor(x);
    return 1 - k * gamma1pRo * std::tgamma(k) / std::tgamma(k + ro + 1);
}

double YuleRand::variate() const
{
    return YuleRand::variate(ro);
}

double YuleRand::variate(double shape)
{
    double prob = 1.0 / ParetoRand::variate(shape, 1.0);
    return GeometricRand::variate(prob) + 1;
}

double YuleRand::Mean() const
{
    return (ro <= 1) ? INFINITY : ro / (ro - 1);
}

double YuleRand::Variance() const
{
    if (ro <= 2)
        return INFINITY;
    double aux = ro / (ro - 1);
    return aux * aux / (ro - 2);
}

double YuleRand::Mode() const
{
    return 1.0;
}

double YuleRand::Skewness() const
{
    if (ro <= 3)
        return INFINITY;
    double skewness = ro + 1;
    skewness *= skewness;
    skewness *= std::sqrt(ro - 2);
    return skewness / (ro * (ro - 3));
}

double YuleRand::ExcessKurtosis() const
{
    if (ro <= 4)
        return INFINITY;
    double numerator = 11 * ro * ro - 49;
    numerator *= ro;
    numerator -= 22;
    double denominator = ro * (ro - 4) * (ro - 3);
    return ro + 3 + numerator / denominator;
}
