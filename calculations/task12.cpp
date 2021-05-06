#include "task12.h"

using std::sin;
using std::cos;
using std::sqrt;
using std::abs;
using std::tan;


Task12::Task12(int idx, double alfa, double beta)
{
    Idx = idx;
    Alfa = alfa;
    Beta = beta;
    SV = Calculation::StokesVector();
    NSV = Calculation::NaturalStokesVector();
    for (int i = 0; i < 4; i++){
        I[i] = Calculation::Intensity(0, 0, 0);
    }
}

std::pair<Calculation::StokesVector, Calculation::NaturalStokesVector>
Task12::calcRadiation(std::complex<double> nju, std::complex<double> phi)
{
    SV.calculate(I[0], I[1], I[2], I[3]);
    NSV.calculateNatural(SV, nju, phi);
    return std::make_pair(V, N);
}

Reflection Task12::calcReflection(std::complex<double> nju, std::complex<double> phi,
                                  Calculation::Gradient &gradient, bool isanalytic)
{
    auto rad = calcRadiation(nju, phi);
    SV = rad.first;
    NSV = rad.second;

    double J = SV.J;
    double Q = SV.Q;
    double U = SV.U;
    double V = SV.V;

    if (Q*Q + V*V + U*U < 1e-5){
        gradient.W = 0;
        gradient.V = 0;
    }
    else {
        gradient.W = Q / sqrt(Q*Q + V*V + U*U);
        gradient.V = U / sqrt(Q*Q + V*V + U*U);
    }

    double x = x0 = 0;
    double y = y0 = 0;
    double x1 = x2 = 0;

    if (isanalytic){
        bool done = true;
        double cg = cos(gradient.Gamma);
        double a = Q*Q + U*U + cg*cg * (Q*Q + V*V + U*U);
        double Beta = abs(std::atan(V / a));

        if (gradient.Gamma * V > 0){
            Beta = -Beta;
        }

        double Alfa = abs(std::atan(sin(2*Beta) / cos(2*Beta) * cg));

        if (abs(U) < 1e-5){
            Alfa = 0;
        }
        else if (U < 0){
            Alfa = -Alfa;
        }
    }
    else {
        bool done;
        auto gr = gradient.gradientMethod(x0, y0, 1e-5);
        x = std::get<0>(gr);
        y = std::get<1>(gr);
        done = std::get<2>(gr);

        if (!done){
            return;
        }

        if (abs(gradient.Fxy(x, y)) > 1e-5){
            return;
        }

        double a = cos(-2*y) + gradient.W;
        double b = sqrt(2) * sin(-2*y);
        double c = gradient.W - cos(-2*y);
        double d = b * b - 4 * c * a;

        if (d < 0){
            return;
        }

        x1 = std::atan( (-b + sqrt(d)) / (2 * a) );
        x2 = std::atan( (-b - sqrt(d)) / (2 * a) );

        if (abs(x1 - x) > abs(x2 - x)){
            x2 = x1;
        }

        Alfa = (x + x2) / 2;

        if (V > 0){
            Beta = -abs(y);
        }
        else {
            Beta = abs(y);
        }
    }

    std::complex<double> a {1, -tan(Alfa) * tan(Beta)};
    auto res = std::complex<double> {tan(Alfa), tan(Beta)} /
               std::complex<double> {1, -tan(Alfa) * tan(Beta)};

    return Reflection {Alfa, x, Beta, y, res.real(), res.imag()};
}

