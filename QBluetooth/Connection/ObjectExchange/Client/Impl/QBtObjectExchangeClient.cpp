/*
 * QBtObjectExchangeClient.cpp
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

#include "../QBtObjectExchangeClient.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtObjectExchangeClient_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtObjectExchangeClient_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtObjectExchangeClient_stub.h"
#endif


//////////////////////////////////////////////////
// QBtObjectExchangeClient::QBtObjectExchangeClient()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtObjectExchangeClient::QBtObjectExchangeClient(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtObjectExchangeClientPrivate::NewL(this));
#else
    _implPtr = new QBtObjectExchangeClientPrivate(this);
#endif
}

QBtObjectExchangeClient::~QBtObjectExchangeClient()
{
    SafeDelete(_implPtr);
}

void QBtObjectExchangeClient::connectToServer(
        const QBtDevice& remoteDevice,
        const QBtService& remoteService)
{
    _implPtr->ConnectToServer(remoteDevice, remoteService);
}

void QBtObjectExchangeClient::disconnect()
{
    _implPtr->Disconnect();
}

void QBtObjectExchangeClient::getFile(const QString& localPath, const QString& remoteFileNameFull)
{
    _implPtr->GetFile(localPath, remoteFileNameFull);
}

void QBtObjectExchangeClient::getByteBuffer(const QString& dataName)
{
    _implPtr->GetByteBuffer(dataName);
}

void QBtObjectExchangeClient::putFile(const QString& fileName)
{
    _implPtr->SendFile(fileName);
}

void QBtObjectExchangeClient::putByteBuffer(const QByteArray& data, const QString& bufferName)
{
    _implPtr->SendData(data, bufferName);
}

void QBtObjectExchangeClient::abort()
{
    _implPtr->Abort();
}

void QBtObjectExchangeClient::setPath(const QString & path)
{
    _implPtr->SetPath(path);
}

// accessor commands
QBtService GetTransmittingService()
{
    return QBtService();
}

QBtDevice GetServerDevice()
{
    return QBtDevice();
}


bool QBtObjectExchangeClient::isBusy() const
{
	return _implPtr->IsBusy();
}


bool QBtObjectExchangeClient::isConnected() const
{
	return _implPtr->IsConnected();
}
