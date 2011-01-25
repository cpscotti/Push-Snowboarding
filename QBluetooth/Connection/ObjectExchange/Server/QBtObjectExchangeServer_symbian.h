/*
 * QBtObjectExchangeServer_symbian.h
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

#ifndef QBTOBJECTEXCHANGESERVER_SYMBIAN_H_
#define QBTOBJECTEXCHANGESERVER_SYMBIAN_H_

#include <e32base.h>
#include <obex.h>
#include <btsdp.h>
#include <btmanclient.h>
#include <e32def.h>
#include <bt_sock.h>

#include "QBtObjectExchangeServer.h"

#ifdef __SERIES60_3X__
_LIT(KTempFile,"c:\\data\\temp.dat");
#else
_LIT(KTempFile,"c:\\system\\data\\temp.dat");
#endif

// CLASS DECLARATIONS
/**
 * QBtObjectExchangeServer
 * Handles the server interactions with the remote client, namely
 * the connection, receiving of messages and disconnection.
 */
class QBtObjectExchangeServerPrivate : public CBase, public MObexServerNotify
{
public: // Constructors and destructor.
	/**
	 * NewL()
	 * Construct a QBtObjectExchangeServerPrivate
	 * @param publicClass to the qt public class
	 * @return a pointer to the created instance of QBtObjectExchangeServerPrivate
	 */
	static QBtObjectExchangeServerPrivate* NewL( QBtObjectExchangeServer* publicClass );

	/**
	 * NewLC()
	 * Construct a QBtObjectExchangeServerPrivate
	 * @param publicClass the qt public class
	 * @return a pointer to the created instance of QBtObjectExchangeServerPrivate
	 */
	static QBtObjectExchangeServerPrivate* NewLC( QBtObjectExchangeServer* publicClass );

	/**
	 * ~QBtObjectExchangeServerPrivate()
	 * Destroy the object and release all memory objects
	 */
	virtual ~QBtObjectExchangeServerPrivate();

	/**
	 * StartServer()
	 * Starts the server
	 */
	TBool StartServer();

	/**
	 * Disconnect()
	 * Disconnects the server
	 */
	void Disconnect();

	/**
	 * IsConnected()
	 * @return ETrue if the server is connected.
	 */
	TBool IsConnected();
	
	/**
	 * SetListeningQueueSize()
	 * Set the size of the listening queue of the server.
	 * @param size
	 */
	void SetListeningQueueSize(TInt size);
	
	QString GetCurrentWorkingDirectory();

private:    // Functions from base classes

	/**
	 * QBtObjectExchangeServerPrivate
	 * Constructs this object
	 * @param publicClass the qt public class
	 */
	QBtObjectExchangeServerPrivate( QBtObjectExchangeServer* publicClass );

	/**
	 * ConstructL
	 * 2nd phase construction of this object
	 */
	void ConstructL();

	/**
	 * InitialiseServerL
	 * Initialises the server
	 */
	TBool InitialiseServerL();

	/**
	 * SetSecurityWithChannelL
	 * Sets the security on the channel port
	 * And returns the channel available.
	 * @param aAuthentication is authentication used
	 * @param aEncryption is encryption used
	 * @param aAuthorisation is authorisation used
	 * @param aDenied is denied used
	 * @return an available channel
	 */
	TInt SetSecurityWithChannelL ( TBool aAuthentication,
			TBool aEncryption,
			TBool aAuthorisation,
			TBool aDenied );

private:    // from MObexServerNotify (these methods are defined
	//as private in MObexServerNotify)

	/**
	 * ErrorIndication
	 * Receive error indication.
	 * It is only called for fatal errors that cause the OBEX connection to terminate.
	 * @param aError the error code
	 */
	void ErrorIndication( TInt aError );

	/**
	 * TransportUpIndication
	 * Called when the underlying socket transport connection is made from
	 * a remote client to the server
	 */
	void TransportUpIndication();

	/**
	 * TransportDownIndication
	 * Transport connection is dropped
	 */
	void TransportDownIndication();

	/**
	 * ObexConnectIndication
	 * Invoked when an OBEX connection is made from a remote client
	 * @param aRemoteInfo connection information supplied by
	 * the remote machine
	 * @param aInfo holds further information about the
	 * requested connection
	 * @return system wide error code
	 */
	TInt ObexConnectIndication( const TObexConnectInfo& aRemoteInfo,
			const TDesC8& aInfo );

	/**
	 * ObexDisconnectIndication
	 * OBEX server has been disconnected
	 * @param aInfo contains information about the disconnection
	 */
	void ObexDisconnectIndication( const TDesC8& aInfo );

	/**
	 * PutRequestIndication
	 * @return the CObexBufObject in which to store
	 * the transferred object
	 */
	CObexBufObject* PutRequestIndication();

	/**
	 * PutPacketIndication
	 * @return system wide error code
	 */
	TInt PutPacketIndication();

	/**
	 * PutCompleteIndication
	 * @return system wide error code
	 */
	TInt PutCompleteIndication();

	/**
	 * GetRequestIndication
	 * Called when a full get request has been received from the client
	 * @param aRequestedObject holds details about the requested object
	 * @return the CObexBufObject in which return to the client
	 */
	CObexBufObject* GetRequestIndication( CObexBaseObject* aRequestedObject );

	/**
	 * GetPacketIndication
	 * @return system wide error code
	 */
	TInt GetPacketIndication();

	/**
	 * GetCompleteIndication
	 * @return system wide error code
	 */
	TInt GetCompleteIndication();

	/**
	 * SetPathIndication
	 * @param aPathInfo the information received in a SETPATH command
	 * @return system wide error code
	 */
	TInt SetPathIndication( const CObex::TSetPathInfo& aPathInfo,
			const TDesC8& aInfo );

	/**
	 * AbortIndication
	 * Server has been aborted
	 */
	void AbortIndication();

private:    // data
	// iObexServer manages the OBEX client connection
	// Owned by QBtObjectExchangeServerPrivate
	CObexServer* iObexServer;
	
	// iObexBufData the raw data that has been transferred
	// Owned by QBtObjectExchangeServerPrivate
	CBufFlat* iObexBufData;

	// Destination where the received file will be stored
	HBufC* iTempFile;

	// iCurrObject the OBEX object that has been transferred
	// Owned by QBtObjectExchangeServerPrivate
	CObexBufObject* iObexBufObject;

	//File Server session
	RFs iFs;
	
	// iObexBufData the raw data that has been transferred
	// Owned by QBtObjectExchangeServerPrivate
	CBufFlat* iObexBufDataTransmit;
	
	// Used in Get operations where client requests to retrieve
	// a file from the filesystem of the server
	CObexBufObject* iObexBufObjectTransmit;
	
	// Size of the listening queue
	TInt queueSize;
	TInt bufferGranularity;
	TInt objectSizeBuffer;
	
	QString currentDir;
	
	//QT public parent class
	QBtObjectExchangeServer* p_ptr;
};


#endif /* QBTOBJECTEXCHANGESERVER_SYMBIAN_H_ */
