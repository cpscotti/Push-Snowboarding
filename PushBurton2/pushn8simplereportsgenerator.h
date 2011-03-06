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

#ifndef PUSHN8SIMPLEREPORTSGENERATOR_H
#define PUSHN8SIMPLEREPORTSGENERATOR_H

#include <typeinfo>

#include "pushburtongenericdevice.h"

//Subscribes to
#include "pushn8gsrdevice.h"
#include "pushn8heartdevice.h"
#include "pushn8phonegps.h"

//Inputs
#include "npushgsrtick.h"
#include "npushhearttick.h"
#include "npushgpstick.h"

//Reports
#include "npushreportgsr.h"
#include "npushreportheart.h"
#include "npushreportspeed.h"
#include "npushreportaltitude.h"

class PushN8SimpleReportsGenerator : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8SimpleReportsGenerator();
    ~PushN8SimpleReportsGenerator();

    bool subscribesToAny();
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

    QString get_description();

    void start_readings();
    void stop_readings();

    bool is_online();


    NPushReportGSR * gsrReport;
    NPushReportHeart * heartReport;
    NPushReportSpeed * speedReport;
    NPushReportAltitude * altitudeReport;

protected:
    bool start_run();
    bool end_run();

public slots:
    void incoming_reading(NPushLogTick *);


};

#endif // PUSHN8SIMPLEREPORTSGENERATOR_H
