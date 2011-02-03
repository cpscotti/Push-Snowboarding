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

#ifndef ABSNORMFEETTYPES_H
#define ABSNORMFEETTYPES_H

#define FOOT_PRESSURE_INVALID_THRESHOLD 50

#include "math.h"
#include <algorithm>

class PressurePointData
{
public:
    int minVal;
    int maxVal;

    int rawVal;
    float normVal;

    void update(int inVal)
    {
        rawVal = inVal;
        if(inVal > FOOT_PRESSURE_INVALID_THRESHOLD) maxVal = std::max(inVal, maxVal);
        if(inVal > FOOT_PRESSURE_INVALID_THRESHOLD) minVal = std::min(inVal, minVal);
        normVal = (inVal - minVal)*(1.0f/(maxVal-minVal));
    }

    void reset()
    {
        minVal = 1024;
        maxVal = 0;

        normVal = -1.0f;
        rawVal = -1;
    }
};

class SnowboradPressureData
{
public:
    PressurePointData LToe;
    PressurePointData RToe;

    PressurePointData LHeel;
    PressurePointData RHeel;

    void reset()
    {
        LToe.reset();
        RToe.reset();

        LHeel.reset();
        RHeel.reset();

        totalRawSum = 0;
    }

    int computeSum()
    {
        totalRawSum = LHeel.rawVal+LToe.rawVal+RHeel.rawVal+RToe.rawVal;
        return totalRawSum;
    }

    float computeNormFactor()
    {
        validCount = 0;
        totalNormFactor = 0.0;

        if(LHeel.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD && LToe.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD) {
            totalNormFactor += LHeel.normVal;
            validCount++;

            totalNormFactor += LToe.normVal;
            validCount++;
        }

//        if(LToe.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD) {
//            totalNormFactor += LToe.normVal;
//            validCount++;
//        }

        if(RHeel.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD && RToe.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD) {
            totalNormFactor += RHeel.normVal;
            validCount++;

            totalNormFactor += RToe.normVal;
            validCount++;
        }

//        if(RToe.rawVal > FOOT_PRESSURE_INVALID_THRESHOLD) {
//            totalNormFactor += RToe.normVal;
//            validCount++;
//        }

        totalNormFactor /= validCount;
        return totalNormFactor;
    }

    int totalRawSum;
    int validCount;
    float totalNormFactor;
};

#endif // ABSNORMFEETTYPES_H
