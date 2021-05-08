#ifndef TASK12_H
#define TASK12_H

#include "calculation.h"
#include <array>
#include <utility>

typedef struct
{
    double alfa,
           alfa1,
           beta,
           beta1,
           re_hi,
           im_hi;
} Reflection;

class Task12
{
public:
    int Idx;
    double Alfa, Beta;
    Calculation::StokesVector SV;
    Calculation::NaturalStokesVector NSV;
    std::array<Calculation::Intensity, 4> I;

    Task12(int idx, double alfa, double beta);
    std::pair<Calculation::StokesVector, Calculation::NaturalStokesVector>
        calcRadiation(std::complex<double> nju, double phi);
    Reflection calcReflection(std::complex<double> nju, double phi,
                              Calculation::Gradient &gradient, bool isanalytic);
};

#endif // TASK12_H
