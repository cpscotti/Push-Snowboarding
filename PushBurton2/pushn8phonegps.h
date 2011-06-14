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

#ifndef PUSHN8PHONEGPS_H
#define PUSHN8PHONEGPS_H

#include <qmobilityglobal.h>
#include <QGeoPositionInfoSource>
#include <QDateTime>

#include "npushgpstick.h"

#include "pushburtongenericdevice.h"

#include <math.h>

#ifdef Q_OS_SYMBIAN
#include <tz.h>
#include <TzLocalizer.h>
#include <TzLocalizationDataTypes.h>
#endif

QTM_BEGIN_NAMESPACE
class QGeoPositionInfo;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

class PushN8PhoneGPS : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8PhoneGPS(QObject * parent);

    ~PushN8PhoneGPS();

    QString getName();

    bool is_online();

    virtual void start_readings();
    virtual void stop_readings();

public slots:
    void positionUpdated(QGeoPositionInfo);

private:

    void computeUTCOffset();

    double getDelta2Pos(QGeoPositionInfo& i, QGeoPositionInfo& f);

    bool streaming;

    QGeoPositionInfoSource * gpsSource;
    bool gpsOnline;
    int UTCOffset;

//    QGeoPositionInfo lastInfo;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // PUSHN8PHONEGPS_H
