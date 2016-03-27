# RandLib
Stochastic calculus

With RandLib one can work with probability distributions.
* Fast sampling. For instance, generate ten thousand variates from standard normal distribution:
```c++
NormalRand distribution(0, 1);
std::vector<double> data(1e6);
distribution.sample(data);
```
![alt tag](https://github.com/StochasticEngineer/RandLib/blob/master/images/standardNormal.png)

* Calculate moments and other properties:
```c++
LogNormalRand distribution(1, 1);
std::cout << " Mean = " << distribution.Mean()
          << " and Variance = " << distribution.Variance()
          << "\n Median = " << distribution.Median()
          << " and Mode = " << distribution.Mode()
          << "\n Skewness = " << distribution.Skewness()
          << " and Ex. kurtosis = " << distribution.ExcessKurtosis();
```
![alt tag](https://github.com/StochasticEngineer/RandLib/blob/master/images/lognormal11.png)
```
Mean = 4.48169 and Variance = 34.5126
Median = 2.71828 and Mode = 1
Skewness = 6.18488 and Ex. Kurtosis = 110.936
```

* Calculate probabilities for discrete distributions and probability density functions for continuous:
```c++
GeometricRand X(4);
std::cout << "Probability to get 5 for Geometric(4) is " << X.P(5);
ExponentialRand Y(4);
std::cout << "Density function at point 5 for Exponential(4) is " << Y.f(5);
```
* Get cumulative density function for random variables with sophisticated distribution:
```c++
BetaRand distribution(6, 7);
int size = 100;
std::vector<double> x(size), y(size);
double sizem1Inv = 1.0 / (size - 1);
for (int i = 0; i != size; ++i)
    x[i] = i * sizem1Inv;
distribution.cdf(x, y);
for (int i = 0; i != size; ++i)
    std::cout << "P(X < " << x[i] << ") = " << y[i];
```
* Find values of characteristic function:
```c++
StableRand X(1.5, 0.5, 1.0, 0.0);
GeometricStableRand Y(1.5, 0.5, 1.0, 0.0);
std::cout << "Stable CF at point 1 - " << X.CF(1.0);
std::cout << "Geometric Stable CF at point 1 - " << Y.CF(1.0);
```
* Fitting parameters:
```c++
NormalRand X(0, 1);
std::vector<double> data(10);
X.sample(data);
std::cout << "True distribution: " << X.name() << "\n";
std::cout << "Sample: ";
for (double var : data)
    std::cout << var << "  ";
std::cout << "\n";

/// Bayesian
NormalInverseGammaRand prior(0, 1, 1, 1);
X.fitMeanAndVarianceBayes(data, prior);
std::cout << "Bayesian estimator: " << X.name() << "\n";
std::cout << "(Posterior distribution: " << prior.name() << ")\n";

/// UMVU
rv.fitMeanAndVarianceUMVU(data);
std::cout << "UMVU estimator: " << X.name() << "\n";

/// Maximum-likelihood
rv.fitMeanAndVarianceMLE(data);
std::cout << "Maximum-likelihood estimator: " << X.name() << "\n";
```
![alt tag](https://github.com/StochasticEngineer/RandLib/blob/master/images/normalFit.png)
```
True distribution: Normal(0, 1)
Sample: -0.328154  0.709122  -0.607214  1.11472  -1.23726  -0.123584  0.59374  -1.20573  -0.397376  -1.63173
Bayesian estimator: Normal(-0.283042, 0.951348)
(Posterior distribution: Normal-Inverse-Gamma(-0.283042, 11, 6, 4.75674))
UMVU estimator: Normal(-0.311347, 0.82504)
Maximum-likelihood estimator: Normal(-0.311347, 0.742536)
```

List of implemented distributions:

Continuous:

|    Title     |     F(x)     |     f(x)     |   variate    |   CF(t)    |
| ------------ | ------------ | ------------ | ------------ | ------------ |
|    Arcsine   | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Beta     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Beta Prime     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Cauchy     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Chi-squared     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Erlang     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Exponential     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Exponential-Normal (EMG)     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     F    | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Frechet    | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Gamma     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Geometric Stable     | :x: | :x: | :white_check_mark: |:white_check_mark:|
|     Gumbel     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Irwin-Hall     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Laplace     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Levy     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Log-normal     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Logistic     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Maxwell-Boltzmann     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Nakagami     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Normal     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Pareto     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Planck     | :x: | :white_check_mark: | :white_check_mark: |:x:|
|     Raised cosine     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Rayleigh     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Sech (Hyperbolic secant)    | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Stable     | :warning: | :warning: | :white_check_mark: |:white_check_mark:|
|     Student's t     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Triangular     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Uniform     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     von Mises     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:warning:|
|     Wald (Inverse Gaussian)     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Weibull     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Wigner Semicircle     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|

Discrete:

|    Title     |     F(x)     |     P(X = x)     |   variate    |   CF(t)    |
| ------------ | ------------ | ------------ | ------------ | ------------ |
|     Bernoulli     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Beta-Binomial     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Binomial     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Geometric    | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Hypergeometric     | :white_check_mark: | :white_check_mark: | :warning: |:x:|
|     Logarithmic     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Negative binomial     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Poisson     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Rademacher     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Skellam    | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Uniform     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:white_check_mark:|
|     Yule     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Zeta     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|
|     Zipf     | :white_check_mark: | :white_check_mark: | :white_check_mark: |:x:|

Singular:

|    Title     |     F(x)     |  variate    |   CF(t)    |
| ------------ | ------------ | ------------ | ------------ |
|     Cantor     | :white_check_mark: | :white_check_mark: | :white_check_mark: |
