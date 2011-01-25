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

#include "pushn8footdevice.h"

PushN8FootDevice::PushN8FootDevice(QBtDevice a_device, QObject *parent) :
            PushN8BtDevice(a_device, parent)
{

    if(device.getName().count("FOOT_L") > 0)
    {
        side = 'L';
    } else if(device.getName().count("FOOT_R") > 0){
        side = 'R';
    } else {
        side = '?';
    }

    timerId = 0;
    timerPeriod = 50;

    //                                toes,heel
    //Assumes messages are like: "FOOT,231,3123,END;"
    packetLen = 4;
    packetStart = "FOOT";
    packetEnd = "END";

}

PushN8FootDevice::~PushN8FootDevice()
{
}

void PushN8FootDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {
        int ntoes = params[1].toInt();
        int nheel = params[2].toInt();

        NPushFootTick * newFootTick = new NPushFootTick(side, ntoes, nheel);

        qDebug() << "Foot data received: " << ntoes << ", " << nheel;

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newFootTick);
        } else {
            delete newFootTick;
        }
    }
}

char PushN8FootDevice::get_side()
{
    return side;
}
