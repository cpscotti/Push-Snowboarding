/*
 * QBtDeviceDiscoverer.cpp
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
 
#include "../QBtDeviceDiscoverer.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtDeviceDiscoverer_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtDeviceDiscoverer_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtDeviceDiscoverer_stub.h"
#endif


//////////////////////////////////////////////////
// QBtDeviceDiscoverer::QBtDeviceDiscoverer()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtDeviceDiscoverer::QBtDeviceDiscoverer(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtDeviceDiscovererPrivate::NewL(this));
#else
    _implPtr = new QBtDeviceDiscovererPrivate(this);
#endif
}

//////////////////////////////////////////////////
// QBtDeviceDiscoverer::~QBtDeviceDiscoverer()	//
// Destructor									//
//////////////////////////////////////////////////
QBtDeviceDiscoverer::~QBtDeviceDiscoverer()
{
    SafeDelete(_implPtr);
}

//////////////////////////////////////////////////
// BluetoothDiscovery::startSearch()			//
// Called to start searching for new devices	//
//////////////////////////////////////////////////
void QBtDeviceDiscoverer::startDiscovery()
{
    _implPtr->DiscoverDevices();
}

//////////////////////////////////////////////////
// BluetoothDiscovery::stopSearch()				//
// Called to stop searching for new devices		//
//////////////////////////////////////////////////
void QBtDeviceDiscoverer::stopDiscovery()
{
    _implPtr->StopDiscovery();
}

//////////////////////////////////////////////////
// BluetoothDiscovery::stopSearch()				//
// Called to stop searching for new devices		//
//////////////////////////////////////////////////
const QBtDevice::List& QBtDeviceDiscoverer::getInquiredDevices() const
{
    return _implPtr->GetInquiredDevices();
}


bool QBtDeviceDiscoverer::isBusy() const
{
	return _implPtr->IsBusy();
}

