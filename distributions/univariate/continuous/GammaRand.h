#ifndef GAMMARAND_H
#define GAMMARAND_H

#include "ContinuousDistribution.h"

/**
 * @brief The GammaDistribution class <BR>
 * Abstract class for Gamma distribution
 *
 * f(x | α, β) = β^α / Γ(α) * x^(α-1) * exp(-βx),
 * where Γ(α) denotes Gamma function
 *
 * Notation X ~ Gamma(α, β)
 *
 * Related distributions: <BR>
 * σX ~ Gamma(α, σβ) <BR>
 * If X ~ Gamma(1, β), then X ~ Exp(β) <BR>
 * If X ~ Gamma(0.5 * n, 0.5), then X ~ Chi^2(n) <BR>
 * If X ~ Gamma(k, β) for integer k, then X ~ Erlang(k, β)
 */
class RANDLIBSHARED_EXPORT GammaDistribution : public ContinuousDistribution
{
protected:
    double alpha; ///< shape α
    double beta; ///< rate β
    double theta; ///< scale θ = 1/β
    double lgammaAlpha; ///< log(Γ(α))
    double pdfCoef; ///< α * log(β) - log(Γ(α))
    double logAlpha; ///< log(α)
    double logBeta; ///< log(β)

private:
    /// constants for faster sampling
    double t, b;

public:
    GammaDistribution(double shape, double rate);
    virtual ~GammaDistribution() {}

    SUPPORT_TYPE SupportType() const override { return RIGHTSEMIFINITE_T; }
    double MinValue() const override { return 0; }
    double MaxValue() const override { return INFINITY; }

protected:
    /**
     * @fn SetParameters
     * @param shape α
     * @param rate β
     */
    void SetParameters(double shape, double rate);

public:
    /**
     * @fn GetShape
     * @return α
     */
    inline double GetShape() const { return alpha; }
    /**
     * @fn GetScale
     * @return θ = 1/β
     */
    inline double GetScale() const { return theta; }
    /**
     * @fn GetRate
     * @return β
     */
    inline double GetRate() const { return beta; }
    /**
     * @fn GetLogGammaShape
     * @return log(Γ(α))
     */
    inline double GetLogGammaShape() const { return lgammaAlpha; }
    /**
     * @fn GetLogShape
     * @return log(α)
     */
    inline double GetLogShape() const { return logAlpha; }
    /**
     * @fn GetLogRate
     * @return log(β)
     */
    inline double GetLogRate() const { return logBeta; }

    double f(const double & x) const override;
    double logf(const double & x) const override;
    double F(const double & x) const override;
    double logF(const double &x) const;
    double S(const double & x) const override;
    double logS(const double & x) const;
    
private:
    enum GENERATOR_ID {
        /// Erlang distribution for α = 1, 2, 3
        INTEGER_SHAPE,
        /// α = 1.5
        ONE_AND_A_HALF_SHAPE,
        /// α < 0.34
        SMALL_SHAPE,
        /// 1 < α < 1.2
        FISHMAN,
        /// 0.34 < α < 1 or α >= 1.2
        MARSAGLIA_TSANG
    };
    /**
     * @fn getIdOfUsedGenerator
     * @param shape α
     * @return id of used variate generator according to the shape
     */
    static GENERATOR_ID getIdOfUsedGenerator(double shape);
    /**
     * @fn variateThroughExponentialSum
     * @param shape α
     * @return gamma variate, generated by sum of exponentially distributed random variables
     */
    static double variateThroughExponentialSum(int shape);
    /**
     * @fn variateForShapeOneAndAHalf
     * @return gamma variate for α = 1.5
     */
    static double variateForShapeOneAndAHalf();
    /**
     * @fn variateBest
     * @return gamma variate for small α, using Best algorithm
     */
    double variateBest() const;
    /**
     * @fn variateAhrensDieter
     * @param shape α
     * @return gamma variate for small α, using Ahrens-Dieter algorithm (if we have no pre-calculated values)
     */
    static double variateAhrensDieter(double shape);
    /**
     * @fn variateFishman
     * @param shape α
     * @return gamma variate, using Fishman algorithm
     */
    static double variateFishman(double shape);
    /**
     * @fn variateMarsagliaTsang
     * @param shape α
     * @return gamma variate, using Marsaglia-Tsang algorithm
     */
    static double variateMarsagliaTsang(double shape);
    
public:
    /**
     * @fn StandardVariate
     * @param shape α
     * @return gamma variate with shape α and unity rate
     */
    static double StandardVariate(double shape);
    /**
     * @fn Variate
     * @param shape α
     * @param rate β
     * @return gamma variate with shape α and rate β
     */
    static double Variate(double shape, double rate);

    double Variate() const override;
    void Sample(std::vector<double> &outputData) const override;

