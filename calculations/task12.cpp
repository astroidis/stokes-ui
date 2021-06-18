#include "task12.h"

#include "logger.h"

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
        I[i] = Calculation::Intensity{0, 0, 0};
    }
}

void Task12::loadIntensities(int idx, double i, double tau, double phi)
{
    I[idx] = Calculation::Intensity {i, tau, phi};
}

std::pair<Calculation::StokesVector, Calculation::NaturalStokesVector>
Task12::calcRadiation(std::complex<double> nju, double phi)
{
    Logger logger("rays.log");

    logger.logInfo("Рассчет рассеивания");

    SV.calculate(I[0], I[1], I[2], I[3]);
    NSV.calculateNatural(SV, nju, phi);
    return std::make_pair(SV, NSV);
}

Reflection Task12::calcReflection(std::complex<double> nju, double phi,
                                  Calculation::Gradient &gradient, bool isanalytic)
{
    Logger logger("rays.log");
    logger.logInfo("Рассчет преломления");

    auto rad = calcRadiation(nju, phi);
    SV = rad.first;
    NSV = rad.second;

    double J = SV.J;
    double Q = SV.Q;
    double U = SV.U;
    double V = SV.V;

    logger.logDebug("Вектор");
    logger.logDebug(QString("J : %1").arg(J, 0, 'g', 3));
    logger.logDebug(QString("Q : %1").arg(Q, 0, 'g', 3));
    logger.logDebug(QString("U : %1").arg(U, 0, 'g', 3));
    logger.logDebug(QString("V : %1").arg(V, 0, 'g', 3));

    if (Q*Q + V*V + U*U < 1e-5){
        gradient.W = 0;
        gradient.V = 0;
    }
    else {
        gradient.W = Q / sqrt(Q*Q + V*V + U*U);
        gradient.V = U / sqrt(Q*Q + V*V + U*U);
    }

    double x = 0, x0 = 0, y = 0, y0 = 0, x1 = 0, x2 = 0;

    double Alfa, Beta;
    if (isanalytic){
//        bool done = true;
        double cg = cos(gradient.Gamma);
        double a = Q*Q + U*U + cg*cg * (Q*Q + V*V + U*U);
        Beta = abs(std::atan(V / sqrt(a)));

        if (gradient.Gamma * V > 0){
            Beta = -Beta;
        }

        Alfa = abs(std::atan(sin(2*Beta) / cos(2*Beta) * cg));

        if (abs(U) < 1e-5){
            Alfa = 0;
        }
        else if (U < 0){
            Alfa = -Alfa;
        }
    }
    else {
        auto gr = gradient.gradientMethod(x0, y0, 1e-5);
        x = std::get<0>(gr);
        y = std::get<1>(gr);
        bool done = std::get<2>(gr);

        if (!done){
            logger.logDebug("Решение методом градиента не найдено");
            auto nan_value = std::numeric_limits<double>::quiet_NaN();
            return Reflection{nan_value, nan_value, nan_value, nan_value, nan_value, nan_value};
        }

        if (abs(gradient.Fxy(x, y)) > 1e-5){
            logger.logDebug("Система не имеет решения");
            auto nan_value = std::numeric_limits<double>::quiet_NaN();
            return Reflection{nan_value, nan_value, nan_value, nan_value, nan_value, nan_value};
        }

        double a = cos(-2*y) + gradient.W;
        double b = sqrt(2) * sin(-2*y);
        double c = gradient.W - cos(-2*y);
        double d = b * b - 4 * c * a;

        if (d < 0){
            logger.logDebug("Не найдено второе решение");
            auto nan_value = std::numeric_limits<double>::quiet_NaN();
            return Reflection{nan_value, nan_value, nan_value, nan_value, nan_value, nan_value};
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

    auto res = std::complex<double> {tan(Alfa), tan(Beta)} /
               std::complex<double> {1, -tan(Alfa) * tan(Beta)};

    logger.logDebug("Найдено решение");
    logger.logDebug(QString("Alfa : %1").arg(Alfa, 0, 'g', 3));
    logger.logDebug(QString("X : %1").arg(x, 0, 'g', 3));
    logger.logDebug(QString("Beta : %1").arg(Beta, 0, 'g', 3));
    logger.logDebug(QString("Y : %1").arg(y, 0, 'g', 3));
    logger.logDebug(QString("Re(Hi) : %1").arg(res.real(), 0, 'g', 3));
    logger.logDebug(QString("Im(hi) : %1").arg(res.imag(), 0, 'g', 3));

    return Reflection {Alfa, x, Beta, y, res.real(), res.imag()};
}

