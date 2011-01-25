/*
 * QBtObjectExchangeClient_stub.cpp
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

#include "../QBtObjectExchangeClient_stub.h"

QBtObjectExchangeClientPrivate::QBtObjectExchangeClientPrivate(
		QBtObjectExchangeClient* publicClass) : p_ptr(publicClass)
{
}

QBtObjectExchangeClientPrivate::~QBtObjectExchangeClientPrivate()
{
	
}

void QBtObjectExchangeClientPrivate::ConnectToServer(const QBtDevice& remoteDevice, const QBtService& remoteService)
{
	
}

void QBtObjectExchangeClientPrivate::Disconnect()
{
	
}

void QBtObjectExchangeClientPrivate::SendFile(const QString& fileName)
{
	
}

void QBtObjectExchangeClientPrivate::SendData (const QByteArray& data, const QString& bufferName)
{
	
}

void QBtObjectExchangeClientPrivate::Abort()
{
	
}

bool QBtObjectExchangeClientPrivate::IsConnected()
{
	return false;
}

bool QBtObjectExchangeClientPrivate::IsBusy()
{
	return false;
}

bool QBtObjectExchangeClientPrivate::SetPath (const QString & path)
{
	return false;
}

void QBtObjectExchangeClientPrivate::GetFile(
		const QString& localPath, const QString& remoteFileNameFull)
{
	
}

void QBtObjectExchangeClientPrivate::GetByteBuffer(const QString& dataName)
{
	
}
