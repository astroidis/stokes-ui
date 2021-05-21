#include "chisqr.h"
#include "calculation.h"

using Calculation::PI;

double factorial(double n)
{
    if (n == 0) { return 1; }
    if (n < 0) { return sqrt(PI); }
    return n * factorial(n - 1);
}

double gamma(double n)
{
    return factorial(n - 1);
}


double chi_square_distribution(double x, double v)
{
    double a = v / 2, b = x / 2;

    // count the Gamma function
    double g = gamma(a);

    // count of the probability density
    double f = pow(x, a - 1) / g / exp(b) / pow(2, a);

    // count the sum of series
    double s = 0, z = 0;
    int k = 0, w = 0;
    g = 1;
    do {
        k = k + 1;
        w = w + 2;
        z = s;
        g = g * (v + w);
        s = s + (pow(x, k) / g);
    } while (s != z);

    // count of probabilities
    double p = 2 * x * f * (1 + s) / v;
    double q = 1 - p;

    return q;
}

double _chi_square_distribution(double x, double v)
{
    double a = v / 2, b = x / 2, g;

    // count the Gamma function...
    // for even degree
    if (int(v) % 2 == 0) {
        g = 1;
        for (int i = 1; i < a; i++) {
            g = g * i;
        }
    }
    // for odd degree
    if (int(v) % 2 != 0) {
        g = sqrt(PI);
        for (double i = 0.5; i < a; i++) {
            g = g * i;
        }
    }

    // count of the probability density
    double f = pow(x, a - 1) / g / exp(b) / pow(2, a);

    // count the sum of series
    double s = 0, z = 0;
    int k = 0, w = 0;
    g = 1;
    do {
        k = k + 1;
        w = w + 2;
        z = s;
        g = g * (v + w);
        s = s + (pow(x, k) / g);
    } while (s != z);

    // count of probabilities
    double p = 2 * x * f * (1 + s) / v;
    double q = 1 - p;

    return q;
}

double search_chi_square(double x, double h, double p, double v)
{
    double z = x;
    x = x + h;
    double y = chi_square_distribution(x, v);

    if (z == x || y == p) {
        return x;
    }
    if (y < p) {
        return search_chi_square(x - h, h / 2, p, v);
    }
    if (y > p) {
        return search_chi_square(x, h, p, v);
    }

    return 0; // warning control reaches end of non-void function -wreturn-type
}

double chi_square_inverse(double p, double v)
{
    return search_chi_square(0, 0.1, p, v);
}
