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

#include "pushn8heartdevice.h"

PushN8HeartDevice::PushN8HeartDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 1000;

    //Assumes messages are like: "HRM,80,75,90,END;"
    packetLen = 5;
    packetStart = "HRM";
    packetEnd = "END";
}

PushN8HeartDevice::~PushN8HeartDevice()
{

}

void PushN8HeartDevice::gotDataReceived(QString data)
{
    QStringList params;
    if(getPackage(data, params))
    {
        int hr_3 = params[1].toInt();
        int hr_2 = params[2].toInt();
        int hr_1 = params[3].toInt();

        NPushHeartTick * newHeartTick = new NPushHeartTick(hr_3, hr_2, hr_1);

        //qDebug() << "Data received: " << data;

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newHeartTick);
        } else {
            delete newHeartTick;
        }
    }
}

QString PushN8HeartDevice::getName()
{
    return "push.n8.bt.heart";
}
