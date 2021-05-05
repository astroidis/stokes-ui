#include "calculation.h"


Calculation::Gradient::Gradient(double gamma, double w, double v)
{
    Gamma = gamma;
    W = w;
    V = v;
}


double Calculation::Gradient::Fxy(double x, double y)
{
    double a = std::cos(2*x) * std::cos(2*y) - std::cos(Gamma) * std::sin(2*x) * std::sin(2*y) - W;
    double b = std::sin(2*x) * std::cos(2*y) + std::cos(Gamma) * std::cos(2*x) * std::sin(2*y) - V;
    return a * a + b * b;
}


double Calculation::Gradient::dFdx(double x, double y)
{
    double a = 4 * std::cos(2*x) * std::cos(Gamma) * std::sin(2*y) * W;
    double b = 4 * std::cos(Gamma) * std::sin(2*x) * std::sin(2*y) * V;
    double c = -4 * std::cos(2*x) * std::cos(2*y) * V;
    double d = 4 * std::cos(2*y) * std::sin(2*x) * W;
    return a + b + c + d;
}


double Calculation::Gradient::dFdy(double x, double y)
{
    double a = -4 * std::cos(2*x) * std::cos(2*y) * std::cos(Gamma) * V;
    double b = 4 * std::cos(2*y) * std::cos(Gamma) * std::cos(Gamma) * std::sin(2*y);
    double c = 4 * std::cos(2*y) * std::cos(Gamma) * std::sin(2*x) * W;
    double d = 4 * std::cos(2*x) * std::sin(2*y) * W;
    double e = 4 * std::sin(2*x) * std::sin(2*y) * V;
    double f = -4 * std::cos(2*y) * std::sin(2*y);
    return a + b + c + d + e + f;
}


std::tuple<double, double, bool> Calculation::Gradient::gradientMethod(double x0, double y0, double eps)
{
    double t = PI / 8;
    double x = x0, y = y0;
    int i = 0, n = 1000;

    while (1){
        if (i > n){
            x = x0;
            y = y0;
            return std::make_tuple(x, y, false);
        }

        double xt = x - t * dFdx(x, y);
        double yt = y - t * dFdy(x, y);

        if ( std::abs(std::sqrt((x-xt) * (x-xt)) + std::sqrt((y-yt) * (y-yt))) < eps ){
            x = xt;
            y = yt;
            return std::make_tuple(x, y, true);
        }

        if (Fxy(x ,y) < Fxy(xt, yt)){
            t /= 2;
        }
        else {
            x = xt;
            y = yt;
        }

        i++;
    }
}

