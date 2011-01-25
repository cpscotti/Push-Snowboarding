/*
 * QBtLocalDevice_stub.h
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

#ifndef QBTLOCALDEVICE_STUB_H_
#define QBTLOCALDEVICE_STUB_H_

class QBtLocalDevicePrivate
{
public:
	
	///////////////////////////////////////
	///	 Bluetooth local properties		///
	///////////////////////////////////////
	static QBtAddress GetLocalDeviceAddress();
	static bool GetLimitedDiscoverableStatus();
	static void SetLimitedDiscoverableStatus(bool isDiscoverabilityLimited);
	static QBtDevice::DeviceMajor GetDeviceClass();
	static void SetDeviceClass(QBtDevice::DeviceMajor classId);
	static void SetLocalDeviceName(const QString & deviceName);
	static QString GetLocalDeviceName();

	
	///////////////////////////////
	///	 Bluetooth Hardware		///
	///////////////////////////////
	static bool IsBluetoothSupported();
	static void AskUserTurnOnBtPower();
	static bool GetBluetoothPowerState();
	static bool SetBluetoothPowerState (bool value); 
	

	///////////////////////////////
	///	 Bluetooth Registry		///
	///////////////////////////////
	static bool AddNewDevice(const QBtDevice& device);
	static bool DeleteDevice(const QBtDevice& device);
	static bool DeleteDevice(const QBtAddress& address);
	static bool UnpairDevice(const QBtDevice& device);
	static bool UnpairDevice(const QBtAddress& address);

};

#endif /* QBTLOCALDEVICE_STUB_H_ */