    /**
     * @fn Mean
     * @return E[X]
     */
    double Mean() const override;
    /**
     * @fn GeometricMean
     * @return E[ln(X)]
     */
    double GeometricMean() const;
    /**
     * @fn Variance
     * @return Var(X)
     */
    double Variance() const override;
    /**
     * @fn GeometricVariance
     * @return Var(ln(X))
     */
    double GeometricVariance() const;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

private:
    /// quantile auxiliary functions
    double initRootForSmallP(double r) const;
    double initRootForLargeP(double logQ) const;
    double initRootForSmallShape(double p) const;
    double initRootForLargeShape(double p) const;
    double quantileInitialGuess(double p) const;
    double quantileInitialGuess1m(double p) const;

    /**
     * @fn df
     * derivative of probability density function
     * @param x
     * @return f'(x)
     */
    double df(double x) const;
    double quantileImpl(double p) const override;
    double quantileImpl1m(double p) const override;

    std::complex<double> CFImpl(double t) const override;
};

class RANDLIBSHARED_EXPORT GammaRand;

/**
 * @brief The FreeScaleGammaDistribution class <BR>
 * Abstract class for Gamma distribution with arbitrary scale/rate
 */
class RANDLIBSHARED_EXPORT FreeScaleGammaDistribution : public GammaDistribution
{
public:
    FreeScaleGammaDistribution(double shape, double rate) : GammaDistribution(shape, rate) {}
    /**
     * @fn SetRate
     * set rate β
     * @param rate
     */
    void SetRate(double rate);
    /**
     * @fn SetScale
     * set scale θ = 1/β
     * @param scale
     */
    void SetScale(double scale);
    /**
     * @fn FitRateUMVU
     * set rate, returned by uniformly minimum variance unbiased estimator
     * @param sample
     */
    void FitRateUMVU(const std::vector<double> &sample);
    /**
     * @fn FitRateMLE
     * set rate, estimated via maximum-likelihood method
     * @param sample
     */
    void FitRateMLE(const std::vector<double> &sample);
    /**
     * @fn FitRateBayes
     * set rate, returned by bayesian estimation
     * @param sample
     * @param priorDistribution
     * @return posterior distribution
     */
    GammaRand FitRateBayes(const std::vector<double> &sample, const GammaDistribution &priorDistribution);
};

/**
 * @brief The GammaRand class <BR>
 * Gamma distribution
 */
class RANDLIBSHARED_EXPORT GammaRand : public FreeScaleGammaDistribution
{
public:
    GammaRand(double shape = 1, double rate = 1) : FreeScaleGammaDistribution(shape, rate) {}
    std::string Name() const override;

    using GammaDistribution::SetParameters;
    /**
     * @fn FitShapeMM
     * set shape, estimated via method of moments
     * @param sample
     */
    void FitShapeMM(const std::vector<double> &sample);
    /**
     * @fn FitShapeAndRateMM
     * set shape and rate, estimated via method of moments
     * @param sample
     */
    void FitShapeAndRateMM(const std::vector<double> &sample);
    /**
     * @fn FitShapeMLE
     * set shape, estimated via maximum-likelihood method
     * @param sample
     */
    void FitShapeMLE(const std::vector<double> &sample);
    /**
     * @fn FitShapeAndRateMLE
     * set shape and rate, estimated via maximum-likelihood method
     * @param sample
     */
    void FitShapeAndRateMLE(const std::vector<double> &sample);
};


/**
 * @brief The ChiSquaredRand class <BR>
 * Chi-squared distribution
 *
 * Notation: X ~ Chi^2(n)
 *
 * Related distributions:
 * X ~ Gamma(0.5 * n, 0.5)
 */
class RANDLIBSHARED_EXPORT ChiSquaredRand : public GammaDistribution
{
public:
    explicit ChiSquaredRand(int degree) : GammaDistribution((degree < 1) ? 0.5 : 0.5 * degree, 0.5) {}
    std::string Name() const override;
    void SetDegree(int degree);
    inline int GetDegree() const { return static_cast<int>(2 * alpha); }
};


/**
 * @brief The ErlangRand class <BR>
 * Erlang distibution
 *
 * Notation: X ~ Erlang(k, β)
 *
 * Related distributions:
 * X ~ Y_1 + Y_2 + ... + Y_k, where Y_i ~ Exp(β)
 * X ~ Gamma(k, β)
 */
class RANDLIBSHARED_EXPORT ErlangRand : public FreeScaleGammaDistribution
{
public:
    ErlangRand(int shape, double rate) : FreeScaleGammaDistribution(shape, rate) {}
    std::string Name() const override;
    void SetParameters(int shape, double rate);
};


#endif // GAMMARAND_H
