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

#include "pushn8airtimedetector.h"


void PushN8AirTimeDetector::incomming_reading(NPushLogTick * gtick)
{
    bool runFuzzy = false;
    if(typeid(*gtick) == typeid(NPushNormFeetTick)) {
        NPushNormFeetTick * ftick = (NPushNormFeetTick *)gtick;

        fp = ftick->pdata.totalRawSum;
        currentTstamp = ftick->msecsToEpoch;

        runFuzzy = true;


//        qDebug() << "Got Sum: " << ftick->pdata.totalRawSum;
    } else if(typeid(*gtick) == typeid(NPushAccTick)) {
        NPushAccTick * atick = (NPushAccTick *)gtick;

        pa = atick->pAccAbsMag;
        currentTstamp = atick->msecsToEpoch;

        runFuzzy = true;

//        qDebug() << "Got Acc: " << atick->pAccAbsMag;
    } else if(typeid(*gtick) == typeid(NPushIMUTick)) {
        NPushIMUTick * itick = (NPushIMUTick *)gtick;
        currentTstamp = itick->msecsToEpoch;

        runFuzzy = true;

        double nacc[3];
        nacc[0] = (((double)itick->accel[0]) -511.0)*1.0/107.0;
        nacc[1] = (((double)itick->accel[1]) -511.0)*1.0/107.0;
        nacc[2] = (((double)itick->accel[2]) -511.0)*1.0/107.0;

        qreal imuAccAbs = qSqrt(nacc[0]*nacc[0] + nacc[1]*nacc[1] + nacc[2]*nacc[2]);

        ia = imuAccAbs;

//        qDebug() << "Got IMU: " << nacc[0] << "," << nacc[1] << "," << nacc[2] << " abs: " << imuAccAbs;

    }

    if(!isnan(fp) || !isnan(pa) || !isnan(ia))
    {
        if(tRunStart < 0)
            tRunStart = (double)((double)currentTstamp/1000.0);
        tRunCurr = (double)((double)currentTstamp/1000.0);
    }

    if( (!isnan(fp) || !isnan(pa) || !isnan(ia))
        && runFuzzy) {
//        qDebug() << currentTstamp;
        Response resp = fuzzyDetector.AskGodAboutAirTime(fp, pa, ia);
        handle_response(resp);
    }
}


void PushN8AirTimeDetector::handle_response(Response resp)
{

    NPushAirTimeTick * newTick = 0;
    if(resp == FlyingLikeAnEagle) {

//        qDebug() << currentTstamp;

        if(!flying) {
            takeOffTime = currentTstamp;
            lastAirborneTime = takeOffTime;
            flying = true;
        } else {
            if(currentTstamp > lastAirborneTime) {
                lastAirborneTime = currentTstamp;
            }
        }

        if(lastAirborneTime > takeOffTime && lastAirborneTime == currentTstamp) {
            quint64 airTime = lastAirborneTime-takeOffTime;

            qDebug() << "Counting Air Time";

            if(airTime > AIR_TIME_THRESHOLD) {
                newTick = new NPushAirTimeTick(airTime, false, currentTstamp);
            }
        }
    } else if(resp == SlidingDownLikeAPenguin) {
        if(flying) {

            if(lastAirborneTime > takeOffTime) {
                quint64 airTime = lastAirborneTime-takeOffTime;
                qDebug() << "Landed Air Time: " << airTime;
                if(airTime > AIR_TIME_THRESHOLD) {
                    newTick = new NPushAirTimeTick(airTime, true, std::max(currentTstamp, lastAirborneTime));
                    atReport->append_air(airTime, takeOffTime);
                }
            }
            flying = false;
        }
    }

    if(newTick != 0) {
        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            emit reading_ready(newTick);
        } else {
            delete newTick;
        }
    }
}

PushN8AirTimeDetector::PushN8AirTimeDetector()
{
    fp = pa = ia = NAN;
    flying = false;
    lastAirborneTime = 0;
    takeOffTime = 0;

    atReport = new NPushReportAirTime();
}

PushN8AirTimeDetector::~PushN8AirTimeDetector()
{
   if(atReport)
       delete atReport;
}


bool PushN8AirTimeDetector::subscribesToAny()
{
    return true;
}

bool PushN8AirTimeDetector::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8AbsNormFeetDevice)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8PhoneAcc)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8IMUDevice)){
        return true;
    } else {
        return false;
    }
}

QString PushN8AirTimeDetector::get_description()
{
    return "Air Time Detector";
}

void PushN8AirTimeDetector::start_readings()
{

}

void PushN8AirTimeDetector::stop_readings()
{

}

bool PushN8AirTimeDetector::is_online()
{
    return true;
}


bool PushN8AirTimeDetector::start_run()
{
    tRunStart = -1;
    tRunCurr = -1;
    return atReport->start_run();
}

bool PushN8AirTimeDetector::end_run()
{
    atReport->set_start_tstamp(tRunStart);
    atReport->set_end_tstamp(tRunCurr);
    bool ret = atReport->end_run();
    emit report_ready(atReport);
    return ret;
}
