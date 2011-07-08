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

#ifndef DEVICESMANAGER_H
#define DEVICESMANAGER_H

#include <QObject>
#include <QDebug>


#include <QSignalMapper>

#include "pushdevicesholder.h"

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>

#include <QMap>

#include "qtmpushdevice.h"

#include "pushn8phoneacc.h"
#include "pushn8phonegps.h"
#include "pushn8phonemag.h"

#include "broadcasterdevice.h"

#include "pushn8absnormfeetdevice.h"
#include "pushn8airtimedetector.h"
#include "pushn8simplereportsgenerator.h"

#include "virtualbrosdevice.h"
#include "rotationcounter.h"

//#include "pushn8unprogdev.h"
#include "pushn8simulationdevice.h"

class DevicesManager : public QObject
{
    Q_OBJECT
public:
    DevicesManager(PushDevicesHolder * aDevHolder, QObject *parent = 0);
    ~DevicesManager();

signals:
    void deviceConnected(QString description);

    void device_connecting(const QString&);
    void device_connected(const QString&);
    void device_disconnected(const QString&);

    void request_run_start();
    void request_run_end();

    void allDiscoveryFinished();

    //This may seem abusive at first look
    //  but makes complete sense since they are just routed from
    //  signals comming from different objects and well.. at some point you need to break into!

public slots:

    void serviceDiscovered(QBluetoothServiceInfo info);
    void srvcDiscErrorDetected(QBluetoothServiceDiscoveryAgent::Error error);
    void srvcDiscoveryAgentFinished();

    void deviceDiscovered(QBluetoothDeviceInfo info);
    void deviceDiscErrorDetected(QBluetoothDeviceDiscoveryAgent::Error);
    void deviceDiscoveryAgentFinished();


    //comm with user app
    void start_bt_search();
    void stop_bt_search();
    void kit_selected(int);
    void switch_to_simulation_device(const QString& fname = "");


private:
    PushDevicesHolder * configuredDevices;

    QBluetoothDeviceDiscoveryAgent * devDiscoveryAgent;
    QBluetoothLocalDevice * localDevice;

    QMap<QBluetoothAddress, QString> knownBtDevices;

    bool searching;
    bool auto_connect;
    int auto_connect_to_kit;

    bool simulating;


    QSignalMapper connectedMapper;
    QSignalMapper disconnectedMapper;

    void SetupPhoneDevices();
    void SetupAbstractDevices();

};

#endif // DEVICESMANAGER_H
