/*
 * QBtLocalDevice.cpp
 *
 *
 *      Author: Ftylitakis Nikolaos, Luis Valente
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QBtLocalDevice.h>
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtLocalDevice_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtLocalDevice_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtLocalDevice_stub.h"
#endif

QBtAddress QBtLocalDevice::getLocalDeviceAddress()
{
    return QBtLocalDevicePrivate::GetLocalDeviceAddress();
}

bool QBtLocalDevice::isVisible()
{
    return QBtLocalDevicePrivate::IsVisible();
}

void QBtLocalDevice::setVisible (bool value)
{
    QBtLocalDevicePrivate::SetVisible (value);
}

QBtDevice::DeviceMajor QBtLocalDevice::getDeviceClass()
{
    return QBtLocalDevicePrivate::GetDeviceClass();
}

void QBtLocalDevice::setDeviceClass(QBtDevice::DeviceMajor classId)
{
    QBtLocalDevicePrivate::SetDeviceClass(classId);
}

void QBtLocalDevice::setLocalDeviceName (const QString & deviceName)
{
    QBtLocalDevicePrivate::SetLocalDeviceName (deviceName);
}

QString QBtLocalDevice::getLocalDeviceName()
{
    return QBtLocalDevicePrivate::GetLocalDeviceName();
}

////////////////////////////////

bool QBtLocalDevice::isBluetoothSupported()
{
    return QBtLocalDevicePrivate::IsBluetoothSupported();
}

void QBtLocalDevice::askUserTurnOnBtPower()
{
    QBtLocalDevicePrivate::AskUserTurnOnBtPower();
}

bool QBtLocalDevice::getBluetoothPowerState()
{
    return QBtLocalDevicePrivate::GetBluetoothPowerState();
}

bool QBtLocalDevice::setBluetoothPowerState (bool value)
{
	return QBtLocalDevicePrivate::SetBluetoothPowerState (value);
}

/////////////////////////////////

bool QBtLocalDevice::addNewDevice(const QBtDevice& device)
{
    return QBtLocalDevicePrivate::AddNewDevice(device);
}

bool QBtLocalDevice::deleteDevice(const QBtDevice& device)
{
    return QBtLocalDevice::deleteDevice(device.getAddress());
}

bool QBtLocalDevice::deleteDevice(const QBtAddress& address)
{
    return QBtLocalDevicePrivate::DeleteDevice(address);
}

bool QBtLocalDevice::unpairDevice(const QBtDevice& device)
{
    return QBtLocalDevice::unpairDevice(device.getAddress());
}

bool QBtLocalDevice::unpairDevice(const QBtAddress& address)
{
    return QBtLocalDevicePrivate::UnpairDevice(address);
}
