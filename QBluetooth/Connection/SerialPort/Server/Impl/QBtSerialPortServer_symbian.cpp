/*
 * QBtSerialPortServer_symbian.cpp
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

#include "../QBtSerialPortServer_symbian.h"

QBtSerialPortServerPrivate* QBtSerialPortServerPrivate::NewL(QBtSerialPortServer* publicClass)
{
	QBtSerialPortServerPrivate* self = QBtSerialPortServerPrivate::NewLC(publicClass);
	CleanupStack::Pop(self);
	return self;
}


QBtSerialPortServerPrivate* QBtSerialPortServerPrivate::NewLC(QBtSerialPortServer* publicClass)
{
	QBtSerialPortServerPrivate* self = new (ELeave) QBtSerialPortServerPrivate(publicClass);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}


void QBtSerialPortServerPrivate::ConstructL()
{
	if(iSocketServ.Connect() != KErrNone)
		emit p_ptr->error(QBtSerialPortServer::ErrorUnavailable);
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::QBtSerialPortServerPrivate(QBtSerialPortServer* publicClass)
//
// constructor
// ----------------------------------------------------------------------------
QBtSerialPortServerPrivate::QBtSerialPortServerPrivate(QBtSerialPortServer* publicClass):
	CActive(CActive::EPriorityStandard),
	iIsConnected(EFalse),
	queueSize(4),
	iState(ENone),		
	restartAtDisconnect(false),
	p_ptr(publicClass)
{
	CActiveScheduler::Add(this);
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::~QBtSerialPortServerPrivate()
//
// destructor
// ----------------------------------------------------------------------------
QBtSerialPortServerPrivate::~QBtSerialPortServerPrivate()
{
	// close sockets, must not throw
	TRAPD (error, StopListenerL() );
	
	// cancel active object
	if(IsActive())
		Cancel();
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::StartListener()
//
// start listener.  listener will open a listening socket on a channel (port)
// gotten from GetOpt() call.  
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::StartListenerL()
{
	TInt aChannel;
	
	// get out if we're already running..
	if (iState != ENone)
	{
		QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortServer::ErrorAlreadyInUse) );
		return;
	}

	// set this active object to connecting state
	iState=EConnecting;

	// load protocol, RFCOMM
	TProtocolDesc pdesc;
	User::LeaveIfError (iSocketServ.FindProtocol(_L("RFCOMM"), pdesc));

	// open a socket
	User::LeaveIfError (iListenSock.Open (iSocketServ, pdesc.iAddrFamily,pdesc.iSockType,QBtConstants::RFCOMM) );

	// get listening channel
	User::LeaveIfError (iListenSock.GetOpt(KRFCOMMGetAvailableServerChannel, KSolBtRFCOMM, aChannel));

	// bluetooth socket address object
	TBTSockAddr btsockaddr;
	btsockaddr.SetPort(aChannel);
	
	// bind socket
	User::LeaveIfError (iListenSock.Bind(btsockaddr));
	// listen on port
	iListenSock.Listen (queueSize);
	
	// advertise service
	QBtService newService;
	newService.setName(p_ptr->_service->getName());
	newService.setClass(QBtConstants::SerialPort);
    newService.setPort(aChannel);
	newService.addProtocol(QBtConstants::L2CAP);
	newService.addProtocol(QBtConstants::RFCOMM);
	
	p_ptr->startAdvertisingService(newService);

	// close old accepted socket if open
	iSock.Close();

	// open blank socket and pass it to accept to be assigned a proper
	// socket upon completion of Accept()
	User::LeaveIfError(iSock.Open(iSocketServ));

	// set to accept incoming connections, active object will handle
	iListenSock.Accept(iSock,iStatus);
	
	if(!IsActive())
		SetActive();
	
	QT_TRYCATCH_LEAVING (emit p_ptr->serverStarted() );
}

// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::StopListener()
//
// stops the listener by closing the listening socket
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::StopListenerL()
{
	// kill sockets
	if (iState != ENone)
	{       
		// cancel all pending socket operations
		iSock.CancelAll();
		iListenSock.CancelAll();
		
		iSock.Close();
		iListenSock.Close();
		iState=ENone;
		
		p_ptr->stopAdvertisingService();
		
		QT_TRYCATCH_LEAVING (emit p_ptr->serverStopped() );
	}
}

// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::SendData(const QString data)
//
// send data to remote device, write data to socket.
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::SendData(const QString & data)
{
	if ( iState!=EWaiting )
		return;
	
	QByteArray tmpArray = data.toUtf8();
	TPtrC8 message8;
	message8.Set((const TUint8 *)tmpArray.constData(), tmpArray.size());
	
	// cancel any read requests on socket
	if(IsActive())
	{
		iSock.CancelAll();
		iListenSock.CancelAll();
		Cancel();
	}
	// try to send message by writing to socket
	// - set the state of this active object to "sending"
	iState=ESending;
	
	// - make async socket write request
	iSock.Write(message8, iStatus);
	
	// - start waiting async req response (iState) from active scheduler
	SetActive();
}

// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::ReceiveData()
//
// receive more data from listening socket, asynchronously.  
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::ReceiveData()
{
	// set state to waiting - for RunL()
	iState = EWaiting;
	
	// make async request
	iSock.RecvOneOrMore(iBuffer, 0, iStatus, iLen);
	
	// set as active to get the async req response (iState) in RunL()
	SetActive();
}



void QBtSerialPortServerPrivate::RunL()
{
	if ( iStatus!=KErrNone )
	{
		StopListenerL();
		HandleListenerDisconnectedL();
		
		if(restartAtDisconnect)
			StartListenerL();
		
		return;
	}

	switch (iState)
	{
		case EConnecting:
		{
			// connected listening socket!
			HandleListenerConnectedL();
			ReceiveData();
			break;
		}
		case EWaiting:
		{
			// returned from receiving data
			//HBufC8* text = HBufC8::NewLC(iBuffer.Length());
			//text->Des().Copy(iBuffer);
			// observer will handle data
			QString receivedString = QString::fromUtf8((char*)iBuffer.Ptr(), iBuffer.Size());
			HandleListenerDataReceivedL(receivedString);
			//CleanupStack::PopAndDestroy(text);
			// start expecting next data to be read
			ReceiveData();
			break;
		}
		case ESending:
		{
			// returned from sending the date, check the state
			// start expecting next data to be read
			HandleListenerDataSendL();
			ReceiveData();
			break;
		}
		default:
			break;
	}
}

TInt QBtSerialPortServerPrivate::RunError(TInt /*aError*/)
{
	// add the error handling
	return KErrNone;
}


