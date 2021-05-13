#include "calculation.h"

#include <vector>
#include <algorithm>
#include <QDebug>

using std::cos;
using std::sin;
using std::sqrt;
using std::abs;

Calculation::StokesVector::StokesVector(double j, double q, double u, double v)
{
    J = j;
    Q = q;
    U = u;
    V = v;
}

double Calculation::StokesVector::p()
{
    if (abs(J) < 1e-8){
        return 0.0;
    }
    else {
        double result = sqrt(Q*Q + U*U + V*V) / J;
        if (result > 1){
            result = 1;
        }
        return result;
    }
}

void Calculation::StokesVector::swap(int k, int n, double A[4][5])
{
    double z = A[k][k];
    int i = k;

    for (int j = k+1; j < n; j++){
        if (abs(A[j][k]) > z){
            z = A[j][k];
            i = j;
        }
    }

    if (i > k){
        for (int j = k; j < n; j++){
            z = A[i][j];
            A[i][j] = A[k][j];
            A[k][j] = z;
        }
    }
}

void printMatrix(double m[4][5]){
    QDebug deb = qDebug().noquote();

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            deb << QString("%1").arg(m[i][j], 7, 'f', 4);
        }
        deb << '\n';
    }
    deb << "\n\n";
}

void Calculation::StokesVector::calculate(Intensity I1, Intensity I2, Intensity I3, Intensity I4)
{
    double A[4][5];
    double X[4] = {0, 0, 0, 0};

    J = 0.0;
    Q = 0.0;
    U = 0.0;
    V = 0.0;

    A[0][0] = 1;
    A[1][0] = 1;
    A[2][0] = 1;
    A[3][0] = 1;

    A[0][1] = cos(2 * I1.phi);
    A[1][1] = cos(2 * I2.phi);
    A[2][1] = cos(2 * I3.phi);
    A[3][1] = cos(2 * I4.phi);

    A[0][2] = sin(2 * I1.phi) * cos(I1.tau);
    A[1][2] = sin(2 * I2.phi) * cos(I2.tau);
    A[2][2] = sin(2 * I3.phi) * cos(I3.tau);
    A[3][2] = sin(2 * I4.phi) * cos(I4.tau);

    A[0][3] = sin(2 * I1.phi) * sin(I1.tau);
    A[1][3] = sin(2 * I2.phi) * sin(I2.tau);
    A[2][3] = sin(2 * I3.phi) * sin(I3.tau);
    A[3][3] = sin(2 * I4.phi) * sin(I4.tau);

    A[0][4] = 2 * I1.i;
    A[1][4] = 2 * I2.i;
    A[2][4] = 2 * I3.i;
    A[3][4] = 2 * I4.i;

    qDebug() << "init matrix";
    printMatrix(A);

    for (int k = 0; k < 4; k++){
        if (abs(A[k][k]) < 1e-10){
            std::vector<double> list;
            list.reserve(4);
            for (int q = 0; q < 4; q++){
                list.push_back(abs(A[q][k]));
            }

            auto m = std::max_element(list.begin(), list.end());
            auto idx = std::distance(list.begin(), m);

            if (idx != k){
                qDebug() << "swap";
                swap(k, idx, A);
            }
            else {
                qDebug() << "There can be error in:  idx != k\n"
                       << "idx =" << idx << " k=" << k << "\n";
                return;
            }
        }

        for (int j = k+1; j < 4; j++){
            double r = A[j][k] / A[k][k];

            for (int i = 0; i < 5; i++){
                A[j][i] -= r * A[k][i];
            }
        }
        printMatrix(A);
    }  // end for (int k = 0; k < 4; k++)

    for (int k = 3; k > -1; k--){
        double r = 0;
        for (int j = k+1; j < 4; j++){
            double g = A[k][j] * X[j];
            r += g;
        }

        if (A[k][k] == 0){
            qDebug() << "There can be error in : A[k][k] == 0\n"
                   << "A[k][k]=" << A[k][k] << "\n";
            return;
        }

        X[k] = (A[k][4] - r) / A[k][k];
    }

    J = X[0];
    Q = X[1];
    U = X[2];
    V = X[3];
}


std::complex<double> Calculation::squareRoot(std::complex<double> v, int index)
{
    double a = sqrt(sqrt(v.real()*v.real() + v.imag()*v.imag()));
    double phi = std::atan(v.imag() / v.real());

    if (v.real() < 0){
        phi += PI;
    }

    if ( (index < 0) || (index > 1) ){
        index = 0;
    }

    if (index == 1){
        phi += PI;
    }

    return std::complex<double> {a * cos(phi), a * sin(phi)};
}


Calculation::NaturalStokesVector::NaturalStokesVector(double j, double q, double u, double v)
    : StokesVector(j, q, u, v)
{
}

void Calculation::NaturalStokesVector::calculateNatural(Calculation::StokesVector &vector,
                                                        std::complex<double> nju, double phi)
{
    double cphi = cos(phi / 180.0), sphi = sin(phi / 180.0);
    std::complex<double> nju2 = nju * nju,
                         cosphi(cphi, 0.0),
                         sinphi(sphi, 0.0),
                         sinphi2 = sinphi * sinphi,
                         sn = nju2 - sinphi2,
                         sq = Calculation::squareRoot(sn, 0);

    if (sq.imag() > 0){
        sq = Calculation::squareRoot(sn, 1);
    }

    std::complex<double> ch1 = cosphi - sq,
                         zn1 = cosphi + sq,
                         nju2cosphi = nju2 * cosphi,
                         ch2 = nju2cosphi - sq,
                         zn2 = nju2cosphi + sq,
                         r1 = ch1 / zn1,
                         r2 = ch2 / zn2,
                         r1r2_ = r1 * std::conj(r2);

    double r1_2 = abs(r1) * abs(r1),
           r2_2 = abs(r2) * abs(r2),
           r1r2_2 = abs(r1r2_) * abs(r1r2_);

    J = (vector.J * (r1_2 + r2_2) - vector.Q * (r1_2 - r2_2)) / (2 * r1_2 * r2_2);
    Q = (vector.Q * (r1_2 + r2_2) - vector.J * (r1_2 - r2_2)) / (2 * r1_2 * r2_2);
    U = (vector.U * r1r2_.real() + vector.V * r1r2_.imag()) / r1r2_2;
    U = (vector.V * r1r2_.real() - vector.U * r1r2_.imag()) / r1r2_2;
}
