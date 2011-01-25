/*
 * QBtObjectExchangeServer_symbian.cpp
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

// INCLUDE FILES
#include <es_sock.h>

#include "../QBtObjectExchangeServer_symbian.h"
#include <QBtAuxFunctions.h>

#include <btmanclient.h>
#include <bt_sock.h> // New for 8.0

#include <bautils.h>
#include <eikenv.h>

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::NewL()
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeServerPrivate* QBtObjectExchangeServerPrivate::NewL(
        QBtObjectExchangeServer* publicClass )
{
    QBtObjectExchangeServerPrivate* self = QBtObjectExchangeServerPrivate::NewLC( publicClass );
    CleanupStack::Pop( self );
    return self;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::NewLC()
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeServerPrivate* QBtObjectExchangeServerPrivate::NewLC(
        QBtObjectExchangeServer* publicClass)
{
    QBtObjectExchangeServerPrivate* self = new ( ELeave ) QBtObjectExchangeServerPrivate( publicClass );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::QBtObjectExchangeServerPrivate()
// Constructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeServerPrivate::QBtObjectExchangeServerPrivate (QBtObjectExchangeServer* publicClass )
	:  iObexServer (NULL),
	   iObexBufData (NULL),
	   iTempFile (NULL),
	   iObexBufObject (NULL),
	   iObexBufObjectTransmit (NULL),
	   queueSize (4),
	   bufferGranularity (1024),	   
	   currentDir (""),
	   p_ptr (publicClass)
{
}

// -----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::ConstructL()
{
    // create file server session
    User::LeaveIfError(iFs.Connect());

    // Create temporarily file
    iTempFile = KTempFile().AllocL();
    //Make sure the file directory exists
    TFileName file = KTempFile();
    TParsePtr parsePtr ( file );
    TFileName filename = parsePtr.DriveAndPath();
    BaflUtils::EnsurePathExistsL( CEikonEnv::Static()->FsSession(), filename);
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::~QBtObjectExchangeServerPrivate()
// Destructor.
// ----------------------------------------------------------------------------
//
QBtObjectExchangeServerPrivate::~QBtObjectExchangeServerPrivate()
{
    if ( iObexServer && iObexServer->IsStarted() )
        iObexServer->Stop();

    SafeDelete(iObexServer);
    //This is used for temp purposes, see TransportDownIndication
    SafeDelete(iObexBufData);
    SafeDelete(iObexBufObject);
    SafeDelete(iTempFile);
    SafeDelete(iObexBufObjectTransmit);

    // parent class variables
    SafeDelete(p_ptr->_service);

    iFs.Delete( *iTempFile ); //Destroy temp file
    iFs.Close();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::InitialiseServerL()
// Initialises the server.
// ----------------------------------------------------------------------------
//
TBool QBtObjectExchangeServerPrivate::InitialiseServerL()
{
    if ( iObexServer )
    {
        emit p_ptr->error(QBtObjectExchangeServer::QObexServerInUse);
        return false;
    }

    // Set the Socket's security with parameters,
    // Authentication, Encryption, Authorisation and Denied
    // Method also return the channel available to listen to.
    TInt channel ( SetSecurityWithChannelL( EFalse, EFalse, ETrue, EFalse ) );

    // start the OBEX server
    TObexBluetoothProtocolInfo obexProtocolInfo;
    obexProtocolInfo.iTransport.Copy( KObexRfcommProtocol );
    obexProtocolInfo.iAddr.SetPort( channel );

    QBtService newService;
    newService.setClass(QBtConstants::OBEXObjectPush); // or FTP
    newService.addProtocol(QBtConstants::L2CAP);
    newService.addProtocol(QBtConstants::RFCOMM);
    newService.addProtocol(QBtConstants::OBEX);
    newService.setName(p_ptr->_service->getName());
    newService.setPort(channel);

    p_ptr->startAdvertisingService(newService);

    iObexServer = CObexServer::NewL( obexProtocolInfo );
    iObexServer->Start( this );

    return true;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::StartServer()
// Starts the server.
// ----------------------------------------------------------------------------
//
TBool QBtObjectExchangeServerPrivate::StartServer()
{
    TRAPD( err,InitialiseServerL() );

    if ( err != KErrNone )
    {
        emit p_ptr->error(QBtObjectExchangeServer::QObexServerFailedToInitialize);
        Disconnect();

        return false;
    }
    else
    {
        TParsePtrC parsePtr ( iTempFile->Des() );
        TPtrC dir = parsePtr.DriveAndPath();
        currentDir = QString::fromUtf16(dir.Ptr(), dir.Size());
        return true;
    }
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::Disconnect()
// Disconnects the server.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::Disconnect()
{
    if ( iObexServer && iObexServer->IsStarted() )
        iObexServer->Stop();


    SafeDelete(iObexServer);

    p_ptr->stopAdvertisingService();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::SetSecurityWithChannelL()
// Sets the security on the channel port and returns the available port.
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::SetSecurityWithChannelL( TBool aAuthentication,
        TBool aEncryption,
        TBool aAuthorisation,
        TBool aDenied )

{

    // Local variable channel to listen to.
    TInt channel = 0;

    RSocketServ socketServer;

    // Connect to SocetServer
    User::LeaveIfError( socketServer.Connect() );
    CleanupClosePushL( socketServer );

    RSocket socket;
    // Open the Socket connection
    User::LeaveIfError( socket.Open( socketServer, KObexRfcommProtocol ) );  // "RFCOMM"
    CleanupClosePushL( socket );

    // Retreive to one channel that is available.
    User::LeaveIfError( socket.GetOpt( KRFCOMMGetAvailableServerChannel,KSolBtRFCOMM, channel ) );

    // Set the Socket's Port.
    TBTSockAddr sockaddr;
    sockaddr.SetPort( channel );

    // Bind and start listeing the port with security setted,
    User::LeaveIfError(socket.Bind(sockaddr));
    User::LeaveIfError(socket.Listen(queueSize ) );

    // setup channel security
    /*TRequestStatus status;
    RBTMan secManager;

    RBTSecuritySettings secDatabase;
    TBTServiceSecurity secSettings;
    // connect to security manager
    User::LeaveIfError(secManager.Connect());
    CleanupClosePushL(secManager);
    User::LeaveIfError(secDatabase.Open(secManager));
    CleanupClosePushL(secDatabase);
    // setup security
    TUid settingsUID;
    settingsUID.iUid = KServiceClass;
    secSettings.SetUid(settingsUID);
    secSettings.SetChannelID(channel);
    secSettings.SetProtocolID(KSolBtRFCOMM);
    secSettings.SetAuthentication(aAuthentication);
    secSettings.SetAuthorisation(aAuthorisation);
    secSettings.SetEncryption(aEncryption);
    secSettings.SetDenied(aDenied);
    // register settings with security
    secDatabase.RegisterService(secSettings, status);
    User::WaitForRequest(status);
    CleanupStack::PopAndDestroy(2,&secManager);//SecDatabase, secManager
    */

    // now close the socket and the socket server
    CleanupStack::PopAndDestroy();  //  socket
    CleanupStack::PopAndDestroy();  //  socketServer

    return channel;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::IsConnected()
