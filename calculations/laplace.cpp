#include "calculation.h"


double Calculation::simpson(std::function<double (double)> f, double a, double b, double eps)
{
    int n = 1;
    double  s = 0, s0 = eps + 1;

    while (std::abs(s - s0) > eps){
        n *= 2;
        double h = (b - a) / n;
        s0 = s;
        s = f(a) + f(b);

        for (int i = 1; i < n; i++){
            s += 2 * (1 + i % 2) * func(a + i*h);
        }

        s *= (b - a) / (3 * n);
    }

    return s;
}

double Calculation::func(double t)
{
    return std::exp(-t * t/2);
}

double Calculation::laplace(double x)
{
    return simpson(func, 0, x, 1e-12) / std::sqrt(2 * 3.141592);
}
