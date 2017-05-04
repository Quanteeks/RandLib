#ifndef NEGATIVEBINOMIALRAND_H
#define NEGATIVEBINOMIALRAND_H

#include "DiscreteDistribution.h"
#include "PoissonRand.h"
#include "../continuous/BetaRand.h"

/**
 * @brief The NegativeBinomialDistribution class <BR>
 * Abstract class for Negative binomial distribution
 *
 * P(X = k) = C(k + r - 1, k) p^r (1-p)^k
 *
 * Notation: X ~ NB(r, p)
 *
 * Related distributions:
 * If X ~ NB(1, p), then X ~ Geometric(p)
 */
template < typename T >
class RANDLIBSHARED_EXPORT NegativeBinomialDistribution : public DiscreteDistribution
{
protected:
    double p, q;
    double logProb; ///< log(p)
    double log1mProb; ///< log(q)

private:
    T r;
    double pdfCoef;
    double qDivP; /// q / p
    static constexpr int tableSize = 16;
    double table[tableSize];
    GammaRand GammaRV;

public:
    NegativeBinomialDistribution(T number, double probability);
    SUPPORT_TYPE SupportType() const override { return RIGHTSEMIFINITE_T; }
    int MinValue() const override { return 0; }
    int MaxValue() const override { return INT_MAX; }

private:
    void SetValidParameters(T number, double probability);

protected:
    void SetParameters(T number, double probability);

public:
    inline double GetProbability() const { return p; }
    inline T GetNumber() const { return r; }

    double P(const int & k) const override;
    double logP(const int & k) const override;
    double F(const int & k) const override;
    double S(const int & k) const override;

protected:
    enum GENERATOR_ID {
        TABLE,
        EXPONENTIAL,
        GAMMA_POISSON
    };

    GENERATOR_ID GetIdOfUsedGenerator() const;

    int variateGeometricByTable() const;
    int variateGeometricThroughExponential() const;
private:
    int variateByTable() const;
    int variateThroughExponential() const;
    int variateThroughGammaPoisson() const;

public:
    int Variate() const override;
    void Sample(std::vector<int> &outputData) const override;

    double Mean() const override;
    double Variance() const override;
    int Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

private:
    std::complex<double> CFImpl(double t) const override;
public:
    /**
     * @fn FitProbabilityMM
     * set probability, estimated via method of moments
     * @param sample
     */
    void FitProbabilityMM(const std::vector<int> & sample);

    /**
     * @fn FitProbabilityBayes
     * @param sample
     * @param priorDistribution
     * @return posterior distribution
     */
    BetaRand FitProbabilityBayes(const std::vector<int> &sample, const BetaDistribution &priorDistribution);
};


/**
 * @brief The NegativeBinomialRand class <BR>
 * Negative binomial distribution
 */
template < typename T >
class RANDLIBSHARED_EXPORT NegativeBinomialRand : public NegativeBinomialDistribution<T>
{
public:
    NegativeBinomialRand(T number, double probability) : NegativeBinomialDistribution<T>(number, probability) {}
    std::string Name() const override;

    using NegativeBinomialDistribution<T>::SetParameters;

    /// Parameters estimation
    static constexpr char TOO_SMALL_VARIANCE[] = "Sample variance should be bigger than sample mean";
    /**
     * @fn FitNumberMM
     * set number, estimated via method of moments
     * @param sample
     */
    void FitNumberMM(const std::vector<int> &sample);
    /**
     * @fn FitNumberAndProbabilityMM
     * set number and probability, estimated via method of moments
     * @param sample
     */
    void FitNumberAndProbabilityMM(const std::vector<int> & sample);

    /**
     * @fn FitNumberAndProbabilityMLE
     * set number and probability, estimated via maximum-likelihood method
     * @param sample
     */
    void FitNumberAndProbabilityMLE(const std::vector<int> &sample);
};


typedef NegativeBinomialRand<int> PascalRand;
typedef NegativeBinomialRand<double> PolyaRand;

#endif // NEGATIVEBINOMIALRAND_H
