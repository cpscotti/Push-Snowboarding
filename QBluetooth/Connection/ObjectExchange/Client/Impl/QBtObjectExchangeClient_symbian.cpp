/*
 * QBtObjectExchangeClient_symbian.cpp
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

#include <QFile>
#include <QTextStream>
#include "../QBtObjectExchangeClient_symbian.h"
#include <QBtAuxFunctions.h>

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::NewL()
// Symbian two-phased constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeClientPrivate* QBtObjectExchangeClientPrivate::NewL(
                                            QBtObjectExchangeClient* publicClass )
{
    QBtObjectExchangeClientPrivate* self = NewLC( publicClass );
    CleanupStack::Pop( self );
    return self;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::NewLC()
// Symbian two-phased constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeClientPrivate* QBtObjectExchangeClientPrivate::NewLC(
                                            QBtObjectExchangeClient* publicClass )
{
    QBtObjectExchangeClientPrivate* self = new ( ELeave ) QBtObjectExchangeClientPrivate(publicClass );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::QBtObjectExchangeClientPrivate()
// Constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeClientPrivate::QBtObjectExchangeClientPrivate( QBtObjectExchangeClient* publicClass )
            : CActive( CActive::EPriorityStandard ),
            iNewFile(NULL),
            iState( ENone ),
            iClient(NULL),
            iCurrObject(NULL),
            buffer(NULL),            
            p_ptr( publicClass )
            //server(NULL),
            //service(NULL),

{
    CActiveScheduler::Add( this );
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::~QBtObjectExchangeClientPrivate()
// Destructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeClientPrivate::~QBtObjectExchangeClientPrivate()
{
    Disconnect();

    SafeDelete(iCurrObject);
    SafeDelete(iClient);
    //SafeDelete(server);
    //SafeDelete(service);
    SafeDelete(buffer);
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::ConstructL()
// Perform second phase construction of this object.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::ConstructL()
{
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::DoCancel()
// Cancel any outstanding requests.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::DoCancel()
{

}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::RunL()
// Respond to an event.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::RunL()
{
    //ERespForbidden
    if ( iStatus == KErrDisconnected)
    {
    	QT_TRYCATCH_LEAVING (emit p_ptr->disconnectedFromServer() );
        if(IsActive())
            Cancel();
    }
    else if ( iStatus != KErrNone )
    {
        switch ( iState )
        {
            case EGettingConnection:
            case EDisconnecting:
            	QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtObjectExchangeClient::OBEXClientConnectionError) );
                break;
            case EWaitingToSend:
            	QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtObjectExchangeClient::OBEXClientSendError) );
                break;
            case EWaitingToReceiveData:
            case EWaitingToReceiveFile:
            	QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtObjectExchangeClient::OBEXClientReceiveError) );
                break;
            default:
                break;
        }
        //TInt statusId = iStatus.Int();
        return;
    }
    else
    {
        switch ( iState )
        {
            case EGettingConnection:
                HandleServerConnection();
                break;

            case EWaitingToSend:
                HandleObjectSent();
                break;

            case EWaitingToReceiveData:
            case EWaitingToReceiveFile:
                HandleObjectReceived();
                break;

            case EDisconnecting:
                HandleDisconnection();
                break;

            case EBrowsing:
                HandleBrowsingFinsished();
                break;

            case ESettingPath:
                HandleSettingPathCompletion();
                break;

            default:
                break;
        };
    }
}



//////////////
// Handles	//
//////////////

void QBtObjectExchangeClientPrivate::HandleServerConnection()
{
    iState = EWaitingToSend;
    QT_TRYCATCH_LEAVING (emit p_ptr->connectedToServer() );
}

void QBtObjectExchangeClientPrivate::HandleObjectSent()
{
    iState = EWaitingToSend;
    SafeDelete(iCurrObject); // delete temporaly object
    QT_TRYCATCH_LEAVING (emit p_ptr->objectSent() );
}

void QBtObjectExchangeClientPrivate::HandleObjectReceived()
{
    if(EWaitingToReceiveFile)
    {
        iState = EWaitingToSend;

        QString fileName = QString::fromUtf16(iCurrObject->Name().Ptr(),iCurrObject->Name().Size());
        QT_TRYCATCH_LEAVING (emit p_ptr->fileReceived(fileName) );

    /*	QString fileNameFullWithLocalPath = tmpLocalPath + fileName;
        QFile file(fileNameFullWithLocalPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        QString fileData = QString::fromUtf16(((CObexFileObject*)iCurrObject)->DataFile().Ptr());
        out << fileData;
        file.close();*/
    }
    else
    {
        iState = EWaitingToSend;
        if(!buffer)
            return;

        TUint8* tmpArray = new TUint8(buffer->Size());
        CleanupStack::PushL(tmpArray);
        TPtr8 data(tmpArray, buffer->Size());
        buffer->Read(0, data);

        QByteArray retArray((const char*)data.Ptr(), data.Size());
        delete tmpArray;

        QT_TRYCATCH_LEAVING (emit p_ptr->dataReceived(retArray) );
    }
}

void QBtObjectExchangeClientPrivate::HandleDisconnection()
{
    iState = ENone;
    QT_TRYCATCH_LEAVING (emit p_ptr->disconnectedFromServer() );
}

void QBtObjectExchangeClientPrivate::HandleBrowsingFinsished()
{
    iState = EWaitingToSend;
    return;
}

void QBtObjectExchangeClientPrivate::HandleSettingPathCompletion()
{
    iState = EWaitingToSend;
    return;
}



// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::ConnectToServer()
// Connect to the server.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::ConnectToServer(const QBtDevice& remoteDevice, const QBtService& remoteService)
{
    //server = new QBtDevice(remoteDevice);
    //service = new QBtService(remoteService);

    TObexBluetoothProtocolInfo protocolInfo;

    protocolInfo.iTransport.Copy( _L("RFCOMM") );
    protocolInfo.iAddr.SetBTAddr (remoteDevice.getAddress().convertToSymbianBtDevAddr());
    protocolInfo.iAddr.SetPort (remoteService.getPort());

    if(iClient)
    {
        //Disconnect();                 	
        delete iClient;
        iClient = NULL;        
    }

    iClient = CObexClient::NewL( protocolInfo );

    iState = EGettingConnection;
    iClient->Connect( iStatus );
    SetActive();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::SendFile()
// Send a message to a service on a remote machine.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::SendFile(const QString& fileName)
{
    if(!CheckServiceAvailability())
        return;

    iState = EWaitingToSend;

    //in case QFileDialog is used (or an address is given in the following format),
    // all '/' are changed to  '\'.
    QString fileNameEdited = fileName;
    fileNameEdited.replace("/", "\\");

    TPtrC16 aName = TPtrC16(fileNameEdited.utf16());

    SafeDelete(iCurrObject);
    iCurrObject = CObexFileObject::NewL(aName);

    TParse parse;
    parse.Set(aName, NULL, NULL);
    TPtrC ptr = parse.NameAndExt();

    iCurrObject->SetNameL( ptr );

    iClient->Put( *iCurrObject, iStatus );
    SetActive();
}



// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::Abort()
// Aborts command.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::Abort()
{
    if ( iClient && iClient->IsConnected() )
    {
        iClient->Abort();
        iState = EWaitingToSend;
    }
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::Disconnect()
// Disconnects from the remote machine.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeClientPrivate::Disconnect()
{
    if ( iState == ENone  || !iClient || iState == EGettingConnection)
        return;

    Abort();
    iState = EDisconnecting;
    iClient->Disconnect( iStatus );
    SetActive();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::IsBusy()
// True, if the client is performing some operation..
// ----------------------------------------------------------------------------
//
TBool QBtObjectExchangeClientPrivate::IsBusy()
{
    return IsActive();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::IsConnected()
// True, if the client is performing some operation..
// ----------------------------------------------------------------------------
//
TBool QBtObjectExchangeClientPrivate::IsConnected()
{
    return (iState != ENone && iState != EGettingConnection);
}

//if KErrArgument -> invalid path!!!
void QBtObjectExchangeClientPrivate::SetPath (const QString & path)
{
    if(!CheckServiceAvailability())
        return;

    iState = ESettingPath;

    // If you do not which to send a name
    if(path == "")
    {
        pathInfo.iNamePresent = EFalse;
    }
    else
    {
        TPtrC16 path16;
        path16.Set(path.utf16());
        pathInfo.iName = path16;
    }

    iClient->SetPath(pathInfo, iStatus);
    SetActive();
}

TBool QBtObjectExchangeClientPrivate::CheckServiceAvailability()
{
    if ( iState == ENone ||  iState == EGettingConnection)
    {
    	QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtObjectExchangeClient::OBEXClientDisconnected) );
        return EFalse;
    }
    else if ( IsBusy() )
    {
    	QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtObjectExchangeClient::OBEXClientInUse) );
        return EFalse;
    }
    else
        return ETrue;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeClientPrivate::SendData
// Send a byte array to a service on a remote machine
// ----------------------------------------------------------------------------
void QBtObjectExchangeClientPrivate::SendData(const QByteArray& data, const QString& bufferName)
{
    if(!CheckServiceAvailability())
            return;

    iState = EWaitingToSend;

    SafeDelete(buffer);
    SafeDelete(iCurrObject);

    TPtrC16 aName = TPtrC16(bufferName.utf16());

    TPtrC8 data8;
    data8.Set((TUint8*)data.constData(), data.size());
    buffer = CBufFlat::NewL(data.size());
    buffer->Write(0, data8, data8.Size());

    iCurrObject = CObexBufObject::NewL(buffer);
    iCurrObject->SetNameL(aName);
    iClient->Put(*iCurrObject, iStatus);
    SetActive();
}

void QBtObjectExchangeClientPrivate::GetFile(const QString& localPath, const QString& remoteFileNameFull)
{
    if(!CheckServiceAvailability())
            return;

    iState = EWaitingToReceiveFile;

    tmpLocalPath = localPath;
    if(tmpLocalPath.size() == 0)
    {
        tmpLocalPath = "c:\\data\\";
    }
    else if(tmpLocalPath.lastIndexOf("\\") != tmpLocalPath.size() - 1)
    {
        tmpLocalPath.append("\\");
    }


    TPtrC16 fullName = TPtrC16(remoteFileNameFull.utf16());

    SafeDelete(iCurrObject);
    iCurrObject = CObexFileObject::NewL();
    iCurrObject->SetNameL( fullName );

    iClient->Get( *iCurrObject, iStatus );
    SetActive();
}

void QBtObjectExchangeClientPrivate::GetByteBuffer(const QString& dataName)
{
    if(!CheckServiceAvailability())
        return;

    iState = EWaitingToReceiveData;

    SafeDelete(buffer);
    SafeDelete(iCurrObject);

    TPtrC16 aName = TPtrC16(dataName.utf16());

    buffer = CBufFlat::NewL(256);
    iCurrObject = CObexBufObject::NewL(buffer);
    iCurrObject->SetNameL(aName);
    iClient->Get(*iCurrObject, iStatus);
    SetActive();

}
