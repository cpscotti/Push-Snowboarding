/*
 * QBtSerialPortClient_symbian.cpp
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

#include "../QBtSerialPortClient_symbian.h"
#include <QBtAuxFunctions.h>
#include "utf.h"

#include <QDebug>

// protocol constants
_LIT(KStrRFCOMM, "RFCOMM");



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

QBtSerialPortClientPrivate* QBtSerialPortClientPrivate::NewL(QBtSerialPortClient* publicClass)
{
    QBtSerialPortClientPrivate* self = QBtSerialPortClientPrivate::NewLC(publicClass);
    CleanupStack::Pop(self);
    return self;
}


QBtSerialPortClientPrivate* QBtSerialPortClientPrivate::NewLC(QBtSerialPortClient* publicClass)
{
    QBtSerialPortClientPrivate* self = new (ELeave) QBtSerialPortClientPrivate(publicClass);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}


void QBtSerialPortClientPrivate::ConstructL()
{
    if (iSocketServ.Connect() != KErrNone)
       QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortClient::ErrorUnavailable) );
    
    // timer
    iTimer = CPeriodic::NewL (CIdle::EPriorityIdle);
    
}


QBtSerialPortClientPrivate::QBtSerialPortClientPrivate(QBtSerialPortClient* publicClass)
 : CActive(CActive::EPriorityStandard),
   iState(ENone),
   p_ptr(publicClass),
   iMessage (0)
{
    CActiveScheduler::Add(this);    
}


QBtSerialPortClientPrivate::~QBtSerialPortClientPrivate()
{
    Disconnect();
    
    // delete connect timer
    CancelConnectionTimer();
    delete iTimer;
    
    // disconnect and kill socket
    if(IsActive())
       Cancel();

    
    //
    if(iMessage)
        delete iMessage;
}


// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::ConnectL(THostName aName, TBTDevAddr aAddr, TInt aPort)
//
// create a connection to given address on given port.
// ----------------------------------------------------------------------------
TBool QBtSerialPortClientPrivate::ConnectL (const QBtDevice& remoteDevice, const QBtService& remoteService)
{		
    if (iState != ENone)        
    { DEBUG_MSG("warning: trying to connect and state != ENone") return false; }

    device =  remoteDevice;
    service = remoteService;

    // load protocol, RFCOMM
    //TProtocolDesc pdesc;
    //User::LeaveIfError(iSocketServ.FindProtocol(_L("RFCOMM"), pdesc));

    
    // see doc: NOTE: Deprecated default connection scenario
    
    // open socket
    User::LeaveIfError(iSock.Open(iSocketServ, KStrRFCOMM));
    
    // set address and port
    TBTSockAddr addr;
    addr.SetBTAddr(device.getAddress().convertToSymbianBtDevAddr());
    addr.SetPort(service.getPort());
    
    
    // what about these fields?
    //TBTServiceSecurity sec;
    //sec.SetAuthentication(ETrue);
    //sec.SetEncryption(ETrue);
    //addr.SetSecurity(sec);   
    
    
    _addr = addr;

    // connect socket
    TRequestStatus status;
    iSock.Connect(addr, status);  
    
    
    iState = EConnecting;

    //Initializing QOS vars
    connFailureCnt = 0;
    pipeDepth = 0;
    
    // start connection timer    
    StartConnectionTimer();
    
            
    // wait (should we wait here? or use the ActiveObject RunL ?)
    User::WaitForRequest(status);
    
    
    // cancel timers    
    CancelConnectionTimer();
    
    
    if (status != KErrNone)
    {
        DEBUG_MSG (QString ("[connect] error detected: %1").arg (status.Int()) )
    	
    	iState = ENone;
    	
    	switch (status.Int())
    	{
    		case KErrTimedOut:
    		case KErrCancel:
    			QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortClient::ErrorConnectionTimeout) );
    			break;
    			
    		case KErrAlreadyExists:
                        QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortClient::ErrorAlreadyConnected) );
                        break;
    			
    		case KErrCouldNotConnect:
    		default:
    			QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortClient::ErrorOpeningConnection) );
    	
    	}
    	
        return false;
    }

       
    // prepare for receive
    ReceiveData();
   
    // notify
    QT_TRYCATCH_LEAVING (emit p_ptr->connectedToServer() )
    
        
    //SetActive();   
    return true;    
}


// ----------------------------------------------------------------------------
// callback for notifying connection timeout 
//
// ----------------------------------------------------------------------------

TInt QBtSerialPortClientPrivate::ConnectTimerCallBack (TAny* aPtr)
{
        DEBUG_MSG ("[timer] entering")
	
	QBtSerialPortClientPrivate* p = (QBtSerialPortClientPrivate*) aPtr;


	// ignore if connection was successful
	if (p->iState != EConnecting)
	{
            DEBUG_MSG ("[timer] got connection, ignoring timeout event")
                    return EFalse;
        }
	
        DEBUG_MSG ("[timer] canceling ActiveObject")
	
	
	if (p->IsActive())
		p->Cancel();
	
	// change state
	p->iState = ENone;
	
        DEBUG_MSG ("[timer] emit error signal")
	
	// emit error
        QT_TRYCATCH_LEAVING (emit p->p_ptr->error (QBtSerialPortClient::ErrorConnectionTimeout))
	
        DEBUG_MSG ("[timer] end")
	
	return EFalse;	
}


void QBtSerialPortClientPrivate::StartConnectionTimer()
{	
	// 30s interval (this should be a parameter)
        TTimeIntervalMicroSeconds32 interval (30 * 1000*1000);
	
	if (iTimer)
		iTimer->Start (interval, interval, TCallBack (ConnectTimerCallBack, this) );	
}

void QBtSerialPortClientPrivate::CancelConnectionTimer()
{
	if (iTimer)
		iTimer->Cancel();
}

// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::Disconnect()
//
// disconnect from remote device, shutdown connected socket
// ----------------------------------------------------------------------------
void QBtSerialPortClientPrivate::Disconnect()
{   	
    if (iState == ENone || iState == EDisconnecting)
        return;

    iState = EDisconnecting;

    
    // cancel timers
    CancelConnectionTimer();
    
    
    // shutdown socket
    TRequestStatus status;
    
    qDebug() << "Disconnecting for real!";
    // cancel all
    iSock.CancelAll();
    qDebug() << "\t...All canceled, " << connFailureCnt;
    if(connFailureCnt > 0) {
        iSock.Shutdown(RSocket::EImmediate, status);
        qDebug() << "\t...All shut down (immediate)";
    } else {
        iSock.Shutdown(RSocket::ENormal, status);
        qDebug() << "\t...All shut down (normal)";
    }
    User::WaitForRequest(status);
    qDebug() << "\t...WaitForRequest";
    iSock.Close();
    qDebug() << "Closed";

    iState = ENone;
    
    // check 
    if (status == KErrNone)
    {    	
        QT_TRYCATCH_LEAVING (emit p_ptr->disconnectedFromServer())
    }
    else
    {
        QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtSerialPortClient::ErrorOnDisconnecting))
    }        
    
        
    //iState = EDisconnecting;    
    // SetActive();
    // notification will be in RunL with KErrCancel (in case of using SetActive)
    
}

TBool QBtSerialPortClientPrivate::IsConnected()
{
    return (iState == ESending || iState == EWaiting);
}

// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::ReceiveDataL()
//
// wait for and receive data from remote device, read connected socket
// ----------------------------------------------------------------------------
void QBtSerialPortClientPrivate::ReceiveData()
{
    // cancel pending operations
    iSock.CancelRead();
    if(IsActive())
        Cancel();
    
    // receive data from socket
    iState=EWaiting;
    iSock.RecvOneOrMore(iBuffer, 0, iStatus, iLen);
    SetActive();
}


// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::SendData(const QString& data)
//
// send given data to remote device, write to connected socket
// ----------------------------------------------------------------------------

void QBtSerialPortClientPrivate::SendData (const QString& data)
{
	QByteArray array = data.toUtf8();	
	SendData (array);
}

// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::SendData(const QByteArray& data)
//
// send given data to remote device, write to connected socket
// ----------------------------------------------------------------------------
void QBtSerialPortClientPrivate:: SendData (const QByteArray& data)
{

    //QOS pipeDepth Check. pipeDepth tells the number of sent messages waiting to "get through".
    //When this becomes bigger than a certain threshold, we assume the bt connection was lost.
    if(pipeDepth > maxPipeDepth) {
        connFailureCnt++;
        qDebug() << this << "Signal lost (Full pipe)! (" << pipeDepth << ")";
//        QT_TRYCATCH_LEAVING (emit p_ptr->connectionResetByPeer())
        QT_TRYCATCH_LEAVING(emit p_ptr->lost_signal())

        //Note that this won't for disconnection of the device, it will just signal the app to stop trying
        //to get messages through otherwise the send buffer would get too big and we would get locked:
        //DoCancel gets blocked on ESending
        return;
    }

    TPtrC8 desc8 (reinterpret_cast<const TText8*> (data.constData()), data.size());

    // delete previous data
    if (iMessage)
        delete iMessage;

    iMessage = HBufC8::NewL(desc8.Length());
    iMessage->Des().Copy(desc8);
	
    // cancel any read requests on socket
    iSock.CancelRead();

    if (IsActive())
    {
        Cancel();
    }
    
    // send message
    iState = ESending;    

    //QOS pipeDepth increment, will send new message.
    //pipeDepth will be decremented when message gets through
    pipeDepth++;

    iSock.Write (*iMessage, iStatus);
    SetActive();
}

void QBtSerialPortClientPrivate::DoCancel()
{
    switch(iState)
    {
    case EWaiting:
        break;
    case ESending:
        //Should never be executed from sendData's Cancel!!!
        qDebug() << "(DoCancel:ESending)";
        iSock.CancelSend();
        break;

        //ENone when exit, can't call iSock.CancelAll or so will panic
    default:
        break;

    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void QBtSerialPortClientPrivate::RunL()
{
    //DEBUG_MSG (QString ("[RunL status: %1 state: %2]").arg (iStatus.Int()).arg (iState) );

    // cancel possible timers
    CancelConnectionTimer();
	
	
	// see what's happened
    if (iStatus != KErrNone)
    {
    	switch (iStatus.Int() )
    	{    		    		
    		// we were disconnected (by us)    		
    		case KErrCancel:
    		{    			
    			iState = ENone;    			
    			// ignore this signal for now as Disconnect() is using WaitForRequest
    			// QT_TRYCATCH_LEAVING (emit p_ptr->disconnectedFromServer() );
    			
    			return;
    		}
    		
    			
    		// we were disconnected by peer	
    		case KErrDisconnected:
    		{
                        qDebug() << "Will emit connected reset by peer";
    			iState = ENone;
                        QT_TRYCATCH_LEAVING (emit p_ptr->connectionResetByPeer())
    			return;
    		}
    			
    			
    		// not ready to initiate new operation (something is still underway)
    		case KErrNotReady:
    		{	
    			// no change in current state (?)    			
                        QT_TRYCATCH_LEAVING (emit p_ptr->error (QBtSerialPortClient::ErrorAlreadyInUse))
    			return;
    		}
    		
    	}
    	
    	
    	
        DEBUG_MSG (QString ("[RunL error: %1]").arg (iStatus.Int()))
    	    	
    	// get the error
    	switch (iState)
    	{
    		case EConnecting:
    		{
                        QT_TRYCATCH_LEAVING (emit p_ptr->error (QBtSerialPortClient::ErrorOpeningConnection) )
    			break;
    		}	
    		
    		case EWaiting:
    		case ESending:
    		{
                        QT_TRYCATCH_LEAVING (emit p_ptr->error (QBtSerialPortClient::ErrorConnectionError))
    			break;
    		}
    		
    		default:
                        QT_TRYCATCH_LEAVING (emit p_ptr->error (QBtSerialPortClient::ErrorUndefinedError))
    			break;
    			
    			
    	}   	
    	
    	iState = ENone; 
        return;
    }

    
    
    // no error
    switch (iState)
    {
    	// notification should be the last thing because the user might change the state of the
    	// object, for example, by calling SendData() in a slot for the 'connectedToServer' signal
    	
        case EConnecting:
        {	
        	// this one would run only if SetActive() is called in connect()        	
        	iState = EIdle;
        	
        	// wait incoming data on socket
            ReceiveData();
        	
        	// notify
                QT_TRYCATCH_LEAVING (emit p_ptr->connectedToServer() )
            break;
        }
        
        case EWaiting:
        {
            //QOS pipeDepth check. When a msg gets through, BtState goes back to waiting.
            //If no messages get through for a long time, pipeDepth will overflow
            if(pipeDepth > 0) {
                pipeDepth--;

                //Pipe empty again, issue signal_is_back
                if(pipeDepth == 0 && connFailureCnt > 0) {
                    QT_TRYCATCH_LEAVING (emit p_ptr->signal_is_back() )
                    qDebug() << this << "Signal is back!";
                    connFailureCnt = 0;
                }
            }

            if(connFailureCnt > 0) {
                qDebug() << "We got a waiting after full pipe!";
            }

            // we got incoming data!
            QString receivedString = QString::fromUtf8((char*)iBuffer.Ptr(), iBuffer.Size());
            
//            DEBUG_MSG (receivedString);

            // start expecting new incoming data
            ReceiveData();
            
            // notify
            QT_TRYCATCH_LEAVING (emit p_ptr->dataReceived (receivedString) )
            break;
        }
        case ESending:
        {                	
            // start expecting new incoming data
            ReceiveData();
            
            // notify
            QT_TRYCATCH_LEAVING  (emit p_ptr->dataSent())
            break;
        }        
                
        default:         
        	break;
    }
}

TInt QBtSerialPortClientPrivate::RunError(TInt /*aError*/)
{
    // Add error handling here, not implemented in this example
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtSerialPortClientPrivate::HandleConnectorDataReceivedL(TDesC& aData)
//
// connector has received data signal
// ----------------------------------------------------------------------------
void QBtSerialPortClientPrivate::HandleConnectorDataReceivedL(const QString & aData)
{
	
}

/*!
 * HandleListenerConnected()
 *
 * Handles the event of server connection
 */
void QBtSerialPortClientPrivate::HandleConnectorConnectedL()
{
	
}

/*!
 * HandleListenerDisconnected()
 *
 * Handles the event of server disconnection
 */
void QBtSerialPortClientPrivate::HandleConnectorDisconnectedL()
{

}

/*!
 * HandleConnectorDataSentL()
 *
 * Data successfully sent to server
 */
void QBtSerialPortClientPrivate::HandleConnectorDataSentL()
{
	
}

