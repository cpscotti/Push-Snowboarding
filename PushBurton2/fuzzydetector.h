/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef FUZZYDETECTOR_H
#define FUZZYDETECTOR_H

#include <QVector>
#include <QDebug>

#include <QtXml>

#include "FilesystemConventions.h"

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
            if(QVector<Relation>::at(i-1).inVal <= inVal
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

    Fuzzificator bpa_onGrnd;


//    Response FuzzyficateToAirOrGround(double fp, double pa, double ia);
    Response FuzzyficateToAirOrGround(double fp, double pa, double ia, double bpa = 0.0);

    void SaveToXml(QString settingsFileName);
    void LoadFromXml(QString settingsFileName);

//    inline double max(double a, double b) {
//        return (a>b)?a:b;
//    }

};

#endif // FUZZYDETECTOR_H
