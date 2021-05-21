#ifndef CHISQR_H
#define CHISQR_H

double factorial(double n);
double gamma(double n);
double chi_square_distribution(double x, double v);
double _chi_square_distribution(double x, double v);
double search_chi_square(double x, double h, double p, double v);
double chi_square_inverse(double p, double v);

#endif // CHISQR_H
