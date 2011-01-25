/*
 * QBtObjectExchangeServer.cpp
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

#include "../QBtObjectExchangeServer.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtObjectExchangeServer_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtObjectExchangeServer_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtObjectExchangeServer_stub.h"
#endif


//////////////////////////////////////////////////////////
// QBtObjectExchangeServer::QBtObjectExchangeServer()	//
// Defaul constructor									//
//////////////////////////////////////////////////////////
QBtObjectExchangeServer::QBtObjectExchangeServer(QObject *parent) :
    QObject(parent), _advertiser(NULL), _service(NULL), _isConnected(false)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtObjectExchangeServerPrivate::NewL(this));
#else
    _implPtr = new QBtObjectExchangeServerPrivate(this);
#endif
}

QBtObjectExchangeServer::~QBtObjectExchangeServer()
{
    SafeDelete(_service);

    if(_advertiser)
    {
        _advertiser->stopAdvertising();
        delete _advertiser;
        _advertiser = NULL;
    }

    SafeDelete(_implPtr);
}

void QBtObjectExchangeServer::startServer(const QString& serviceName)
{
    QBtService tmpService;
    tmpService.setName(serviceName);
    setTransmittingService(tmpService);

    _implPtr->StartServer();
}

void QBtObjectExchangeServer::stopServer()
{
    _implPtr->Disconnect();
}

bool QBtObjectExchangeServer::isConnected()
{
    return _implPtr->IsConnected();
}

QBtService QBtObjectExchangeServer::getTransmittingServiceInfo()
{
    if(_service)
        return *_service;
    else
        return QBtService();
}

void QBtObjectExchangeServer::setTransmittingService(const QBtService& service)
{
    SafeDelete(_service);

    _service = new QBtService(service);
}

void QBtObjectExchangeServer::startAdvertisingService(const QBtService& service)
{
    setTransmittingService(service);
    _advertiser = new QBtServiceAdvertiser(this);
    _advertiser->startAdvertising(*_service);
}

void QBtObjectExchangeServer::stopAdvertisingService()
{
    if(_advertiser)
    {
        _advertiser->stopAdvertising();
        delete _advertiser;
        _advertiser = NULL;
    }
}
