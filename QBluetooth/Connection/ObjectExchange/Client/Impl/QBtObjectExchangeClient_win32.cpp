/*
 * QBtObjectExchangeClient_win32.cpp
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

#include "../QBtObjectExchangeClient_win32.h"

bool QBtObjectExchangeClientPrivate::isBusy = false;

QBtObjectExchangeClientPrivate::QBtObjectExchangeClientPrivate(QBtObjectExchangeClient* publicClass) :
	p_ptr(publicClass), connectionHandle(BTSDK_INVALID_HANDLE), server(NULL), connectingService(NULL),
	maxFileNameSize(256)
{
	InitBthSdk();
}

QBtObjectExchangeClientPrivate::~QBtObjectExchangeClientPrivate()
{
	Disconnect();
	DeinitBthSdk();
	
	SafeDelete(server);
	SafeDelete(connectingService);
}

/**
* ConnectToServer
* Connect to the server
* @param remoteDevice the remote server to connect to
* @param remoteServive the remote service of the server
*/
void QBtObjectExchangeClientPrivate::ConnectToServer(const QBtDevice& remoteDevice, const QBtService& remoteService)
{
	//check
	if( !remoteDevice.getAddress().isValid())
	{
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientNoSelectedDevice);
		return;
	}
	if( remoteService.getClass()  == QBtConstants::UndefinedClass &&
		remoteService.getHandle() == 0)
	{
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientNoSelectedService);
		return;
	}
	
	server = new QBtDevice(remoteDevice);
	connectingService = new QBtService(remoteService);
	
	BTDEVHDL devHandle = GetDeviceHandle(server->getAddress());

	BTINT32 result = BTSDK_FALSE;

	if(connectionHandle != BTSDK_INVALID_HANDLE)
		result = Btsdk_Connect(connectingService->getHandle(), 0, &connectionHandle);
	else
		result = Btsdk_ConnectEx(devHandle, connectingService->getClass(), 0, &connectionHandle);

	if(result != BTSDK_OK)
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientConnectionError);
	else
		emit p_ptr->connectedToServer();
}

/**
* Disconnect
* Disconnect from the remote machine, by sending an
* OBEX disconnect, and closing the transport on (and
* regardless of) response from the server.
*/
void QBtObjectExchangeClientPrivate::Disconnect()
{
	if (connectionHandle != BTSDK_INVALID_HANDLE)
	{
		BTINT32 result = BTSDK_FALSE;
		result = Btsdk_Disconnect(connectionHandle);
		if(result != BTSDK_OK)
			emit p_ptr->error(QBtObjectExchangeClient::OBEXClientConnectionError);
		else
			emit p_ptr->disconnectedFromServer();
	}
}

/**
* SendFile
* Send a file to a service on a remote machine
*/
void QBtObjectExchangeClientPrivate::SendFile(const QString& fileName)
{
	if (connectionHandle == BTSDK_INVALID_HANDLE)
	{	
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientDisconnected);
		return;
	}

	WIN32_FIND_DATA pFindData;
	HANDLE hFile;
	BTUINT32 iErrorCode;
	
	//in case QFileDialog is used (or an address is given in the following format), 
	// all '/' are changed to  '\'.
	QString fileNameEdited = fileName;
	fileNameEdited.replace("/", "\\");

	QString  fileNamePath = fileNameEdited.left( fileNameEdited.lastIndexOf("\\") );


//	SetPath("");
//	SetPath(fileNamePath);

//	ZeroMemory(ucCurrentLocalPath, BTSDK_PATH_MAXLENGTH);
//	strcpy((char*)ucCurrentLocalPath, (char*)localFilePath);

	Btsdk_FTPRegisterStatusCallback4ThirdParty(connectionHandle, 
											&QBtObjectExchangeClientPrivate::FTPStatusCallback);
											
	hFile = FindFirstFile(fileNameEdited.utf16(),&pFindData);

	QString fileNameWStr = QString::fromUtf16(pFindData.cFileName);

	iErrorCode = Btsdk_FTPPutFile(connectionHandle, 
									(BTUINT8*)fileNameEdited.toUtf8().constData(),
									(BTUINT8*)fileNameWStr.toUtf8().constData());
		
	Btsdk_FTPRegisterStatusCallback4ThirdParty(connectionHandle, NULL);

	if(iErrorCode == BTSDK_OK)
		emit p_ptr->objectSent();
	else
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientSendError);
}

/**
* SendData
* Send a byte array to a service on a remote machine
*/
void QBtObjectExchangeClientPrivate::SendData(const QByteArray& /*data*/, const QString& /*bufferName*/)
{
	return;
}

