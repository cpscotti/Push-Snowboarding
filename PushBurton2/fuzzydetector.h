#ifndef FUZZYDETECTOR_H
#define FUZZYDETECTOR_H

#include <QVector>
#include <QDebug>

#include <QtXml>

enum Response
{
    FlyingLikeAnEagle = 42,
    SlidingDownLikeAPenguin,
    CantHelpYouOnThisOne
};

struct Relation
{
public:

    Relation(double _i, double _o) : inVal(_i), outVal(_o)
    {}

    Relation()
    {}

    double inVal;
    double outVal;
};

class Fuzzificator : public QVector<Relation>
{
public:
    double fuzificate(double inVal)
    {
        int relCnt = QVector<Relation>::count();
        if(relCnt <= 1) {
            qDebug() << "Fuzzificator Problem!! No Fuzification curve";
            return NAN;
        }

        int i;
        Relation left,right;
        for(i=1;i<relCnt;i++) {
            if(QVector<Relation>::at(i-1).inVal < inVal
               && QVector<Relation>::at(i).inVal >= inVal) {
                left = QVector<Relation>::at(i-1);
                right = QVector<Relation>::at(i);
                break;
            }
        }

        if(i == relCnt) {
            qDebug() << "Fuzzificator Problem!! inVal out of fuzification range";
            return NAN;
        }

        /*
         fuzification line is: (l_i,l_o) -> (r_i, r_o)  // x_i (in), x_o (out)

         for a test point t_i, t_o is:

         t_o = l_o+ (r_o - l_o)/(r_i - l_i) * (t_i - l_i)

         */

        double outVal = left.outVal + ((right.outVal - left.outVal)/(right.inVal - left.inVal))*(inVal - left.inVal);

        return outVal;
    }
};

class FuzzyDetector
{
public:
    FuzzyDetector();
    ~FuzzyDetector();

    Fuzzificator fp_onAir;
    Fuzzificator fp_onGrnd;

    Fuzzificator pa_onAir;
    Fuzzificator pa_onGrnd;

    Fuzzificator ia_onAir;
    Fuzzificator ia_onGrnd;


    Response AskGodAboutAirTime(double fp, double pa, double ia);

    void SaveToXml(QString settingsFileName);
    void LoadFromXml(QString settingsFileName);


};

#endif // FUZZYDETECTOR_H
