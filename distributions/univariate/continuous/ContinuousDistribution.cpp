#include "ContinuousDistribution.h"

void ContinuousDistribution::ProbabilityDensityFunction(const std::vector<double> &x, std::vector<double> &y) const
{
    size_t size = x.size();
    if (size > y.size())
        return;
    for (size_t i = 0; i != size; ++i)
        y[i] = f(x[i]);
}

double ContinuousDistribution::quantileImpl(double p) const
{
    double guess = 0.0;
    SUPPORT_TYPE supp = SupportType();
    if (supp == FINITE_T) {
        if (RandMath::findRoot([this, p] (double x)
        {
            return F(x) - p;
        }, MinValue(), MaxValue(), guess))
            return guess;
        return NAN;
    }

    /// We use quantile from sample as an initial guess
    static constexpr int SAMPLE_SIZE = 100;
    std::vector<double> sample(SAMPLE_SIZE);
    this->Sample(sample);
    std::sort(sample.begin(), sample.end());
    guess = sample[p * SAMPLE_SIZE];

    if (RandMath::findRoot([this, p] (double x)
    {
        double first = F(x) - p;
        double second = f(x);
        return DoublePair(first, second);
    }, guess))
        return guess;
    return NAN;
}

double ContinuousDistribution::quantileImpl1m(double p) const
{
    double guess = 0.0;
    SUPPORT_TYPE supp = SupportType();
    if (supp == FINITE_T) {
        if (RandMath::findRoot([this, p] (double x)
        {
            double y = F(x) - 1.0;
            return y + p;
        }, MinValue(), MaxValue(), guess))
            return guess;
        return NAN;
    }

    /// We use quantile from sample as an initial guess
    static constexpr int SAMPLE_SIZE = 100;
    std::vector<double> sample(SAMPLE_SIZE);
    this->Sample(sample);
    /// Sort in desceding order
    std::sort(sample.begin(), sample.end(), std::greater<>());
    guess = sample[p * SAMPLE_SIZE];

    if (RandMath::findRoot([this, p] (double x)
    {
        double first = F(x) - 1;
        first += p;
        double second = f(x);
        return DoublePair(first, second);
    }, guess))
        return guess;
    return NAN;
}

double ContinuousDistribution::Hazard(double x) const
{
    if (x < MinValue())
        return 0.0; /// 0/1
    if (x > MaxValue())
        return NAN; /// 0/0
    return f(x) / S(x);
}

double ContinuousDistribution::Mode() const
{
    double guess = Mean(); /// good starting point
    if (!std::isfinite(guess))
        guess = Median(); /// this shouldn't be nan or inf
    double root = 0;
    RandMath::findMin([this] (double x)
    {
        return -f(x);
    }, guess, root);
    return root;
}

double ContinuousDistribution::GetLeftLimit(double value, double epsilon) const
{
    if (!std::isfinite(f(value))) {
        if (std::fabs(value) < 1)
            value -= epsilon;
        else
            value *= 0.9999;
    }
    return value;
}

double ContinuousDistribution::GetRightLimit(double value, double epsilon) const
{
    if (!std::isfinite(f(value))) {
        if (std::fabs(value) < 1)
            value += epsilon;
        else
            value *= 1.0001;
    }
    return value;
}

