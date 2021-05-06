#include "task3.h"

#include <cmath>

#include "calculation.h"

using std::sqrt;
using std::min;
using std::max;
using std::abs;
using std::sin;
using std::cos;


double Task3::len2rect(double re, double im, double re1, double re2, double im1, double im2)
{
    double x1 = min(re1, re2);
    double x2 = max(re1, re2);
    double y1 = min(im1, im2);
    double y2 = max(im1, im2);

    /* 1 | 2 | 3
     * 4 | - | 5
     * 6 | 7 | 8
    */

    if ( (re >= x1) && (re <= x2) && (im >= y1) && (im <= y2) ){
        return 0;
    }
    else if (re < x1){
        if (im > y2)
            return sqrt( (re-x1)*(re-x1) + (im-y2)*(im-y2) );
        else if (im < y2)
            return sqrt( (re-x1)*(re-x1) + (im-y1)*(im-y1) );
        else
            return abs(re - x1);
    }
    else if (re > x2){
        if (im > y2)
            return sqrt( (re-x2)*(re-x2) + (im-y2)*(im-y2) );
        else if (im < y1)
            return sqrt( (re-x2)*(re-x2) + (im-y1)*(im-y1) );
        else
            return abs(re - x2);
    }
    else {
        return min( abs(im - y1), abs(im - y2) );
    }
}


double Task3::chi2P(double chi, int df)
{
    /** Return prob(chisq >= chi, with df degrees of freedom).
     * df must be even.
     */

    // If chi is very large, exp(-m) will underflow to 0.
    double m = chi / 2.0;
    double sum = term = std::exp(-m);

    for (int i = 1; i < df / 2; i++){
        term *= m / i;
        sum += term;
    }
    // With small chi and large df, accumulated
    // roundoff error, plus error in
    // the platform exp(), can cause this to spill
    // a few ULP above 1.0. For
    // example, chi2P(100, 300) on my box
    // has sum == 1.0 + 2.0**-52 at this
    // point.  Returning a value even a teensy
    // bit over 1.0 is no good.
    return min(sum, 1.0);
}


Task3::ItemC1::ItemC1(double dtetta, double q, double u, double v)
{
    dTetta = dtetta;
    Q = q;
    U = u;
    V = v;
}

double Task3::ItemC1::calc()
{
    if (V != 0){
        return ( Q * sin(2*dTetta*PI/180) + U * cos(2*dTetta*PI/180) ) / V;
    }
    else {
        return 0.0;  // throw something
    }
}


Task3::ItemC2::ItemC2(double dgamma, double q, double u, double v)
{
    dGamma = dgamma;
    Q = q;
    U = u;
    V = v;
}

double Task3::ItemC2::calc()
{
    if (V != 0){
        return U / V;
    }
    else {
        return 0.0;  // throw something
    }
}


Task3::ItemC3::ItemC3(double dgamma, double alfa, double beta)
{
    dGamma = dgamma;
    Alfa = alfa;
    Beta = beta;
}

double Task3::ItemC3::calc()
{
    return Alfa * Beta;
}

std::vector<Task3::Interval> Task3::getIntervals(int icount, double imin, double imax)
{
    double len = (imax - imin) / icount;
    std::vector<Task3::Interval> intervals;
    intervals.reserve(icount);
    for (int i = 0; i < icount; i++){
        Interval item;
        item.number = i;
        item.value_count = 0;
        item.low = imin + i * len;
        item.high = imin + (i + 1) * len;
        item.laplaceValueLow = Calculation::laplace(item.low);
        item.laplaceValueHigh = Calculation::laplace(item.high);
        item.pi = item.laplaceValueHigh - item.laplaceValueLow;
        intervals.push_back(item);
    }

    return intervals;
}
