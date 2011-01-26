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

#include "pushn8btdevice.h"

PushN8BtDevice::PushN8BtDevice(QBtDevice a_device, QObject *parent) :
    PushBurtonGenericDevice(parent)
{
    qDebug() << "Initializing Instance of PushN8BtDevice";
    device = a_device;
    serviceDisc = new QBtServiceDiscoverer(this);

    connect(serviceDisc, SIGNAL(newServiceFound(QBtDevice,QBtService)), this, SLOT(serviceFound(QBtDevice,QBtService)));

    serviceDisc->startDiscovery(device, QBtConstants::SerialPort);
    qDebug() << "Services discovery started";
    SPPClient = 0;

    timerId = 0;
    timerPeriod = 500;

    packetStart = "SNRS";
    packetEnd = "END;";
    packetLen = 4;
}

void PushN8BtDevice::disconnect_from_backend()
{
    qDebug() << "Disconnecting device from (bt) backend";
    if(SPPClient) {
        if(SPPClient->isConnected()) {
            SPPClient->disconnect();
        }
//        SPPClient->deleteLater();
//        SPPClient = 0;
    }
    qDebug() << "Disconnected device from (bt) backend";
}

PushN8BtDevice::~PushN8BtDevice()
{
    disconnect_from_backend();
    SPPClient->deleteLater();
    if(serviceDisc)
    {
        serviceDisc->deleteLater();
    }
}

QString PushN8BtDevice::getDeviceName()
{
    return device.getName();
}

QString PushN8BtDevice::get_description()
{
    return device.getName();
}

void PushN8BtDevice::timerEvent(QTimerEvent * event)
{
    if(SPPClient)
    {
        if(SPPClient->isConnected())
        {
//            qDebug() << "Before sending data";
            SPPClient->sendData(QString("A"));
//            qDebug() << "After sending data";
        }
    }
//    qDebug() << "(!) Emitting bt device reading for " << this->get_description();
}

void PushN8BtDevice::serviceFound(QBtDevice dev, QBtService a_deviceService)
{
    qDebug() << "Found SPP service";
    service = a_deviceService;

    SPPClient = new QBtSerialPortClient(this);

    SPPClient->connect(dev, service);
    emit connected();


    connect (SPPClient, SIGNAL(dataReceived(QString)),
             this, SLOT (gotDataReceived(QString)));

    connect (SPPClient, SIGNAL(error(QBtSerialPortClient::ErrorCode)),
             this, SLOT(gotError(QBtSerialPortClient::ErrorCode)));


    connect(SPPClient, SIGNAL(connectionResetByPeer()), this, SLOT(connectionResetByPeer()));
    connect(SPPClient, SIGNAL(disconnectedFromServer()), this, SLOT(gotDisconnected()));

}

void PushN8BtDevice::gotDisconnected()
{
    qDebug() << "Disconnected from server";
}

void PushN8BtDevice::connectionResetByPeer()
{
    qDebug() << "Connection Reset by peer at " << this;
    this->stop_readings();
    emit disconnected();
//    disconnect_from_backend();
}

void PushN8BtDevice::gotDataReceived(QString data)
{
    QStringList params = data.split(",");
    if(params.count() == 4) {
        if(params[0] == "SNRS1") {
//            NPushAccTick * newAccTick = new NPushAccTick(accSensor->reading());
            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                //anyone listening to us
//                emit reading_ready(newAccTick);
            } else {
//                delete newAccTick;
            }
        }
    }
//    qDebug() << "Data received: " << data;
}

void PushN8BtDevice::gotError(QBtSerialPortClient::ErrorCode error)
{
    this->stop_readings();
    disconnect_from_backend();
    qDebug() << "Error received: " << error;
}

bool PushN8BtDevice::is_online()
{
    if(SPPClient)
        return SPPClient->isConnected();
    else
        return false;
}

QString PushN8BtDevice::getDetails()
{
    QString ret;
    ret.append("\tName: " + device.getName() + "\n");
//    ret.append("\tAddress: " + device.getAddress() + "\n");
    ret.append("\tFound SSP: " + QString((SPPClient)?"YES":"NO") + "\n");
    return ret;
}

bool PushN8BtDevice::getPackage(QString data, QStringList& result)
{
//    QStringList PushN8BtDevice::getPackage(QString data)
    dataBuffer.append(data);

//    qDebug() << "current buffer: " << dataBuffer;

    QStringList heap = dataBuffer.split(QRegExp("(,|;|\\n|\\r)+"), QString::SkipEmptyParts);

    /* this is not perfect and can be improved */
    /* but it is pretty good as it is right now... */

    if(heap.count() < packetLen)
    {
        return false;
    } else {
        for(int i=0; i < (heap.count()-packetLen+1); i++)
        {
            if(heap[i] == packetStart)
            {
//                qDebug() << "Match at " << i;

                //Anything prior to that can be discarded

                if(heap[i+packetLen-1] == packetEnd)
                {
                    dataBuffer.clear();
                    result.clear();
//                    qDebug() << "got package:" << heap[i+1] << "," << heap[i+2];
                    for(int j=i;j<i+packetLen;j++)
                        result.push_back(heap[j]);
//                    qDebug() << "got full package ";// << result[1] << "," << result[2];
                    return true;
                } else {
//                    qDebug() << "end is " << heap[i+packetLen-1] << "; clearing all";
                    dataBuffer.clear();
                    return false;
                }
            }
        }
        return false;
    }
}