/**
* GetFile
* 
* Request from the server to GET the file defined in the argument
*/
void QBtObjectExchangeClientPrivate::GetFile(const QString& localPath, const QString& remoteFileNameFull)
{
	if (connectionHandle == BTSDK_INVALID_HANDLE)
		return;

	int slashPosition = remoteFileNameFull.lastIndexOf("\\");

	QString localPathStr = localPath;
	QString remoteFilePath = remoteFileNameFull.left(slashPosition+1);
	QString remoteFileName = remoteFileNameFull.right(remoteFileNameFull.size() - slashPosition - 1);

	BTUINT32 iErrorCode;

	Btsdk_FTPRegisterStatusCallback4ThirdParty(connectionHandle, &QBtObjectExchangeClientPrivate::FTPStatusCallback);	

	// go to route direcotry
	SetPath("");
	//bool remote = SetPath(remoteFilePath);
	
	//////////////////////////////////////////////////////////////////////////
	if(localPathStr.lastIndexOf("\\") != localPathStr.size() -1)
		localPathStr.append("\\");

	localPathStr += remoteFileName;
	//////////////////////////////////////////////////////////////////////////
	
	iErrorCode = Btsdk_FTPGetFile(connectionHandle, 
								(BTUINT8*)remoteFileNameFull.toUtf8().constData(), 
								(BTUINT8*)localPathStr.toUtf8().constData());

	Btsdk_FTPRegisterStatusCallback4ThirdParty(connectionHandle, NULL);

	if(iErrorCode == BTSDK_OK)
		emit p_ptr->fileReceived(QString(localPathStr));
	else
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientReceiveError);
		
	return;
}

/**
* GetByteBuffer()
* 
* Request from the server to GET the data buffer defined in the argument
*/
void QBtObjectExchangeClientPrivate::GetByteBuffer(const QString& /*dataName*/)
{
	return;
}

/**
* Abort
* Send the OBEX aborts command to the remote machine
*/
void QBtObjectExchangeClientPrivate::Abort()
{
	if(isBusy)
		Btsdk_FTPCancelTransfer(connectionHandle);
}

/**
* SetPath()
* Set the current path of the remote device OBEX session.
*/
bool QBtObjectExchangeClientPrivate::SetPath (const QString & path)
{
	if (connectionHandle == BTSDK_INVALID_HANDLE)
	{
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientDisconnected);
		return false;
	}

	BTINT32 iErrorCode;
	//BTUINT8 editedFolderName[BTSDK_PATH_MAXLENGTH];
	//memset(editedFolderName, '\0', BTSDK_PATH_MAXLENGTH);

	/*editedFolderName[0] = '\\';

	if(path.size() > 0)
		memcpy(editedFolderName+1, 
				path.constData(), 
				(path.size() < BTSDK_PATH_MAXLENGTH) ? 
						path.size() : 
						BTSDK_PATH_MAXLENGTH);*/
	QString pathEdited = path;
	pathEdited.prepend("\\");

	// first try if file is in relative path form
	iErrorCode = Btsdk_FTPSetRmtDir(connectionHandle, (BTUINT8*)pathEdited.toUtf8().constData());

	if(iErrorCode != BTSDK_OK)
	{
		// then try as absolute path, going first to root path and then setting 
		// the path given as argument
		Btsdk_FTPSetRmtDir(connectionHandle, NULL);
		iErrorCode = Btsdk_FTPSetRmtDir(connectionHandle, (BTUINT8*)pathEdited.toUtf8().constData());
	}

	if(iErrorCode == BTSDK_OK)
	{
		QString remotePath(GetRemoteWorkingDirectory());
		emit p_ptr->remotePathSet(remotePath);
		return true;
	}
	else
	{
		emit p_ptr->error(QBtObjectExchangeClient::OBEXClientUnableToSetPath);
		return false;
	}
}

/**
* IsConnected
* @return true if the client is connected
*/
bool QBtObjectExchangeClientPrivate::IsConnected()
{
	return (connectionHandle != BTSDK_INVALID_HANDLE);
}

/**
* IsBusy
* @return true if the client is performing some operation.
*/
bool QBtObjectExchangeClientPrivate::IsBusy()
{
	return isBusy;
}

QString QBtObjectExchangeClientPrivate::GetRemoteWorkingDirectory()
{
	if (connectionHandle == BTSDK_INVALID_HANDLE)
		return "";

	BTUINT8* remoteCurrentDir = new BTUINT8(maxFileNameSize);
	BTINT32 result = BTSDK_FALSE;
	
	ZeroMemory(remoteCurrentDir, maxFileNameSize);
	
	result = Btsdk_FTPGetRmtDir(connectionHandle, remoteCurrentDir);

	if(result == BTSDK_OK)
		return QString((char*)remoteCurrentDir);
	else
		return "";
}

/************************************************************************/
/*                     Transfer info callback                           */
/************************************************************************/
void QBtObjectExchangeClientPrivate::FTPStatusCallback(UCHAR ucFirst, 
					UCHAR ucLast, UCHAR* /*ucFileName*/, DWORD /*dwFilesize*/, DWORD /*dwCursize*/)
{
	static BTUINT32 s_TotalFileSize;
	static BTUINT32 s_CurrentFileSize;

	if (1 == ucFirst)
	{
	//	s_TotalFileSize = dwFilesize;
//		s_CurrentFileSize = 0;
		
		isBusy = true;
	}
	
//	printf("*******It is transfering file %s, and current size = %d****\n",
//							ucFileName,s_CurrentFileSize += dwCursize);
		    
	if (1 == ucLast)
	{
	//	printf("*******It has finished transfering file %s,and total size = %d ****\n",
	//		   ucFileName,s_TotalFileSize);
		
		isBusy = false;
	}
}
