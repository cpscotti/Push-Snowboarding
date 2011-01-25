/*
 * QBtSerialPortClient_win32.cpp
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

#include "../QBtSerialPortClient_win32.h"

#include <stdio.h>

QBtSerialPortClientPrivate::QBtSerialPortClientPrivate(QBtSerialPortClient* publicClass) : 
	p_ptr(publicClass), com(NULL), connectionHandle(BTSDK_INVALID_HANDLE), server(NULL),
	connectingService(NULL), OSComPort(0), serialHandle(INVALID_HANDLE_VALUE), incomingDataBuffer("")
{
	InitBthSdk();
	PLugInCommPort();
}

QBtSerialPortClientPrivate::~QBtSerialPortClientPrivate()
{
	CloseSerialPortHandle();
	Disconnect();
	DeinitializeCommPort();
	
	SafeDelete(server);
	SafeDelete(connectingService);
	
	DeinitBthSdk();
}

void QBtSerialPortClientPrivate::Connect(
		const QBtDevice& remoteDevice, 
		const QBtService& remoteService)
{
	if(!remoteDevice.getAddress().isValid())
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorNoDeviceSelected);
		return;
	}
		
	if(remoteService.getClass() == QBtConstants::UndefinedClass &&
		remoteService.getHandle() == 0)
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorNoServiceSelected);
		return;
	}
	
	server = new QBtDevice(remoteDevice);
	connectingService = new QBtService(remoteService);
		
	if(com)
		CloseSerialPortHandle();
	
	if(IsConnected())
		Disconnect();
	
	if(connectingService->getHandle() != 0)
	{
		if(!ConnectBtSerialPortProfile_Handle())
		{
			emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection);
			return;
		}
	}
	else
		if(!ConnectBtSerialPortProfile_ServClass())
		{
			emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection);
			return;
		}
		
	if(!InitializeSerialPortHandle())
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection);
		return;
	}
		
}

void QBtSerialPortClientPrivate::Disconnect()
{
	BTINT32 ulRet = BTSDK_FALSE;

	if(connectionHandle != BTSDK_INVALID_HANDLE){
		ulRet = Btsdk_Disconnect(connectionHandle);
		connectionHandle = BTSDK_INVALID_HANDLE;
	}
}

void QBtSerialPortClientPrivate::SendData(const QByteArray& data)
{
	if(!IsConnected() || com == NULL)
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorUnavailable);
		return;
	}

	DWORD size = data.size();

	com->sendData((char*)data.data(),size);
}

void QBtSerialPortClientPrivate::SendData(const QString& data)
{
	if(!IsConnected() || com == NULL)
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorUnavailable);
		return;
	}

	DWORD size = data.size();

	com->sendData((char*)data.toUtf8().constData(),size);

}

bool QBtSerialPortClientPrivate::IsConnected()
{
	return (connectionHandle != BTSDK_INVALID_HANDLE);
}

void QBtSerialPortClientPrivate::CloseSerialPortHandle()
{
	if(com)
	{
		com->disconnect();
		delete com;
		com = NULL;
	}
}

/************************************************************************/
/*               Connection Specific functions		                    */
/************************************************************************/

bool QBtSerialPortClientPrivate::ConnectBtSerialPortProfile_Handle()
{
	BTINT32 ulRet = BTSDK_FALSE;
	BtSdkSPPConnParamStru sppStru = {0};
	sppStru.size = sizeof(BtSdkSPPConnParamStru);
	sppStru.com_index = (BTUINT8)OSComPort ;

	int connCnt = 0;
	do
	{
		ulRet = Btsdk_Connect(connectingService->getHandle(), (BTUINT32)&sppStru, &connectionHandle ); 
		connCnt++;
	}while(ulRet != BTSDK_OK && connCnt < 5);

	connCnt = 0;
	if(ulRet != BTSDK_OK)
		emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection);

	return (ulRet == BTSDK_OK);
}

bool QBtSerialPortClientPrivate::ConnectBtSerialPortProfile_ServClass()
{
	BTINT32 ulRet = BTSDK_FALSE;
	BtSdkSPPConnParamStru sppStru = {0};
	sppStru.size = sizeof(BtSdkSPPConnParamStru);
	sppStru.com_index = (BTUINT8)OSComPort ;
	//BTUINT16 service = BTSDK_CLS_SERIAL_PORT;
	
	BTDEVHDL devHandle = GetDeviceHandle(server->getAddress());

	int connCnt = 0;
	do
	{
		ulRet = Btsdk_ConnectEx(devHandle, connectingService->getClass(), (BTUINT32)&sppStru, &connectionHandle ); 
		connCnt++;
	}while(ulRet != BTSDK_OK && connCnt < 5);

	connCnt = 0;
	if(ulRet != BTSDK_OK)
		emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection);

	return (ulRet == BTSDK_OK);
}

