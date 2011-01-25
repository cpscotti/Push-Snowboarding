/*
 * QBtSerialPortClient.cpp
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

#include "../QBtSerialPortClient.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtSerialPortClient_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtSerialPortClient_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtSerialPortClient_stub.h"
#endif


//////////////////////////////////////////////////
// QBtSerialPortClient::QBtSerialPortClient()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtSerialPortClient::QBtSerialPortClient(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtSerialPortClientPrivate::NewL(this));
#else
    _implPtr = new QBtSerialPortClientPrivate(this);
#endif
}

QBtSerialPortClient::~QBtSerialPortClient()
{
    SafeDelete(_implPtr);
}

bool QBtSerialPortClient::isConnected()
{
    return _implPtr->IsConnected();
}

void QBtSerialPortClient::connect(const QBtDevice& remoteDevice, const QBtService& remoteService)
{
#ifdef Q_OS_SYMBIAN
	QT_TRAP_THROWING(_implPtr->ConnectL(remoteDevice, remoteService) );
#else
	_implPtr->Connect (remoteDevice, remoteService);
#endif    
	
}

void QBtSerialPortClient::disconnect()
{
    _implPtr->Disconnect();
}


void QBtSerialPortClient::sendData(const QString& data)
{
    _implPtr->SendData(data);
}


void QBtSerialPortClient::sendData (const QByteArray & data)
{
	_implPtr->SendData(data);
}
