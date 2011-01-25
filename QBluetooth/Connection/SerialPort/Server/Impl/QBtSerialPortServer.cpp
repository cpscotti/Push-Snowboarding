/*
 * QBtSerialPortServer.cpp
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

#include "../QBtSerialPortServer.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtSerialPortServer_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtSerialPortServer_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtSerialPortServer_stub.h"
#endif


//////////////////////////////////////////////////
// QBtSerialPortServer::QBtSerialPortServer()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtSerialPortServer::QBtSerialPortServer(QObject *parent) :
    QObject(parent), _advertiser(NULL), _service(NULL), _isConnected(false) 
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtSerialPortServerPrivate::NewL(this));
#else
    _implPtr = new QBtSerialPortServerPrivate(this);
#endif
}

QBtSerialPortServer::~QBtSerialPortServer()
{
    SafeDelete(_implPtr);
    SafeDelete(_service);
}

void QBtSerialPortServer::startServer(const QString& serviceName)
{
    QBtService tmpService;
    tmpService.setName(serviceName);
    setTransmittingService(tmpService);

	#ifdef Q_OS_SYMBIAN
		QT_TRAP_THROWING (_implPtr->StartListenerL());
	#else
		_implPtr->StartListener();
	#endif
    
}

void QBtSerialPortServer::stopServer()
{
	#ifdef Q_OS_SYMBIAN
		QT_TRAP_THROWING (_implPtr->StopListenerL() );
	#else
		_implPtr->StopListener();
	#endif	
	
    
}

void QBtSerialPortServer::sendData(const QString & data)
{
    _implPtr->SendData(data);
}

bool QBtSerialPortServer::isConnected()
{
    return _isConnected;
}

QBtService QBtSerialPortServer::getTransmittingServiceInfo()
{
    if(_service)
        return *_service;
    else
        return QBtService();
}

void QBtSerialPortServer::setTransmittingService(const QBtService& service)
{
    SafeDelete(_service);
    _service = new QBtService(service);
}

void QBtSerialPortServer::setConnectionStatus(bool connected)
{
    _isConnected = connected;
}

void QBtSerialPortServer::startAdvertisingService(const QBtService& service)
{
    setTransmittingService(service);
    _advertiser = new QBtServiceAdvertiser(this);
    _advertiser->startAdvertising(service);
}

void QBtSerialPortServer::stopAdvertisingService()
{
    if(_advertiser)
    {
        _advertiser->stopAdvertising();
        delete _advertiser;
        _advertiser = NULL;
    }
}
