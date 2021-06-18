#include "calculation.h"

#include "logger.h"

using std::sin;
using std::cos;
using std::sqrt;
using std::abs;


Calculation::Gradient::Gradient(double gamma, double w, double v)
{
    Gamma = gamma;
    W = w;
    V = v;
}


double Calculation::Gradient::Fxy(double x, double y)
{
    double a = cos(2*x) * cos(2*y) - cos(Gamma) * sin(2*x) * sin(2*y) - W;
    double b = sin(2*x) * cos(2*y) + cos(Gamma) * cos(2*x) * sin(2*y) - V;
    return a * a + b * b;
}


double Calculation::Gradient::dFdx(double x, double y)
{
    double a = 4 * cos(2*x) * cos(Gamma) * sin(2*y) * W;
    double b = 4 * cos(Gamma) * sin(2*x) * sin(2*y) * V;
    double c = -4 * cos(2*x) * cos(2*y) * V;
    double d = 4 * cos(2*y) * sin(2*x) * W;
    return a + b + c + d;
}


double Calculation::Gradient::dFdy(double x, double y)
{
    double a = -4 * cos(2*x) * cos(2*y) * cos(Gamma) * V;
    double b = 4 * cos(2*y) * cos(Gamma) * cos(Gamma) * sin(2*y);
    double c = 4 * cos(2*y) * cos(Gamma) * sin(2*x) * W;
    double d = 4 * cos(2*x) * sin(2*y) * W;
    double e = 4 * sin(2*x) * sin(2*y) * V;
    double f = -4 * cos(2*y) * sin(2*y);
    return a + b + c + d + e + f;
}


std::tuple<double, double, bool> Calculation::Gradient::gradientMethod(double x0, double y0, double eps)
{
    Logger logger("rays.log");
    logger.logInfo("Вычисление градиента");

    double t = PI / 8;
    double x = x0, y = y0;
    int i = 0, n = 1000;

    logger.logDebug(QString("t : %1").arg(t, 0, 'f', 3));
    logger.logDebug(QString("x : %1").arg(x, 0, 'f', 3));
    logger.logDebug(QString("y : %1").arg(y, 0, 'f', 3));
    logger.logDebug(QString("i : %1").arg(i));
    logger.logDebug(QString("n : %1").arg(n));

    while (1){
        logger.logDebug(QString("Итерация %1").arg(i));

        if (i > n){
            x = x0;
            y = y0;

            logger.logDebug("Вычисление неуспешно");
            logger.logDebug(QString("x : %1").arg(x, 0, 'f', 3));
            logger.logDebug(QString("y : %1").arg(y, 0, 'f', 3));

            return std::make_tuple(x, y, false);
        }

        double xt = x - t * dFdx(x, y);
        double yt = y - t * dFdy(x, y);

        logger.logDebug(QString("xt : %1").arg(xt, 0, 'f', 3));
        logger.logDebug(QString("yt : %1").arg(yt, 0, 'f', 3));

        if ( abs(sqrt((x-xt) * (x-xt)) + sqrt((y-yt) * (y-yt))) < eps ){
            x = xt;
            y = yt;

            logger.logDebug("Вычисление успешно");
            logger.logDebug(QString("x : %1").arg(x, 0, 'f', 3));
            logger.logDebug(QString("y : %1").arg(y, 0, 'f', 3));

            return std::make_tuple(x, y, true);
        }

        if (Fxy(x ,y) < Fxy(xt, yt)){
            t /= 2;
        }
        else {
            x = xt;
            y = yt;
        }

        logger.logDebug(QString("x : %1").arg(x, 0, 'f', 3));
        logger.logDebug(QString("y : %1").arg(y, 0, 'f', 3));

        i++;
    }
}