double ContinuousDistribution::ExpectedValue(const std::function<double (double)> &funPtr, double minPoint, double maxPoint) const
{
    /// attempt to calculate expected value by numerical method
    /// use for distributions w/o explicit formula
    static constexpr double epsilon = 1e-10;
    double lowerBoundary = minPoint, upperBoundary = maxPoint;
    if (isRightBounded()) {
        lowerBoundary = std::max(minPoint, lowerBoundary);
        lowerBoundary = GetLeftLimit(lowerBoundary, epsilon);
    }
    if (isLeftBounded()) {
        upperBoundary = std::min(maxPoint, upperBoundary);
        upperBoundary = GetRightLimit(upperBoundary, epsilon);
    }

    if (lowerBoundary >= upperBoundary)
        return 0.0;

    bool leftBounded = std::isfinite(lowerBoundary), rightBounded = std::isfinite(upperBoundary);

    if (leftBounded && rightBounded) {
        return RandMath::integral([this, funPtr] (double x)
        {
            return funPtr(x) * f(x);
        },
        lowerBoundary, upperBoundary);
    }

    if (leftBounded) {
        return RandMath::integral([this, funPtr, lowerBoundary] (double x)
        {
            if (x >= 1.0)
                return 0.0;
            double denom = 1.0 - x;
            double t = lowerBoundary + x / denom;
            double y = funPtr(t) * f(t);
            denom *= denom;
            return y / denom;
        },
        0.0, 1.0);
    }

    if (rightBounded) {
        return RandMath::integral([this, funPtr, upperBoundary] (double x)
        {
            if (x <= 0.0)
                return 0.0;
            double t = upperBoundary - (1.0 - x) / x;
            double y = funPtr(t) * f(t);
            return y / (x * x);
        },
        0.0, 1.0);
    }

    /// Infinite case
    return RandMath::integral([this, funPtr] (double x)
    {
        if (std::fabs(x) >= 1.0)
            return 0.0;
        double x2 = x * x;
        double denom = 1.0 - x2;
        double t = x / denom;
        double y = funPtr(t) * f(t);
        denom *= denom;
        return y * (1.0 + x2) / denom;
    },
    -1.0, 1.0);
}


double ContinuousDistribution::ExpectedValue(const std::function<double (double)> &funPtr, double startPoint) const
{
    // TODO: Get rid of this function
    static constexpr double epsilon = 1e-10;

    double lowerBoundary = startPoint, upperBoundary = startPoint;
    SUPPORT_TYPE suppType = SupportType();
    if (suppType == FINITE_T)
    {
        lowerBoundary = GetLeftLimit(MinValue(), epsilon);
        upperBoundary = GetRightLimit(MaxValue(), epsilon);

        return RandMath::integral([this, funPtr] (double x)
        {
            return funPtr(x) * f(x);
        },
        lowerBoundary, upperBoundary);
    }
    else if (suppType == RIGHTSEMIFINITE_T) {
        lowerBoundary = GetLeftLimit(MinValue(), epsilon);
        return RandMath::integral([this, funPtr, lowerBoundary] (double x)
        {
            if (x >= 1.0)
                return 0.0;
            double denom = 1.0 - x;
            double t = lowerBoundary + x / denom;
            double y = funPtr(t) * f(t);
            denom *= denom;
            return y / denom;
        },
        0.0, 1.0);
    } else if (suppType == LEFTSEMIFINITE_T) {
        upperBoundary = GetRightLimit(MaxValue(), epsilon);
        return RandMath::integral([this, funPtr, upperBoundary] (double x)
        {
            if (x <= 0.0)
                return 0.0;
            double t = upperBoundary - (1.0 - x) / x;
            double y = funPtr(t) * f(t);
            return y / (x * x);
        },
        0.0, 1.0);
    }
    /// Infinite case
    return RandMath::integral([this, funPtr] (double x)
    {
        if (std::fabs(x) >= 1.0)
            return 0.0;
        double x2 = x * x;
        double denom = 1.0 - x2;
        double t = x / denom;
        double y = funPtr(t) * f(t);
        denom *= denom;
        return y * (1.0 + x2) / denom;
    },
    -1.0, 1.0);
}

double ContinuousDistribution::Likelihood(const std::vector<double> &sample) const
{
    double res = 1.0;
    for (const double & var : sample )
        res *= f(var);
    return res;
}

double ContinuousDistribution::LogLikelihood(const std::vector<double> &sample) const
{
    double res = 0.0;
    for (const double & var : sample )
        res += std::log(f(var));
    return res;
}
