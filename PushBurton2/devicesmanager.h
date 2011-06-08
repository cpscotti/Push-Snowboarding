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

//Headers only necessary when bluetooth is available/on symbian device
#ifdef Q_OS_SYMBIAN
#include <QBluetooth/QBtDevice.h>
#include <QBluetooth/QBtService.h>
#include <QBluetooth/QBtConstants.h>
#include <QBluetooth/QBluetooth.h>
#include <QBluetooth/QBtDeviceDiscoverer.h>
#include <QBluetooth/QBtServiceDiscoverer.h>
#include <QBluetooth/QBtSerialPortClient.h>
#include <QBluetooth/QBtLocalDevice.h>
#endif

#include <QSignalMapper>

#include "pushdevicesholder.h"

#ifdef Q_OS_SYMBIAN
#include "pushn8btdevice.h"
#include "pushn8footdevice.h"
#include "pushn8gsrdevice.h"
#include "pushn8imudevice.h"
#include "pushn8heartdevice.h"
#endif

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

    //This may seem abusive at first look
    //  but makes complete sense since they are just routed from
    //  signals comming from different objects and well.. at some point you need to break into!
//    void phone_gps_connecting();
//    void phone_gps_connected();

//    void motion_box_connecting();
//    void motion_box_connected();

//    void heart_connecting();
//    void heart_connected();

//    void gsr_connecting();
//    void gsr_connected();

//    void foot_l_connecting();
//    void foot_l_connected();

//    void foot_r_connecting();
//    void foot_r_connected();


public slots:

    //inner mechanisms
#ifdef Q_OS_SYMBIAN
    void deviceFound(QBtDevice);
#endif

    //comm with user app
    void start_bt_search();
    void stop_bt_search();
    void kit_selected(int);
    void switch_to_simulation_device(const QString& fname = "");


private:
    PushDevicesHolder * configuredDevices;

    bool searching;
    bool auto_connect;
    int auto_connect_to_kit;

    bool simulating;


    QSignalMapper connectedMapper;
    QSignalMapper disconnectedMapper;

#ifdef Q_OS_SYMBIAN
    QVector<QBtDevice> unknownFoundDevices;
    QBtDeviceDiscoverer * deviceDiscoverer;

    bool SetupDevice(QBtDevice);
    bool IsDeviceNovell(QBtDevice);
#endif

    void SetupPhoneDevices();
    void SetupAbstractDevices();

};

#endif // DEVICESMANAGER_H
