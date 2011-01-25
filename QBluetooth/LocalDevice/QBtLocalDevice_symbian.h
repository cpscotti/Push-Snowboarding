/*
 * QBtLocalDevice_symbian.h
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

#ifndef QBTLOCALDEVICE_SYMBIAN_H_
#define QBTLOCALDEVICE_SYMBIAN_H_

#include <e32base.h>
#include <e32std.h>
#include <featureinfo.h>
#include <featdiscovery.h>
#include <btnotifierapi.h>
#include <centralrepository.h>
#include <BTServerSDKCRKeys.h>
#include <btdevice.h>
#include <btmanclient.h>
#include <e32property.h>
#include <bt_subscribe.h>

#include <QBtLocalDevice.h>

class QBtLocalDevicePrivate
{
public:

    ///////////////////////////////////////
    ///	 Bluetooth local properties		///
    ///////////////////////////////////////
    static QBtAddress GetLocalDeviceAddress();
    static TBool IsVisible();
    static void SetVisible (TBool value);
    static QBtDevice::DeviceMajor GetDeviceClass();
    static void SetDeviceClass(QBtDevice::DeviceMajor classId);
    static void SetLocalDeviceName (const QString & deviceName);
    static QString GetLocalDeviceName();


    ///////////////////////////////
    ///	 Bluetooth Hardware		///
    ///////////////////////////////
    static TBool IsBluetoothSupported();
    static void AskUserTurnOnBtPower();
    static TBool GetBluetoothPowerState();
    static TBool SetBluetoothPowerState (TBool value); 


    ///////////////////////////////
    ///	 Bluetooth Registry		///
    ///////////////////////////////
    static TBool AddNewDevice(const QBtDevice& device);
    static TBool DeleteDevice(const QBtAddress& address);
    static TBool UnpairDevice(const QBtAddress& address);

private:
    static TBool DefineProperty(TUint propertyId, RProperty::TType typeEnum);
    static TBool DeleteOrUnpairDevice(const QBtAddress& address, TBool unpair);

private:
    QBtLocalDevice* p_ptr;
};

#endif /* QBTLOCALDEVICE_SYMBIAN_H_ */
