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

#include "devicesmanager.h"

DevicesManager::DevicesManager(PushDevicesHolder * aDevHolder, QObject *parent) :
    QObject(parent), configuredDevices(aDevHolder)
{
    deviceDiscoverer = 0;
    searching = false;

    auto_connect = true;
    auto_connect_to_kit = 1;

    simulating = false;

    SetupPhoneDevices();
    SetupAbstractDevices();
}

DevicesManager::~DevicesManager()
{
    if(deviceDiscoverer)
        delete deviceDiscoverer;

    unknownFoundDevices.clear();
}

void DevicesManager::start_bt_search()
{
    if(!searching)
    {
        searching = true;
        QBtLocalDevice::askUserTurnOnBtPower();
        unknownFoundDevices.clear();
        if(deviceDiscoverer) delete deviceDiscoverer;
        deviceDiscoverer = new QBtDeviceDiscoverer(this);

        connect(deviceDiscoverer , SIGNAL(newDeviceFound (QBtDevice)), this, SLOT(deviceFound(QBtDevice)));

        deviceDiscoverer->startDiscovery();
        qDebug() << "Bluetooth Discovery started";
    }
}

void DevicesManager::deviceFound(QBtDevice newDevice)
{
    QString devName = newDevice.getName();
    qDebug() << "Found device " << devName;

    //Check if device name complies to PUSHN8 devices convention
    if(devName.count(QString("PUSHN8")) > 0 || devName.count(QString("FireFly")) > 0)
    {
        QString kitN = "_";
        kitN += QString::number(auto_connect_to_kit);
        if(auto_connect && (devName.count(kitN) > 0))
        {
            qDebug() << "Trying to auto-connect to " << devName;
            if(SetupDevice(newDevice) == true)//tries to auto-connect
            {
                //auto-connect succeded, return
                return;
            }
        }

        if(IsDeviceNovell(newDevice))
        {
            unknownFoundDevices.append(newDevice);
        }
    }
}

bool DevicesManager::SetupDevice(QBtDevice thisDev)
{
    if(IsDeviceNovell(thisDev))
    {

        qDebug() << "Creating PushN8Device";

        if(thisDev.getName().count("PUSHN8_FOOT") > 0)
        {
            //Adding to configuredDevices
            PushN8FootDevice * newN8Device = new PushN8FootDevice(thisDev);
            configuredDevices->push_back(newN8Device);

            if(newN8Device->get_side() == 'L')
            {
                emit foot_l_connecting();
                connect(newN8Device, SIGNAL(connected()), this, SIGNAL(foot_l_connected()));
            } else {
                emit foot_r_connecting();
                connect(newN8Device, SIGNAL(connected()), this, SIGNAL(foot_r_connected()));
            }
        } else if(thisDev.getName().count("PUSHN8_GSR") > 0)
        {
            //Adding to configuredDevices
            PushN8GSRDevice * newN8Device = new PushN8GSRDevice(thisDev);
            configuredDevices->push_back(newN8Device);

            emit gsr_connecting();
            connect(newN8Device, SIGNAL(connected()), this, SIGNAL(gsr_connected()));
        } else if(thisDev.getName().count("PUSHN8_MOTION") > 0)
        {
            //Adding to configuredDevices
            PushN8IMUDevice * newN8Device = new PushN8IMUDevice(thisDev);
            configuredDevices->push_back(newN8Device);

            emit motion_box_connecting();
            connect(newN8Device, SIGNAL(connected()), this, SIGNAL(motion_box_connected()));
        } else if(thisDev.getName().count("PUSHN8_HEART") > 0)
        {
            //Adding to configuredDevices
            PushN8HeartDevice * newN8Device = new PushN8HeartDevice(thisDev);
            configuredDevices->push_back(newN8Device);

            emit heart_connecting();
            connect(newN8Device, SIGNAL(connected()), this, SIGNAL(heart_connected()));
        } else {
            return false;
        }
        qDebug() << "PushN8Device added to list";
    }
    return true;
}

bool DevicesManager::IsDeviceNovell(QBtDevice dev)
{
    //Check if device is already on the list
    for(int i=0; i < configuredDevices->count(); i++)
    {
        //If it is a bt device
        if(typeid(configuredDevices->at(i)) == typeid(PushN8BtDevice))
        {
            PushN8BtDevice * currDev = (PushN8BtDevice *)configuredDevices->at(i);
            if(currDev->getDeviceName() == dev.getName())
                return false;
        }
    }

    return true;
}

void DevicesManager::stop_bt_search()
{
    if(searching)
    {
        searching = false;
        deviceDiscoverer->stopDiscovery();
    }
}

void DevicesManager::switch_to_simulation_device()
{
    //TODO
    qDebug() << "Switched to simulation device!";
    configuredDevices->deleteAllAndClear();

    PushN8SimulationDevice * simulationDevice = new PushN8SimulationDevice();
    configuredDevices->push_back(simulationDevice);

    SetupAbstractDevices();

    emit foot_l_connected();
    emit foot_r_connected();
    emit motion_box_connected();
    emit heart_connected();
    emit gsr_connected();
    emit phone_gps_connected();

}

void DevicesManager::kit_selected(int kitN)
{
    if(kitN > 0) {
        auto_connect_to_kit = kitN;
    } else {
        if(simulating)
        {
            qDebug() << "Already simulating devices, to go back, restart the app..";
        } else {
            simulating = true;
            switch_to_simulation_device();
        }
    }
}

void DevicesManager::SetupPhoneDevices()
{
    PushN8PhoneAcc * phoneAcc = new PushN8PhoneAcc();
    configuredDevices->push_back(phoneAcc);

    PushN8PhoneMag * phoneMag = new PushN8PhoneMag();
    configuredDevices->push_back(phoneMag);

    PushN8PhoneGPS * phoneGPS = new PushN8PhoneGPS(this);
    configuredDevices->push_back(phoneGPS);

    emit phone_gps_connecting();
    connect(phoneGPS, SIGNAL(connected()), this, SIGNAL(phone_gps_connected()));
}

void DevicesManager::SetupAbstractDevices()
{
    PushN8AbsNormFeetDevice * absFeetDevice = new PushN8AbsNormFeetDevice();
    configuredDevices->push_back(absFeetDevice);

    PushN8AirTimeDetector * airTimeDetector = new PushN8AirTimeDetector();
    configuredDevices->push_back(airTimeDetector);

    PushN8SimpleReportsGenerator * reportGenerator = new PushN8SimpleReportsGenerator();
    configuredDevices->push_back(reportGenerator);
}
