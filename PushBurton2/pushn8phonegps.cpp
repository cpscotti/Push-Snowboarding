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

#include "pushn8phonegps.h"

PushN8PhoneGPS::PushN8PhoneGPS(QObject * parent)
{
    computeUTCOffset();
    gpsSource = QGeoPositionInfoSource::createDefaultSource(0);

    connect(gpsSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
            this, SLOT(positionUpdated(QGeoPositionInfo)));

    gpsSource->requestUpdate(60000);
    gpsSource->setUpdateInterval(500);

    gpsOnline = false;

    timerId = -1;//disable any periodic events


}

PushN8PhoneGPS::~PushN8PhoneGPS()
{
    gpsSource->deleteLater();
}

void PushN8PhoneGPS::start_readings()
{
    gpsSource->startUpdates();
}

void PushN8PhoneGPS::stop_readings()
{
    gpsSource->stopUpdates();
}

void PushN8PhoneGPS::timerEvent(QTimerEvent *)
{

}

QString PushN8PhoneGPS::get_description()
{
    return QString("Phone's Internal Gps");
}

bool PushN8PhoneGPS::is_online()
{
    return gpsOnline;
}

void PushN8PhoneGPS::positionUpdated(QGeoPositionInfo info)
{
    if(info.isValid())
    {
        //emitting connected signal when receiving first reading
        if(!gpsOnline)
            emit connected();

        gpsOnline = true;

        QDateTime correctedTstamp;
        correctedTstamp = info.timestamp().addSecs(UTCOffset);//ugly localisation!
        info.setTimestamp(correctedTstamp);

        NPushGpsTick * newGPSTick = new NPushGpsTick(info);

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            emit reading_ready(newGPSTick);
        } else {
            //TODO memory leak danger here put on "stand by"
            delete newGPSTick;
        }
    }

}

void PushN8PhoneGPS::computeUTCOffset()
{
#ifdef Q_OS_SYMBIAN
    RTz tzServer;
    CTzId* timeZoneID;

    User::LeaveIfError(tzServer.Connect());

    CleanupClosePushL(tzServer);

    timeZoneID = tzServer.GetTimeZoneIdL();

    RArray<TInt> tzid;
    RArray<TInt> utcOffsets;

    tzid.Append(timeZoneID->TimeZoneNumericID());
    tzServer.GetOffsetsForTimeZoneIdsL(tzid, utcOffsets);

    UTCOffset = utcOffsets[0]*60;

    qDebug() << "Local Offset to UTC" << UTCOffset;

    tzid.Close();
    utcOffsets.Close();

    CleanupStack::PushL(timeZoneID);
    CleanupStack::PopAndDestroy(timeZoneID);
    CleanupStack::PopAndDestroy(1);
#else
    UTCOffset = 0;
#endif
}
