/*
 * QBtServiceDiscoverer.cpp
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

#include "../QBtServiceDiscoverer.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtServiceDiscoverer_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtServiceDiscoverer_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtServiceDiscoverer_stub.h"
#endif

//////////////////////////////////////////////////
// QBtServiceDiscoverer::QBtServiceDiscoverer()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtServiceDiscoverer::QBtServiceDiscoverer(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtServiceDiscovererPrivate::NewL(this));
#else
    _implPtr = new QBtServiceDiscovererPrivate(this);
#endif
}

//////////////////////////////////////////////////////
// QBtServiceDiscoverer::~QBtServiceDiscoverer()	//
// Destructor										//
//////////////////////////////////////////////////////
QBtServiceDiscoverer::~QBtServiceDiscoverer()
{
    SafeDelete(_implPtr);
}

//////////////////////////////////////////////////
// QBtServiceDiscoverer::startDiscovery()		//
// Called to start searching for new services	//
//////////////////////////////////////////////////
void QBtServiceDiscoverer::startDiscovery (const QBtDevice& targetDevice)
{
    _remoteDevice = targetDevice;
    _implPtr->DiscoverServicesOnDevice(& _remoteDevice);
}

//////////////////////////////////////////////////////////////////////////
// QBtServiceDiscoverer::startSearch(									//
//									QBtDevice* targetDevice, 			//
//									QBtService::ServiceClass serviceId)	//
// Called to start searching for specific services matching				//
// with serviceId							 							//
//////////////////////////////////////////////////////////////////////////
void QBtServiceDiscoverer::startDiscovery (const QBtDevice& targetDevice, QBtConstants::ServiceClass uuid)
{
    _remoteDevice = targetDevice;
    _implPtr->DiscoverSpecificClass (& _remoteDevice, uuid);
}

void QBtServiceDiscoverer::startDiscovery (const QBtDevice & targetDevice, QBtConstants::ServiceProtocol uuid)
{
    _remoteDevice = targetDevice;
    _implPtr->DiscoverSpecificProtocol (& _remoteDevice, uuid);
}

//////////////////////////////////////////////////
// QBtServiceDiscoverer::stopDiscovery()		//
// Called to stop searching for new services	//
//////////////////////////////////////////////////
void QBtServiceDiscoverer::stopDiscovery()
{
    _implPtr->StopDiscovery();
}

//////////////////////////////////////////////////////
// QBtServiceDiscoverer::GetSelectedDevice()		//
// Called to get target device. If service search	//
// took place for this device then it's field		//
// "supportedServices" will be updated				//
//////////////////////////////////////////////////////
const QBtDevice & QBtServiceDiscoverer::getTargetDevice() const
{
    return _remoteDevice;
}

QBtService::List QBtServiceDiscoverer::getInquiredServices()
{
    return _remoteDevice.getSupportedServices();
}


bool QBtServiceDiscoverer::isBusy () const
{
	return _implPtr->IsBusy();
}
