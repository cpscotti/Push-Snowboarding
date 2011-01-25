/*
 * QBtLocalDevice_win32.h
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

#include "../QBtLocalDevice_win32.h"
#include <QBtAuxFunctions.h>

QBtAddress QBtLocalDevicePrivate::GetLocalDeviceAddress()
{
    BTINT32 result = BTSDK_FALSE;
    BTUINT8 devAddr[6] = {0};
    result = Btsdk_GetLocalDeviceAddress(devAddr);

    if(result == BTSDK_OK)
    {
        QByteArray btAddrr((char*)devAddr, 6);
        return QBtAddress::getAddressFromReversedArray(btAddrr);
    }
    return QBtAddress();
}

bool QBtLocalDevicePrivate::GetLimitedDiscoverableStatus()
{
    BTUINT16 pmode;
    BTINT32 retBool = BTSDK_FALSE;

    retBool = Btsdk_GetDiscoveryMode(&pmode);

    if((pmode & BTSDK_LIMITED_DISCOVERABLE) != 0)
        return true;

    return false;
}

void QBtLocalDevicePrivate::SetLimitedDiscoverableStatus(bool isDiscoverabilityLimited)
{
    BTUINT16 usMode = BTSDK_GENERAL_DISCOVERABLE * (!isDiscoverabilityLimited)
                        | BTSDK_LIMITED_DISCOVERABLE * isDiscoverabilityLimited
                        | BTSDK_CONNECTABLE
                        | BTSDK_PAIRABLE;

    Btsdk_SetDiscoveryMode(usMode);

    return;
}

QBtDevice::DeviceMajor QBtLocalDevicePrivate::GetDeviceClass()
{
    BTUINT32 devClass;
    BTINT32 result = BTSDK_FALSE;
    result = Btsdk_GetLocalDeviceClass(&devClass);

    if(result != BTSDK_OK)
        return QBtDevice::Uncategorized;

    if(devClass & BTSDK_DEVCLS_COMPUTER)
        return QBtDevice::Computer;
    else if(devClass & BTSDK_DEVCLS_PHONE)
        return QBtDevice::Phone;
    else if(devClass & BTSDK_DEVCLS_LAP)
        return QBtDevice::LANAccess;
    else if(devClass & BTSDK_DEVCLS_AUDIO)
        return QBtDevice::AudioVideo;
    else if(devClass & BTSDK_DEVCLS_PERIPHERAL)
        return QBtDevice::Peripheral;
    else if(devClass & BTSDK_DEVCLS_IMAGE)
        return QBtDevice::Imaging;
    else if(devClass & BTSDK_DEVCLS_IMAGE)
        return QBtDevice::Imaging;
    else if(devClass & BTSDK_DEVCLS_WEARABLE)
        return QBtDevice::Wearable;
    else
        return QBtDevice::Uncategorized;
}

void QBtLocalDevicePrivate::SetDeviceClass(QBtDevice::DeviceMajor classId)
{
    BTUINT32 devClass;
    BTINT32 result = BTSDK_FALSE;
    result = Btsdk_GetLocalDeviceClass(&devClass);

    if(result != BTSDK_OK)
        return;

    QBtDevice::DeviceMajor currentDeviceClass = GetDeviceClass();

    BTUINT32 newDevClass = (devClass ^ (currentDeviceClass * 0x100)) | (classId * 0x100);

    Btsdk_SetLocalDeviceClass(newDevClass);
}

void QBtLocalDevicePrivate::SetLocalDeviceName(const QString & devName)
{
    Btsdk_SetLocalName((BTUINT8*)devName.toUtf8().constData(), devName.size());
}

QString QBtLocalDevicePrivate::GetLocalDeviceName()
{
    BTINT32 result = BTSDK_FALSE;
    BTUINT8 devName[BTSDK_DEVNAME_LEN] = {0};
    BTUINT16 nameSize = BTSDK_DEVNAME_LEN;

    result = Btsdk_GetLocalName(devName, &nameSize);

    if(result == BTSDK_OK)
        return QString::fromUtf8((char*)devName);
    else
        return "Unknown";
}

////////////////////////////////

bool QBtLocalDevicePrivate::IsBluetoothSupported()
{
    return Btsdk_IsServerConnected();
}

void QBtLocalDevicePrivate::AskUserTurnOnBtPower()
{
    Btsdk_StartBluetooth();
}

bool QBtLocalDevicePrivate::GetBluetoothPowerState()
{
    return Btsdk_IsBluetoothReady();
}

bool QBtLocalDevicePrivate::SetBluetoothPowerState (bool /*value*/)
{
	return false;
}

/////////////////////////////////

bool QBtLocalDevicePrivate::AddNewDevice(const QBtDevice& device)
{
    BTUINT8 devAddr[6] = {0};
    BTDEVHDL devHandle = BTSDK_INVALID_HANDLE;
    QByteArray addrArray = device.getAddress().toReversedByteArray();
    memcpy(devAddr, addrArray.constData(), addrArray.size());
    devHandle = Btsdk_AddRemoteDevice(devAddr);

    return (devHandle != BTSDK_INVALID_HANDLE);
}

bool QBtLocalDevicePrivate::DeleteDevice(const QBtDevice& device)
{
    return QBtLocalDevicePrivate::DeleteDevice(device.getAddress());
}

bool QBtLocalDevicePrivate::DeleteDevice(const QBtAddress& address)
{
    BTINT32 result = BTSDK_FALSE;
    //get device handle
    BTDEVHDL devHandle = BTSDK_INVALID_HANDLE;
    BTUINT8 btAddr [6]= {0};
    QByteArray btAddrQt = address.toReversedByteArray();
    memcpy(btAddr, btAddrQt.constData(), btAddrQt.size());
    devHandle = Btsdk_GetRemoteDeviceHandle(btAddr);

    if(devHandle == BTSDK_INVALID_HANDLE)
        return false;

    result = Btsdk_DeleteRemoteDeviceByHandle (devHandle);

    return (result == BTSDK_OK);
}

bool QBtLocalDevicePrivate::UnpairDevice(const QBtDevice& device)
{
    return QBtLocalDevicePrivate::UnpairDevice(device.getAddress());
}

bool QBtLocalDevicePrivate::UnpairDevice(const QBtAddress& address)
{
    BTINT32 result = BTSDK_FALSE;
    //get device handle
    BTDEVHDL devHandle = BTSDK_INVALID_HANDLE;
    BTUINT8 btAddr [6]= {0};
    QByteArray btAddrQt = address.toReversedByteArray();
    memcpy(btAddr, btAddrQt.constData(), btAddrQt.size());
    devHandle = Btsdk_GetRemoteDeviceHandle(btAddr);

    if(devHandle == BTSDK_INVALID_HANDLE)
        return false;

    result = Btsdk_UnPairDevice (devHandle);

    return (result == BTSDK_OK);
}


//////////////////////////////////////////////////////////////////////////
bool QBtLocalDevicePrivate::IsVisible()
{
	BTUINT16 pmode;
	BTINT32 retBool = BTSDK_FALSE;

	retBool = Btsdk_GetDiscoveryMode(&pmode);

	if((pmode & BTSDK_LIMITED_DISCOVERABLE || pmode & BTSDK_DISCOVERABLE) != 0)
		return true;

	return false;
}

void QBtLocalDevicePrivate::SetVisible (bool value)
{
	BTUINT16 usMode =  BTSDK_CONNECTABLE
						| BTSDK_PAIRABLE | BTSDK_DISCOVERABLE*((int)value);

	Btsdk_SetDiscoveryMode(usMode);

	return;
}
