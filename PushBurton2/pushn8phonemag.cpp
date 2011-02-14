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

#include "pushn8phonemag.h"

PushN8PhoneMag::PushN8PhoneMag()
{
    magSensor = new QMagnetometer();

    magSensor->connectToBackend();
    if(!magSensor->isConnectedToBackend()) {
        qDebug() << "Could not connect Magnetometer to backend";
    }

    if(!magSensor->start()) {
        qDebug() << "Could not start Magnetometer";
    }

    if(!magSensor->isActive()) {
        qDebug() << "Magnetometer is not active";
    }

    emit connected();

    timerId = 0;
    timerPeriod = 200;
}

PushN8PhoneMag::~PushN8PhoneMag()
{
    if(magSensor)
        delete magSensor;
}

QString PushN8PhoneMag::get_description()
{
    return QString("Phone Internal Magnetometer");
}

bool PushN8PhoneMag::is_online()
{
    if(magSensor)
        return magSensor->isActive();
    else
        return false;
}

void PushN8PhoneMag::timerEvent(QTimerEvent * event)
{
    NPushMagTick * newMagTick = new NPushMagTick(magSensor->reading());

    if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
    {
        emit reading_ready(newMagTick);
    } else {
        delete newMagTick;
    }
}
