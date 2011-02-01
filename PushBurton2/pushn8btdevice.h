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

#ifndef PUSHN8BTDEVICE_H
#define PUSHN8BTDEVICE_H

#include <QObject>
#include <QDebug>

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

#include <QRegExp>

#include "pushburtongenericdevice.h"
#include "npushlogtick.h"

class PushN8BtDevice : public PushBurtonGenericDevice
{
#ifdef Q_OS_SYMBIAN
    Q_OBJECT
public:
    explicit PushN8BtDevice(QBtDevice a_device, QObject *parent = 0);
    ~PushN8BtDevice();

    QString getDeviceName();

    QString getDetails();

    QString get_description();

    bool is_online();

    QString dataBuffer;

    virtual void disconnect_from_backend();

protected:
    QBtDevice device;
    QBtSerialPortClient * SPPClient;

    QString packetStart;
    QString packetEnd;
    int packetLen;

    QStringList getPackage(QString data);

    bool getPackage(QString data, QStringList& result);

private:
    QBtServiceDiscoverer * serviceDisc;
    QBtService service;


protected:
    virtual void timerEvent(QTimerEvent *);

signals:

public slots:

    void serviceFound(QBtDevice,QBtService);
    void gotDataReceived(QString);
    void gotError(QBtSerialPortClient::ErrorCode);

    void gotDisconnected();
    void connectionResetByPeer();
    void lost_signal();
    void regained_signal();

#endif
};

#endif // PUSHN8BTDEVICE_H
