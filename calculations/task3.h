#ifndef TASK3_H
#define TASK3_H

#include <vector>
#include <map>
#include <string>

namespace Task3
{
    //const double PI = 3.141592;

    enum class PlaneTypes
    {
        IS_DIEL,
        IS_METAL
    };

    struct ItemC
    {
        bool iscalc;
        double value;
        double value_norm;
    };


    class ItemC1
    {
    public:
        double dTetta, Q, U, V;

        ItemC1(double dtetta, double q, double u, double v);
        double calc();
    };


    class ItemC2
    {
    public:
        double dGamma, Q, U, V;

        ItemC2(double dgamma, double q, double u, double v);
        double calc();
    };


    class ItemC3
    {
    public:
        double dGamma, Afla, Beta;
        bool iscalc;

        ItemC3(double dgamma, double alfa, double beta);
        double calc();
    };


    class Interval
    {
    public:
        static int INTERVAL_COUNT = 5;
        int number, value_count;
        double low, high, laplaceValueLow, laplaceValueHigh, pi, npi, ni_npi,
               ni_npi_norm;
    };


    struct Sample
    {
        double tetta, lambda, n;
    };


    struct Param
    {
        double tetta1, a1, v1, u1, tetta2, a2, v2, u2;
    };


    struct Material
    {
        std::string name, label;
        double re_min, re_max, im_min, im_max, length;
    };


    inline double det2(double a11, double a21, double a12, double a22)
    {
        return a11 * a22 - a21 * a12;
    }

    double len2rect(double re, double im, double re1, double re2, double im1, double im2);
    double chi2P(double chi, double df);
    std::vector<Interval> getIntervals(int count, double min, double max);
    void calcStat(void_t data, std::map<std::string, double> &stat);
    std::pair<int, Material> calcPlaneMaterial(void_t data, std::vector<Material> materials, PlaneTypes type);
}

#endif // TASK3_H
