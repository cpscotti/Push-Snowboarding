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

#include "pushn8phoneacc.h"

PushN8PhoneAcc::PushN8PhoneAcc()
{
    accSensor = new QAccelerometer();
    accSensor->connectToBackend();
    if(!accSensor->isConnectedToBackend()) {
        qDebug() << "Could not connect mAccel to backend";
    }

    if(!accSensor->start()) {
        qDebug() << "Could not start mAccel";
    }

    if(!accSensor->isActive()) {
        qDebug() << "mAccel is not active";
    }

    emit connected();

    timerId = 0;
    timerPeriod = 50;
}

PushN8PhoneAcc::~PushN8PhoneAcc()
{
    if(accSensor)
        delete accSensor;
}

QString PushN8PhoneAcc::get_description()
{
    return QString("Phone Internal Accelerometer");
}

bool PushN8PhoneAcc::is_online()
{
    if(accSensor)
        return accSensor->isActive();
    else
        return false;
}

void PushN8PhoneAcc::timerEvent(QTimerEvent * event)
{
    NPushAccTick * newAccTick = new NPushAccTick(accSensor->reading());

    if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
    {
        emit reading_ready(newAccTick);
    } else {
        //TODO memory leak danger here put on "stand by"
        delete newAccTick;
    }
}
