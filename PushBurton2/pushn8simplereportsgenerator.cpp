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

#include "pushn8simplereportsgenerator.h"

PushN8SimpleReportsGenerator::PushN8SimpleReportsGenerator()
{
    gsrReport = new NPushReportGSR();
    heartReport = new NPushReportHeart();
    speedReport = new NPushReportSpeed();
    altitudeReport = new NPushReportAltitude();
}

PushN8SimpleReportsGenerator::~PushN8SimpleReportsGenerator()
{

    if(gsrReport)
        delete gsrReport;

    if(heartReport)
        delete heartReport;

    if(speedReport)
        delete speedReport;

    if(altitudeReport)
        delete altitudeReport;
}

bool PushN8SimpleReportsGenerator::start_run()
{
    gsrReport->start_run();
    heartReport->start_run();
    speedReport->start_run();
    altitudeReport->start_run();
    return false;
}

bool PushN8SimpleReportsGenerator::end_run()
{
    bool ret = gsrReport->end_run() && heartReport->end_run() && speedReport->end_run() && altitudeReport->end_run();

    emit report_ready(gsrReport);
    emit report_ready(heartReport);
    emit report_ready(speedReport);
    emit report_ready(altitudeReport);
    return ret;
}


void PushN8SimpleReportsGenerator::incoming_reading(NPushLogTick * gtick)
{
    if(typeid(*gtick) == typeid(NPushGSRTick)) {
        //handle gsr
        NPushGSRTick * gsrtick = (NPushGSRTick *)gtick;

        gsrReport->append_new_reading(gsrtick->reading);

    } else if(typeid(*gtick) == typeid(NPushHeartTick)) {
        //handle heart
        NPushHeartTick * hearttick = (NPushHeartTick *)gtick;

        heartReport->append_new_reading(hearttick->hr_3);

    } else if(typeid(*gtick) == typeid(NPushGpsTick)) {
        //handle gps
        NPushGpsTick * gpsTick = (NPushGpsTick *)gtick;

        speedReport->append_new_reading(gpsTick->tick.attribute(QGeoPositionInfo::GroundSpeed));
        altitudeReport->append_new_reading(gpsTick->tick.coordinate().altitude());
    }
}

QString PushN8SimpleReportsGenerator::getName()
{
    return "push.report.simple";
}

void PushN8SimpleReportsGenerator::start_readings()
{

}

void PushN8SimpleReportsGenerator::stop_readings()
{

}

bool PushN8SimpleReportsGenerator::is_online()
{
    return true;
}

bool PushN8SimpleReportsGenerator::subscribesToAny()
{
    return true;
}

bool PushN8SimpleReportsGenerator::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8GSRDevice)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8PhoneGPS)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8HeartDevice)){
        return true;
    } else {
        return false;
    }
}
