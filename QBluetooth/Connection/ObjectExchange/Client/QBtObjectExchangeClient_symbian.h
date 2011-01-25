/*
 * QBtObjectExchangeClient_symbian.h
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

#ifndef QBTOBJECTEXCHANGECLIENT_SYMBIAN_H_
#define QBTOBJECTEXCHANGECLIENT_SYMBIAN_H_

#include <QBtObjectExchangeClient.h>

#include <f32file.h>
#include <e32base.h>
#include <obex.h>

class QBtObjectExchangeClientPrivate : public CActive
{

public: // Constructors and destructor.
    /**
     * NewL
     * Construct a QBtObjectExchangeClientPrivate
     * @param pointer to qt public class
     * @return a pointer to the created instance of
     * QBtObjectExchangeClientPrivate
     */
    static QBtObjectExchangeClientPrivate* NewL( QBtObjectExchangeClient* publicClass );

    /**
     * NewLC
     * Construct a QBtObjectExchangeClientPrivate
     * @param pointer to qt public class
     * @return a pointer to the created instance
     * of QBtObjectExchangeClientPrivate
     */
    static QBtObjectExchangeClientPrivate* NewLC( QBtObjectExchangeClient* publicClass );

    /**
     * ~QBtObjectExchangeClientPrivate
     * Destroy the object and release all memory objects.
     * Close any open sockets
     */
    virtual ~QBtObjectExchangeClientPrivate();

    /**
     * ConnectToServer
     * Connect to the server
     * @param remoteDevice the remote server to connect to
     * @param remoteServive the remote service of the server
     */
    void ConnectToServer(const QBtDevice& remoteDevice, const QBtService& remoteService);

    /**
     * Disconnect
     * Disconnect from the remote machine, by sending an
     * OBEX disconnect, and closing the transport on (and
     * regardless of) response from the server.
     */
    void Disconnect();

    /**
     * SendFile
     * Send a file to a service on a remote machine
     */
    void SendFile(const QString& fileName);

    /**
     * SendData
     * Send a byte array to a service on a remote machine
     */
    void SendData(const QByteArray& data, const QString& bufferName);


    /**
     * Abort
     * Send the OBEX aborts command to the remote machine
     */
    void Abort();

    /**
     * IsConnected
     * @return ETrue if the client is connected
     */
    TBool IsConnected();

    /**
     * IsBusy
     * @return ETrue if the client is performing some operation.
     */
    TBool IsBusy();

    /**
     * SetPath()
     * Set the current path of the remote device OBEX session.
     */
    void SetPath(const QString & path);

    /**
     * GetFile
     *
     * Request from the server to GET the file defined in the argument
     */
    void GetFile(const QString& localPath, const QString& remoteFileNameFull);

    /**
     * GetByteBuffer()
     *
     * Request from the server to GET the data buffer defined in the argument
     */
    void GetByteBuffer(const QString& dataName);

protected:    // from CActive

    /**
     * DoCancel
     * Cancel any outstanding requests
     */
    void DoCancel();

    /**
     * RunL
     * Respond to an event
     */
    void RunL();

private:    // Constructors

    TBool CheckServiceAvailability();

    /**
     * CMessageClient
     * @param pointer to qt public class
     */
    QBtObjectExchangeClientPrivate( QBtObjectExchangeClient* publicClass );

    /**
     * ConstructL
     * Perform second phase construction of this object
     */
    void ConstructL();

    ///////////////
    ///	Handles ///
    ///////////////
    void HandleServerConnection();
    void HandleObjectSent();
    void HandleDisconnection();
    void HandleObjectReceived();
    void HandleBrowsingFinsished();
    void HandleSettingPathCompletion();

private:    // data
    // This is set when a get operation is requested to the server.
    // Indicates where the file will be saved on the local machine
    // after successfull retrieving.
    QString tmpLocalPath;
    //the remote device server
    //QBtDevice* server;
    //the remote server service to connect to
    //QBtService* service;

    HBufC* iNewFile;

    /**
     * TState
     * The state of the active object, determines behaviour
     * within the RunL method.
     * EWaitingToGetDevice waiting for the user to select a device
     * EGettingDevice searching for a device
     * EGettingService searching for a service
     * EGettingConnection connecting to a service on a remote machine
     * EWaitingToSend sending a message to the remote machine
     * EDisconnecting disconnecting the server
     */
    enum TState
    {
        EGettingConnection,
        EWaitingToSend,
        EWaitingToReceiveFile,
        EWaitingToReceiveData,
        EDisconnecting,
        EBrowsing,
        ESettingPath,
        ENone
    };

    CObex::TSetPathInfo path;

    /**
     * iState the state of the active object,
     * determines behaviour within the RunL method.
     */
    TState iState;

    /**
     * iClient manages the OBEX client connection
     * Owned by QBtObjectExchangeClientPrivate
     */
    CObexClient* iClient;

    /**
     * iCurrObject the OBEX object to transfer
     * Owned by QBtObjectExchangeClientPrivate
     */
    CObexBaseObject* iCurrObject;
    CBufBase* buffer;
    CObex::TSetPathInfo pathInfo;

    QBtObjectExchangeClient* p_ptr;
};

#endif /* QBTOBJECTEXCHANGECLIENT_SYMBIAN_H_ */
