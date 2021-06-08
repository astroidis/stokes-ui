#include "task3.h"

#include <cmath>
#include <algorithm>
#include <QDebug>

#include "calculation.h"
#include "chisqr.h"
#include "logger.h"

using std::sqrt;
using std::min;
using std::max;
using std::abs;
using std::sin;
using std::cos;
using std::vector;
using Calculation::PI;


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
    double term = std::exp(-m), sum = term;

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
        iscalc = true;
        return ( Q * sin(2*dTetta*PI/180) + U * cos(2*dTetta*PI/180) ) / V;
    }
    else {
        iscalc = false;
        qDebug() << "ItemC1::calc() : V == 0\n";
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
        iscalc = true;
        return U / V;
    }
    else {
        qDebug() << "ItemC2::calc() : V == 0\n";
        iscalc = false;
        return 0.0;  // throw something
    }
}


Task3::ItemC3::ItemC3(double dgamma, double alpha, double beta)
{
    dGamma = dgamma;
    Alpha = alpha;
    Beta = beta;
}

double Task3::ItemC3::calc()
{
    iscalc = true;
    return Alpha * Beta;
}

vector<Task3::Interval> Task3::getIntervals(int icount, double imin, double imax)
{
    double len = (imax - imin) / icount;
    vector<Task3::Interval> intervals;
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


double mean(vector<Task3::ItemC> data)
{
    double sum = 0;
    for (const auto &val : data){
        sum += val.value;
    }
    return sum / data.size();
}

double stdev(vector<Task3::ItemC> data)
{
    double m = mean(data);
    double s = 0;
    for (const auto &x : data){
        s += (x.value - m) * (x.value - m);
    }

    return sqrt(s / (data.size() - 1));
}

std::map<std::string, double> Task3::calcStat(std::vector<ItemC> &data)
{
    std::map<std::string, double> stat;
    Logger logger("statistics.log");

    vector<ItemC> data_c;
    data_c.reserve(data.size());
    for (auto &x : data){
        if (x.iscalc){
            data_c.push_back(x);
        }
    }

    double countP = data_c.size(),
            midP = mean(data_c),
            sdP = stdev(data_c);

    logger.logInfo(QString("Всего: %1").arg(countP, 0, 'g',3));
    logger.logInfo(QString("Среднее (по всем): %1").arg(midP, 0, 'g',3));
    logger.logInfo(QString("СКО (по всем): %1").arg(sdP, 0, 'g',3));

    stat["Всего"] = countP;
    stat["Среднее (по всем)"] = midP;
    stat["СКО (по всем)"] = sdP;

    vector<ItemC> data_n;
    data_n.reserve(data_c.size());
    for (auto &x : data_c){
        x.iscalc = x.iscalc && (abs(x.value - midP) < sdP);
        if (x.iscalc){
            data_n.push_back(x);
        }
    }

    double mid = mean(data_n);
    double sd = stdev(data_n);
    auto compare = [](const ItemC &x1, const ItemC &x2)->bool{return x1.value < x2.value;};
    double min_v = (*std::min_element(data_n.begin(), data_n.end(), compare)).value;
    double max_v = (*std::max_element(data_n.begin(), data_n.end(), compare)).value;

    for (auto &x : data_n){
        x.value_norm = (x.value - mid) / sd;
    }

    logger.logInfo(QString("Минимальное: %1").arg(min_v, 0, 'g',3));
    logger.logInfo(QString("Максимальное: %1").arg(max_v, 0, 'g',3));
    logger.logInfo(QString("Среднее: %1").arg(mid, 0, 'g',3));
    logger.logInfo(QString("Дисперсия: %1").arg(sd * sd, 0, 'g',3));
    logger.logInfo(QString("СКО: %1").arg(sd, 0, 'g',3));

    stat["Минимальное"] = min_v;
    stat["Максимальное"] = max_v;
    stat["Среднее"] = mid;
    stat["Дисперсия"] = sd * sd;
    stat["СКО"] = sd;

    int count_n = data_n.size();
    auto compare_n = [](const ItemC &x1, const ItemC &x2)->bool{return x1.value_norm < x2.value_norm;};

    double min_n = (*std::min_element(data_n.begin(), data_n.end(), compare_n)).value_norm;
    double max_n = (*std::max_element(data_n.begin(), data_n.end(), compare_n)).value_norm;

    logger.logInfo(QString("Минимальное (норм): %1").arg(min_n, 0, 'g',3));
    logger.logInfo(QString("Максимальное (норм): %1").arg(max_n, 0, 'g',3));

    stat["Минимальное (норм)"] = min_n;
    stat["Максимальное (норм)"] = max_n;

    logger.logInfo("Вычисление интервалов");

    auto intervals = getIntervals(Interval::INTERVAL_COUNT, min_n, max_n);
    double SumProb = 0;
    for (const auto &x : intervals){
        SumProb += x.pi;
        logger.logInfo(QString("Interval(%1, %2)").arg(x.low, 0, 'g',3).arg(x.high, 0, 'g',3));
    }

    for (const auto &x : data_n){
        double value = std::round(x.value_norm * 100) / 100;
        int index = -1;

        for (int j = 0; j < static_cast<int>(intervals.size()); j++){
            if (abs(value - intervals[j].high) < 1e-3){
                if (j < Interval::INTERVAL_COUNT / 2)
                    index = j + 1;
                else
                    index = j;
                break;
            }

            if (abs(value - intervals[j].low) < 1e-3){
                if (j < Interval::INTERVAL_COUNT / 2)
                    index = j;
                else
                    index = j - 1;
                break;
            }

            if ( (value > intervals[j].low) && (value < intervals[j].high+1e-3) ){
                index = j;
                break;
            }
        } // for (int j = 0; j < intervals.size(); j++)

        if (index == -1){
            if (value <= min_n + 1e-2)
                index = 0;
            else
                index = Interval::INTERVAL_COUNT - 1;
        }

        logger.logInfo(QString("Значение : %1   Интервал : %2")
                       .arg(value, 0, 'g',3).arg(index, 0, 'g',3));

        index = std::clamp(index, 0, Interval::INTERVAL_COUNT-1);
        intervals[index].value_count++;
    }  // for (const auto &x : data_n)

    double pis[5] = {0}, ns[5] = {0};

    int i = 0;
    for (auto &x : intervals){
        x.npi = count_n * x.pi / SumProb;
        x.ni_npi = (x.value_count - x.npi) * (x.value_count - x.npi);
        x.ni_npi_norm = x.ni_npi / x.npi;
        pis[i] = x.pi;
        ns[i] = x.value_count;

        logger.logInfo(QString("P[%1] : %2   N[%1] : %3").arg(i, 0, 'g',3)
                       .arg(x.pi, 0, 'g',3).arg(x.value_count, 0, 'g',3));

        i++;
    }
    stat["P[0]"] = pis[0];
    stat["P[1]"] = pis[1];
    stat["P[2]"] = pis[2];
    stat["P[3]"] = pis[3];
    stat["P[4]"] = pis[4];

    stat["N[0]"] = ns[0];
    stat["N[1]"] = ns[1];
    stat["N[2]"] = ns[2];
    stat["N[3]"] = ns[3];
    stat["N[4]"] = ns[4];

    double SumHi = 0;
    for (const auto &x : intervals)
        SumHi += x.ni_npi_norm;

    double SumHiTeor = chi_square_inverse(0.05, Interval::INTERVAL_COUNT-3);

    logger.logInfo(QString("Хи-квадрат (эмп.) : %1").arg(SumHi, 0, 'g', 3));
    logger.logInfo(QString("Хи-квадрат (теор.) : %1").arg(SumHiTeor, 0, 'g', 3));

    stat["Хи-квадрат (эмп.)"] = SumHi;
    stat["Хи-квадрат (теор.)"] = SumHiTeor;

    data_c.clear();
    data_c.reserve(data_n.size());
    for (auto &x : data_n){
        data_c.push_back(x);
    }

    double  midE = mean(data_c),
            sdE = stdev(data_c),
            d = (*std::max_element(data_c.begin(), data_c.end(),
                                 [=](const auto &x1, const auto &x2){
                                 return abs(x1.value - midE) < abs(x2.value - midE);})).value,
            Ek = abs(d - midE),
            Tk = Ek / sdE,
            FTk = 2 * Calculation::laplace(Tk);

    logger.logInfo(QString("Ek : %1").arg(Ek, 0, 'g', 3));
    logger.logInfo(QString("Tk : %1").arg(Tk, 0, 'g', 3));
    logger.logInfo(QString("F(Tk) : %1").arg(FTk, 0, 'g', 3));

    stat["Ek"] = Ek;
    stat["Tk"] = Tk;
    stat["F(Tk)"] = FTk;

    return stat;
}

std::pair<int, Task3::Material>
Task3::calcPlaneMaterial(std::vector<Task3::Sample> data, std::vector<Task3::Material> materials,
                         Task3::PlaneTypes type)
{
    int idx = 0;
    double val = data[0].lambda;

    for (int i = 0; i < static_cast<int>(data.size()); i++){
        if (data[i].lambda < val){
            idx = i;
            val = data[i].lambda;
        }
    }

    for (int i = 0; i < static_cast<int>(data.size()); i++){
        double  Tetta = Calculation::PI * data[i].tetta / 180.0,
                Lambda = data[i].lambda;

        switch (type){
            case PlaneTypes::IS_DIEL:
                if (i < idx){
                    data[i].n = sin(Tetta) / cos(Tetta)
                                * sqrt(1 + Lambda * Lambda - 2 * Lambda * cos(2 * Tetta))
                                / (1 - Lambda);
                }
                else{
                    data[i].n = sin(Tetta) / cos(Tetta)
                                * sqrt(1 + Lambda * Lambda + 2 * Lambda * cos(2 * Tetta))
                                / (1 + Lambda);
                }
                break;

            case PlaneTypes::IS_METAL:
                if (i < idx){
                    data[i].n = sin(Tetta) * sin(Tetta) / cos(Tetta) * (1 + Lambda) / (1 - Lambda);
                }
                else{
                    data[i].n = sin(Tetta) * sin(Tetta) / cos(Tetta) * (1 - Lambda) / (1 + Lambda);
                }
                break;

            default:
                break;
        }
    }  // for (int i = 0; i < data.size(); i++)

    double a = 0;
    for (auto x : data){
        a += x.n;
    }

    double nmid = a / (data.size() - 1);
    std::complex<double> coef;

    int ParamCount, i2;
    switch (type) {
        case PlaneTypes::IS_DIEL:
            coef = std::complex(nmid, 0.0);
            break;

        case PlaneTypes::IS_METAL:
        {
            int count = data.size() - 1;

            if (count / 2 == 1){
                ParamCount = count / 2 + 1;
                i2 = count - 1;
            }
            else{
                ParamCount = count / 2;
                i2 = count / 2;
            }

            vector<Param> params;
            params.reserve(ParamCount);
            for (int i = 0; i < ParamCount; i++){
                params.push_back(Param {0, 0, 0, 0, 0, 0, 0, 0});
            }

            for (int i = 0; i < static_cast<int>(data.size()); i++){
                if (i < ParamCount){
                    params[i].tetta1 = data[i].tetta;
                    params[i].a1 = data[i].n;
                }
                if (i >= i2){
                    params[i].tetta2 = data[i].tetta;
                    params[i].a2 = data[i].n;
                }
            }

            double u_mid = 0, v_mid = 0, c_mid = 0;
            for (auto &x : params){
                double d1 = det2(
                        std::pow(x.a1, 4) + x.a1*std::pow(sin(x.tetta1*Calculation::PI/180), 2),
                        std::pow(x.a2, 4) + x.a2*std::pow(sin(x.tetta2*Calculation::PI/180), 2),
                        x.a1 * x.a1,
                        x.a2 * x.a2
                    );
                double d2 = det2(1, 1, x.a1*x.a1, x.a2*x.a2);

                double d4 = 0, u = 0, v = 0;
                if (d1 / d2 < 0){
                    // все хуйня
                }
                else{
                    v = sqrt(d1 / d2);
                    double d3 = det2(
                            1,
                            1,
                            std::pow(x.a1, 4) + x.a1*std::pow(sin(x.tetta1*Calculation::PI/180), 2),
                            std::pow(x.a2, 4) + x.a2*std::pow(sin(x.tetta2*Calculation::PI/180), 2)
                        );
                    d4 = det2(1, 1, x.a1*x.a1, x.a2*x.a2);
                    u = d3 / d4;
                }

                if ( (abs(d2) > 1e-3) && (abs(d4) > 1e-3) ){
                    v_mid += v;
                    u_mid += u;
                    c_mid += 1;
                }
                else {
                    // все хуйня
                }
            }  // for (auto &x : params)

            v_mid /= c_mid;
            u_mid /= c_mid;

            double re = sqrt( (u_mid + sqrt(u_mid*u_mid + 4*v_mid*v_mid)) / 2 );
            coef = std::complex(re, -v_mid/re);

            break;
        }

        default:
            break;
    }

    idx = 0;
    int length = 1e+8;

    for (int i = 0; i < static_cast<int>(materials.size()); i++){
        materials[i].length = len2rect(coef.real(), coef.imag(), materials[i].re_min,
                                       materials[i].re_max, materials[i].im_min, materials[i].im_max);
        if (materials[i].length < length){
            idx = i;
            length = materials[i].length;
        }
    }

    return std::pair {idx, materials[idx]};
}


int Task3::Interval::INTERVAL_COUNT = 5;