void QBtSerialPortServerPrivate::DoCancel()
{

}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::IsConnected()
//
// returns true if listener has been connected to
// ----------------------------------------------------------------------------
TBool QBtSerialPortServerPrivate::IsConnected()
{
	return iIsConnected;
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::HandleListenerDataReceivedL(TDesC& aData)
//
// emit signal to indicate successfull data receiving
// @param the data received
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::HandleListenerDataReceivedL(const QString & aData)
{
	//QString dataStr = QString::fromUtf8((char*)aData.Ptr(), aData.Length());
	QT_TRYCATCH_LEAVING (emit p_ptr->dataReceived(aData) );
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::HandleListenerConnectedL()
//
// emit signal that a client was successfully connected
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::HandleListenerConnectedL()
{
	TBTSockAddr sockAddr;	
	iSock.RemoteName(sockAddr);
	TBTDevAddr devAddr(sockAddr.BTAddr().Des());
	QBtAddress clientAddress(devAddr);
		
	iIsConnected=ETrue;
	QT_TRYCATCH_LEAVING  (emit p_ptr->clientConnected(clientAddress) );
}


// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::HandleListenerDisconnectedL()
//
// emit signal that client disconnected
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::HandleListenerDisconnectedL()
{
	iIsConnected=EFalse;
	QT_TRYCATCH_LEAVING (emit p_ptr->clientDisconnected() );
}

// ----------------------------------------------------------------------------
// QBtSerialPortServerPrivate::HandleListenerDataSend()
//
// emit signal that client sent successfully data to the server
// ----------------------------------------------------------------------------
void QBtSerialPortServerPrivate::HandleListenerDataSendL()
{
	QT_TRYCATCH_LEAVING  (emit p_ptr->dataSent() );
}

void QBtSerialPortServerPrivate::SetListeningQueueSize(TInt size)
{
	if(size > 0)
		queueSize = size;
}

/*
void SetSecurity()
{
	//now set security

	//old way to set security is in Listener.cpp
	//QBtSerialPortServerPrivate::SetSecurityL(TInt aChannel)
	TBTServiceSecurity secSettings;    

	TUid settingsUID;
	settingsUID.iUid = KBT_serviceID;
	secSettings.SetUid(settingsUID);
	//the old way involved the following two steps:
	//secSettings.SetChannelID(aChannel);
	//secSettings.SetProtocolID(KSolBtRFCOMM);
	secSettings.SetAuthentication(EFalse);
	secSettings.SetAuthorisation(EFalse);
	secSettings.SetEncryption(EFalse);

	// attach the security settings.
	btsockaddr.SetSecurity(secSettings);
}	
*/
