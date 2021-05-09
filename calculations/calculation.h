#ifndef CALCULATION_H
#define CALCULATION_H

#include <functional>
#include <cmath>
#include <complex>

namespace Calculation
{
    const double PI = 3.141592;

    class Gradient
    {
    public:
        double Gamma,
               W,
               V;

    public:
        Gradient(double gamma=PI/4, double w=0.77, double v=-0.3);

        double Fxy(double x, double y);
        double dFdx(double x, double y);
        double dFdy(double x, double y);
        std::tuple<double, double, bool> gradientMethod(double x0, double y0, double eps);
    };


    typedef struct
    {
        double i,
               tau,
               phi;
    } Intensity;


    class StokesVector
    {
    public:
        double J,
               Q,
               U,
               V;

    public:
        StokesVector(double j=0, double q=0, double u=0, double v=0);

        double p();
        void calculate(Intensity I1, Intensity I2, Intensity I3, Intensity I4);
        std::complex<double> squareRoot(std::complex<double> v, int index);

    private:
        void swap(int k, int n, double A[4][5]);
    };


    class NaturalStokesVector : public StokesVector
    {
    public:
        NaturalStokesVector(double j=0, double q=0, double u=0, double v=0);
        void calculateNatural(StokesVector &vector, std::complex<double> nju, double phi);
    };


    double simpson(std::function<double(double)> f, double a, double b, double eps);
    double func(double t);
    double laplace(double x);
    std::complex<double> squareRoot(std::complex<double> v, int index);
}

#endif // CALCULATION_H
