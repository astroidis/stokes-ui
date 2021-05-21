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

    class ItemC
    {
    public:
        bool iscalc;
        double value;
        double value_norm;

        ItemC() = default;
        virtual double calc() {return value;}
    };


    class ItemC1 : public ItemC
    {
    public:
        double dTetta, Q, U, V;

        ItemC1(double dtetta, double q, double u, double v);
        double calc() override;
    };


    class ItemC2 : public ItemC
    {
    public:
        double dGamma, Q, U, V;

        ItemC2(double dgamma, double q, double u, double v);
        double calc() override;
    };


    class ItemC3 : public ItemC
    {
    public:
        double dGamma, Alpha, Beta;

        ItemC3(double dgamma, double alpha, double beta);
        double calc() override;
    };


    class Interval
    {
    public:
        static int INTERVAL_COUNT;
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
    double chi2P(double chi, int df);
    std::vector<Interval> getIntervals(int count, double min, double max);
    std::map<std::string, double> calcStat(std::vector<ItemC> &data);
    std::pair<int, Material>
        calcPlaneMaterial(std::vector<Sample> data, std::vector<Material> materials, PlaneTypes type);
}

#endif // TASK3_H
