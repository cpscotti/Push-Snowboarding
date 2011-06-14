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

#include "pushn8gsrdevice.h"

PushN8GSRDevice::PushN8GSRDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 500;

    //Assumes messages are like: "GSR,231,END;"
    packetLen = 3;
    packetStart = "GSR";
    packetEnd = "END";

}

PushN8GSRDevice::~PushN8GSRDevice()
{
}

void PushN8GSRDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {
        int gsrr = params[1].toInt();
        if(gsrr > 0 && gsrr < 1024)
        {
            NPushGSRTick * newGSRTick = new NPushGSRTick(gsrr);

            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                //anyone listening to us
                emit reading_ready(newGSRTick);
            } else {
                delete newGSRTick;
            }
        }
    }
//    qDebug() << "Data received: " << data;
}

QString PushN8GSRDevice::getName()
{
    return "push.n8.bt.gsr";
}