// Results true if the server is connected.
// ----------------------------------------------------------------------------
//
TBool QBtObjectExchangeServerPrivate::IsConnected()
{
    return iObexServer != NULL;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::SetListeningQueueSize()
// Set the size of the listening queue of the server.
// @param size
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::SetListeningQueueSize(TInt size)
{
    if(size > 0)
        queueSize = size;
}

QString QBtObjectExchangeServerPrivate::GetCurrentWorkingDirectory()
{
    return currentDir;
}

////////////////////////////////////////////
////	Obex Server Notify Handles		////
////////////////////////////////////////////

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::ErrorIndication()
// Receive error indication.
// It is only called for fatal errors that cause the OBEX connection to terminate.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::ErrorIndication( TInt aError )
{
    switch(aError)
    {
        case KErrIrObexClientPutPeerAborted:
            emit p_ptr->error(QBtObjectExchangeServer::QObexServerClientAbortedTransfer);
            break;

        case KErrIrObexServerPutPeerAborted:
            emit p_ptr->error(QBtObjectExchangeServer::QObexServerAbortedTransmission);
            break;

        default:
            emit p_ptr->error(QBtObjectExchangeServer::QObexServerSessionError);

    }

    Disconnect();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::TransportUpIndication()
// Called when the underlying socket transport connection is made from
// a remote client to the server
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::TransportUpIndication()
{
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::TransportDownIndication()
// Transport connection is dropped.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::TransportDownIndication()
{
    if ( iObexBufObject )
    {
        //transfer cancelled, set temporary
        //buffer so obexobject doesn't create the file again
        SafeDelete(iObexBufData);

        try{
            QT_TRAP_THROWING(
            {
                iObexBufData = CBufFlat::NewL( bufferGranularity );
                iObexBufObject->SetDataBufL( iObexBufData );
            });
        }
        catch(char* str){}
    }

    if ( iTempFile )
        iFs.Delete( *iTempFile );
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::ObexConnectIndication()
// Invoked when an OBEX connection is made from a remote client.
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::ObexConnectIndication(
        const TObexConnectInfo&, const TDesC8& )
{
    emit p_ptr->serverStarted();
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::ObexDisconnectIndication()
// OBEX server has been disconnected.
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::ObexDisconnectIndication( const TDesC8& /*aInfo*/ )
{
    emit p_ptr->serverStopped();
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::PutRequestIndication()
// ----------------------------------------------------------------------------
//
CObexBufObject* QBtObjectExchangeServerPrivate::PutRequestIndication()
{
    SafeDelete(iObexBufObject);
    iFs.Delete( *iTempFile );

    //Can't leave here
    try{
        QT_TRAP_THROWING({
                iObexBufObject = CObexBufObject::NewL( NULL );
                //The file is created if it doesn't exist
                iObexBufObject->SetDataBufL( *iTempFile );
                objectSizeBuffer = iObexBufObject->Length();
        });
    }
    catch(char* str)
    {
        p_ptr->error(QBtObjectExchangeServer::QObexServerSessionPutRequestProblem);
        return NULL; // request denied
    }

    emit p_ptr->receivingStarted();
    return iObexBufObject;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::PutPacketIndication()
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::PutPacketIndication()
{
    emit p_ptr->receivingObjectInfo(objectSizeBuffer, iObexBufObject->BytesReceived());
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::PutCompleteIndication()
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::PutCompleteIndication()
{
    TFileName file = KTempFile();
    TParsePtr parsePtr ( file );
    TFileName filename = parsePtr.DriveAndPath();
    filename.Append(iObexBufObject->Name());

    SafeDelete(iObexBufObject);

    TInt err2 = BaflUtils::RenameFile(iFs, KTempFile, filename, CFileMan::EOverWrite);
    if( err2 )
    {
        emit p_ptr->error(QBtObjectExchangeServer::QObexServerSessionCopyFileError);
    }

    QString newFileStr = QString::fromUtf16(filename.Ptr(), filename.Size());
    emit p_ptr->receivingStopped(newFileStr);
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::GetRequestIndication()
// Called when a full get request has been received from the client.
// ----------------------------------------------------------------------------
//
CObexBufObject* QBtObjectExchangeServerPrivate
::GetRequestIndication( CObexBaseObject* aRequiredObject )
{
    TPtrC16 aName = TPtrC16(aRequiredObject->Name().Ptr());
    SafeDelete(iObexBufObjectTransmit);
    SafeDelete(iObexBufDataTransmit);

    try{
        QT_TRAP_THROWING(
        {
            iObexBufDataTransmit = CBufFlat::NewL( bufferGranularity );
            iObexBufObjectTransmit = CObexBufObject::NewL(NULL);
        });

        TParse parse;
        parse.Set(aName, NULL, NULL);
        TPtrC ptr = parse.NameAndExt();

        QT_TRAP_THROWING(
        {
            iObexBufObjectTransmit->SetNameL( ptr );
            iObexBufObjectTransmit->SetDataBufL(aName, iObexBufDataTransmit);
        });
    }
    catch(char * str)
    {
        emit p_ptr->transmittingRejected();
        return NULL;
    }

    emit p_ptr->transmittingStarted(QString::fromUtf16(aName.Ptr(), aName.Size()));
    return iObexBufObjectTransmit;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::GetPacketIndication()
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::GetPacketIndication()
{
    emit p_ptr->transmittingObjectInfo(0, iObexBufObjectTransmit->BytesSent());
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::GetCompleteIndication()
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::GetCompleteIndication()
{
    QString fileSent = "Unknown";
    if(iObexBufObjectTransmit)
        fileSent = QString::fromUtf16(iObexBufObjectTransmit->Name().Ptr(),
                                        iObexBufObjectTransmit->Name().Size());

    emit p_ptr->transmittingStopped(fileSent);
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::SetPathIndication()
// ----------------------------------------------------------------------------
//
TInt QBtObjectExchangeServerPrivate::SetPathIndication( const CObex
        ::TSetPathInfo& aPathInfo,
        const TDesC8& aInfo )
{
    try
    {
        QT_TRAP_THROWING(
            BaflUtils::EnsurePathExistsL( CEikonEnv::Static()->FsSession(), aPathInfo.iName));

        currentDir = QString::fromUtf16(aPathInfo.iName.Ptr(), aPathInfo.iName.Size());
    }
    catch(char* str)
    {
        currentDir = "";
        return KErrPathNotFound;
    }
    return KErrNone;
}

// ----------------------------------------------------------------------------
// QBtObjectExchangeServerPrivate::AbortIndication()
// ----------------------------------------------------------------------------
//
void QBtObjectExchangeServerPrivate::AbortIndication()
{
    emit p_ptr->clientAbortedTransmittion();
    SafeDelete(iObexBufObject);

    if(iTempFile)
        iFs.Delete( *iTempFile );
}

// End of File

