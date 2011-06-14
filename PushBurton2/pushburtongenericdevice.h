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

#ifndef PUSHBURTONGENERICDEVICE_H
#define PUSHBURTONGENERICDEVICE_H



#include <QObject>
#include <QString>
//#include <QVector>
#include "npushlogtick.h"
#include "npushgenreport.h"

/* This is the base class for all devices.
 * This adds to all its subclasses the following functionalities
    * Adds QObject basic functionality for all devices (emit/slots/signals)
    * Periodic timer for sensor pooling (subclasses assign period value to timerPeriod variable)
    * defines the common interface for all sensors

 * Devices name should follow:
 All devices: push.*
    -> n8 specific devices: push.n8.*
        -> bt specific devices: push.n8.bt.*
    -> qt specific devices/sensors: push.qt.*
    -> abstract devices: push.abstract.*
        -> trick detectors: push.abstract.snowb.*
    -> report generators: push.report.
    -> network devices: push.network.*

    -> (TEMP) saver (yeh.. this'll be a device!): push.logsaver
    -> (TEMP) auto run manager (yes... we should have that!): push.runmanager.


 */
class PushBurtonGenericDevice : public QObject
{
    Q_OBJECT
public:

    PushBurtonGenericDevice(QObject * parent = 0);

    virtual ~PushBurtonGenericDevice() = 0;

    virtual QString getName() = 0;

    virtual void start_readings();
    virtual void stop_readings();

    virtual bool is_online() = 0;

    virtual void disconnect_from_backend();

    virtual bool start_run();
    virtual bool end_run();

    virtual bool subscribesToAny();
    virtual bool subscribesTo(PushBurtonGenericDevice* deviceType);

protected:
    int timerId;
    int timerPeriod;



public slots:
    virtual void incoming_reading(NPushLogTick *);

signals:
    void reading_ready(NPushLogTick *);

    void report_ready(NPushGenReport *);

    void connected();
    void disconnected();
};

#endif // PUSHBURTONGENERICDEVICE_H
