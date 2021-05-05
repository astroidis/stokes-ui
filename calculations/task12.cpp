#include "task12.h"

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
//    auto [V, N] = calcRadiation(nju, phi);  // ide шипит на эту строку хз почему
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
        gradient.W = Q / std::sqrt(Q*Q + V*V + U*U);
        gradient.V = U / std::sqrt(Q*Q + V*V + U*U);
    }

    double x = x0 = 0;
    double y = y0 = 0;
    double x1 = x2 = 0;

    if (isanalytic){
        bool done = true;
        double cg = std::cos(gradient.Gamma);
        double a = Q*Q + U*U + cg*cg * (Q*Q + V*V + U*U);
        double Beta = std::abs(std::atan(V / a));

        if (gradient.Gamma * V > 0){
            Beta = -Beta;
        }

        double Alfa = std::abs(std::atan(std::sin(2*Beta) / std::cos(2*Beta) * cg));

        if (std::abs(U) < 1e-5){
            Alfa = 0;
        }
        else if (U < 0){
            Alfa = -Alfa;
        }
    }
    else {
        // todo todo todo
    }

    std::complex<double> a {1, -std::tan(Alfa) * std::tan(Beta)};
    auto res = std::complex<double> {std::tan(Alfa), std::tan(Beta)} /
               std::complex<double> {1, -std::tan(Alfa) * std::tan(Beta)};

    return Reflection {Alfa, x, Beta, y, res.real(), res.imag()};
}

