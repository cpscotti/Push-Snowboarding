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

#include "pushn8imudevice.h"

PushN8IMUDevice::PushN8IMUDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 50;


    //Assumes messages are like: "IMU,231,23,43,54,454,645,54,23,54,END;"
    packetLen = 11;
    packetStart = "IMU";
    packetEnd = "END";

    digitalAcc = false;
}

PushN8IMUDevice::~PushN8IMUDevice()
{

}


void PushN8IMUDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {
        //detect negative values to switch to digital mode.
        if(!digitalAcc) {
            if(params[4].toInt() < 0 ||
               params[5].toInt() < 0 ||
               params[6].toInt() < 0) {
                digitalAcc = true;
            }
        }

        NPushIMUTick * newIMUTick = new NPushIMUTick(params);
        newIMUTick->digitalAcc = digitalAcc;

//        qDebug() << "Parms: " << params[1] << "," << params[2] << "," << params[3];

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newIMUTick);
        } else {
            delete newIMUTick;
        }
    }

}

