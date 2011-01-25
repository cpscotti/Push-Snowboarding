/*
 * QBtLocalDevice_stub.cpp
 *
 *  
 *      Author: Ftylitakis Nikolaos
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

#include "../QBtLocalDevice_stub.h"

TBool QBtLocalDevicePrivate::isBluetoothSupported()
{
	return false;
}

void QBtLocalDevicePrivate::AskUserTurnOnBtPower()
{
	return;
}

bool QBtLocalDevicePrivate::GetBluetoothPowerState()
{
	return false;
}

bool QBtLocalDevicePrivate::SetBluetoothPowerState (bool value)
{
	return false;
}



QBtAddress QBtLocalDevicePrivate::GetLocalDeviceAddress()
{
	return QBtAddress();
}

bool QBtLocalDevicePrivate::GetLimitedDiscoverableStatus()
{
	return false;
}

void QBtLocalDevicePrivate::SetLimitedDiscoverableStatus(bool isDiscoverabilityLimited)
{
	
}

QBtDevice::DeviceMajor QBtLocalDevicePrivate::GetDeviceClass()
{
	return QBtDevice::Uncategorized;
}

void QBtLocalDevicePrivate::SetDeviceClass(QBtDevice::DeviceMajor classId)
{
	
}

void QBtLocalDevicePrivate::SetLocalDeviceName(const QString & deviceName)
{
	
}

QString QBtLocalDevicePrivate::GetLocalDeviceName()
{
	return "";
}

bool QBtLocalDevicePrivate::AddNewDevice(const QBtDevice& device)
{
	return false;
}

bool QBtLocalDevicePrivate::DeleteDevice(const QBtDevice& device)
{
	return false;
}

bool QBtLocalDevicePrivate::DeleteDevice(const QBtAddress& address)
{
	return false;
}

bool QBtLocalDevicePrivate::UnpairDevice(const QBtDevice& device)
{
	return false;
}
