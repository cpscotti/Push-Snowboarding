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

#ifndef PUSHN8AIRTIMEDETECTOR_H
#define PUSHN8AIRTIMEDETECTOR_H

#define AIR_TIME_THRESHOLD 220

#include "npushlogtick.h"

#include "npushimutick.h"
#include "npushacctick.h"
#include "npushnormfeettick.h"

#include "npushairtimetick.h"

#include "pushburtongenericdevice.h"

#include "AbsNormFeetTypes.h"

#include "pushn8absnormfeetdevice.h"
#include "pushn8phoneacc.h"
#include "pushn8imudevice.h"

#include "fuzzydetector.h"


#include "npushreportairtime.h"

class PushN8AirTimeDetector : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8AirTimeDetector();

    ~PushN8AirTimeDetector();

    bool subscribesToAny();
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

    QString get_description();

    void start_readings();
    void stop_readings();

    bool is_online();

    SnowboradPressureData last_pdata;

    NPushReportAirTime * atReport;

protected:
    bool start_run();
    bool end_run();

private:

    void handle_response(Response);
    bool flying;

    FuzzyDetector fuzzyDetector;

    quint64 currentTstamp;
    quint64 takeOffTime;
    quint64 lastAirborneTime;

    double tRunStart;
    double tRunCurr;

    double fp,pa,ia;

public slots:
    void incomming_reading(NPushLogTick *);

};

#endif // PUSHN8AIRTIMEDETECTOR_H