/************************************************************************/
/*					Virtual comm port manipulation                      */
/************************************************************************/

bool QBtSerialPortClientPrivate::PLugInCommPort()
{
	BTINT32 ulRet = BTSDK_FALSE;
	BTUINT32 s_ComSerialNum = 0;

	//----------------------------connect------------------------//
	s_ComSerialNum = Btsdk_GetASerialNum();
	Btsdk_PlugInVComm(s_ComSerialNum , &OSComPort ,1 ,COMM_SET_RECORD|COMM_SET_USAGETYPE, 2200);
	ulRet = Btsdk_InitCommObj((BTUINT8)OSComPort, BTSDK_CLS_SERIAL_PORT);
	ulRet = BTSDK_OK;

	if(ulRet != BTSDK_OK)
	{
		emit p_ptr->error(QBtSerialPortClient::ErrorUnableToInitializePort);
		return false;
	}

	return true;
}

void QBtSerialPortClientPrivate::DeinitializeCommPort()
{
	BTINT32 ulRet = BTSDK_FALSE;
	BTUINT32 serialNum = 0;
	
	if(OSComPort == 0)
		return;

	serialNum = Btsdk_CommNumToSerialNum(OSComPort);
	Btsdk_PlugOutVComm(serialNum, COMM_SET_RECORD);
	ulRet = Btsdk_DeinitCommObj(OSComPort);
}

bool QBtSerialPortClientPrivate::InitializeSerialPortHandle()
{
	int	erreur;

/*#ifdef _DEBUG
	char szPort[10];
	wsprintf( szPort, "\\\\.\\COM%d", OSComPort );
#endif
#ifndef _DEBUG*/
	char szPort[10];
	sprintf( szPort, "\\\\.\\COM%d", OSComPort );
//#endif

	if(com != NULL)
		return false;

	com = new Tserial_event();

	if(com)
	{
		com->setManager(&QBtSerialPortClientPrivate::SerialEventManager);

		erreur = com->connect(szPort, 19200, SERIAL_PARITY_NONE, 8, false);

		if(!erreur)
		{
			com->setRxSize(1);
			com->owner = (void*) this;
		}

		return (!erreur);
	}
	else
		return false;

}


/************************************************************************/

/************************************************************************/
/*                       Serial Port event handling                     */
/************************************************************************/

void QBtSerialPortClientPrivate::SerialEventManager(uint32 object, uint32 event)
{
	Tserial_event *com;
	QBtSerialPortClientPrivate *handlingObject;

	com = (Tserial_event *) object;
	if (com!=0)
	{
		handlingObject = (QBtSerialPortClientPrivate *) com->owner;
		if (handlingObject!=0)
			handlingObject->SerialCallback(com, event);
	}
}

void QBtSerialPortClientPrivate::SerialCallback(Tserial_event *com_source, uint32 event)
{
	char *buffer;
	int   size;
	Tserial_event *com;

	com = (Tserial_event *) com_source;
	if (com!=0)
	{
		switch(event)
		{
		case  SERIAL_CONNECTED  :
			emit p_ptr->connectedToServer();
			break;
		case  SERIAL_DISCONNECTED  :
			emit p_ptr->disconnectedFromServer();
			break;
		case  SERIAL_DATA_SENT  :
			emit p_ptr->dataSent();
			break;
		case  SERIAL_DATA_ARRIVAL  :
			size   = com->getDataInSize();
			buffer = com->getDataInBuffer();

			if ((size>0) && (buffer!=0))
			{
				//if(buffer[0] == '\0')
				//{
				//	incomingDataBuffer.clear();
				//}
				//else
				//{
					QString newArray = QString::fromUtf8(buffer, size);
					emit p_ptr->dataReceived(newArray);
				//	incomingDataBuffer.append(newArray);
				//}
			}
			else
				emit p_ptr->error(QBtSerialPortClient::ErrorUndefinedError);

			com->dataHasBeenRead();
			break;
		}
	}
}


/************************************************************************/


