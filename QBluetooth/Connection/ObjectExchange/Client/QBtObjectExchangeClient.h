/*
 * QBtObjectExchangeClient.h
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

#ifndef QBTOBJECTEXCHANGECLIENT_H_
#define QBTOBJECTEXCHANGECLIENT_H_

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QBtServiceAdvertiser.h>
#include <QObject>

//forward declaration
class QBtObjectExchangeClientPrivate;

/**
 * Class that provides the mechanism to connect to a remote
 * OBEX server and send or recieve files or raw data.
 *
 * After instantiation user can call connectToServer(QBtDevice&, QBtService&)
 * to connect to the remote server specified by the remoteDevice argument
 * to its service provide by remoteService argument.
 *
 *  P.S. At the remote service, one of the supported protocols must be
 * QBtConstants::OBEX. Suggestion, use a service whose class is equal to
 * QBtConstants::OBEXObjectPush or QBtConstants::OBEXFileTransfer.
 * Again, it is suggested to use QBtConstants::OBEXObjectPush.
 * Both options will work fine if the user wants to send a file to
 * the server, but at present further operations can be done using
 * QBtConstants::OBEXObjectPush.
 *
 * Signal connectedToServer() is emitted upon successfull connection to the OBEX server.
 *
 * After the successfull connection to the server, user can call putFile(const QString&)
 * to send to the remote server the file specified by the string passed as argument.
 * If successfull then objectSent() signal is emitted.
 *
 * Also user is able to send a request to get a file from the remote server specifing
 * the absolute path of the file to be fetched at the remote device's file system.
 * Also a local path must be set which is where the file will be stored after the
 * successfull transfer.
 * getFile(const QString& localPath, const QString& remoteFileNameFull)
 * If successfull then fileReceived(const QString) is emitted.
 *
 * getByteBuffer() and putByteBuffer() are implemented but not tested.
 * If successfull they emit dataReceived(const QByteArray) and objectSent() respectively.
 *
 * If connected to an OBEX FTP bluetooth service then setPath() can be called
 * to set the remote working directory. If successfull then remotePathSet(const QString)
 * will be emitted.
 *
 * User can abort any transmittion in progress by calling abort().
 *
 * User can disconnect from the server at any time or the disconnection will take place at
 * destruction time. Any transmittion in progress will be terminated instantly.
 * A successfull disconnection emits disconnectedFromServer() signal.
 */
class DLL_EXPORT QBtObjectExchangeClient : public QObject
{
    Q_OBJECT

public:
    enum ErrorCode{
        OBEXClientConnectionError,
        OBEXClientSendError,
        OBEXClientReceiveError,
        OBEXClientDisconnected,
        OBEXClientInUse,
        OBEXClientInternalError,
        OBEXClientNoSelectedDevice,
        OBEXClientNoSelectedService,
        OBEXClientUnableToSetPath
    };

public:
    QBtObjectExchangeClient(QObject* parent);
    ~QBtObjectExchangeClient();

    /**
     * connectToServer()
     *
     * Connect to a remote server defined by parameter 1 to its service
     * defined by parameter 2.
     *
     * P.S. At the remote service, one of the supported protocols must be
     * QBtConstants::OBEX. Suggestion, use a service whose class is equal to
     * QBtConstants::OBEXObjectPush or QBtConstants::OBEXFileTransfer.
     * Again, it is suggested to use QBtConstants::OBEXObjectPush.
     * Both options will work fine if the user wants to send a file to
     * the server, but at present further operations can be done using
     * QBtConstants::OBEXObjectPush.
     *
     * @param remoteDevice
     * @param remoteService
     */
    void connectToServer(const QBtDevice& remoteDevice, const QBtService& remoteService);

    /**
     * getFile()
     * Get a file from the remote Server
     *
     * @param localPath the path where the file will be saved
     * @param remoteFileNameFull the name of the file to be transfered
     *							containing the full path of its remote location
     */
    void getFile(const QString& localPath, const QString& remoteFileNameFull);

    /**
     * getByteBuffer(const QString& dataName)
     * Request from the server to GET the data buffer defined in the argument
     *
     * @param dataName The name of the buffer.
     */
    void getByteBuffer(const QString& dataName);

    /**
     * Send a specified file from the local file system to the remote
     * OBEX server.
     *
     * @param fileName The file name of the file to be sent. The absolute
     *                 path of the file is required.
     */
    void putFile(const QString& fileName);

    /**
     * putByteBuffer()
     *
     * send the server a byte sequence.
     * @param data,  the data of the buffer
     * @param bufferName, the name of the buffer (used for convenience on the data
     * 		proccessing of the server)
     */
    void putByteBuffer(const QByteArray& data, const QString& bufferName);

    /**
     * abort()
     *
     * abort any panding or running transmittion.
     */
    void abort();

    /**
     * setPath(const QString path)
     *
     * Set the current path of the remote device OBEX session.
     * @param path The remote folder to be set as current
     */
    void setPath(const QString & path);

    // accessor commands

    /**
     * Get info about the service connected to.
     * @return An object containing the service info
     */
    QBtService getTransmittingService();

    /**
     * Get info about the server connected to.
     * @return An object containing the remote server device info.
     */
    QBtDevice getServerDevice();
    
    
    /**
     * Checks if the client is performing some operation.
     */
    bool isBusy() const;
    
    /**
     * Checks if the client got a connection (it does not check *now* if the client is still connected).
     */
    bool isConnected() const;
    
    
    

public slots:
    void disconnect();

signals:

    /**
     * Emitted in case of error
     */
    void error(QBtObjectExchangeClient::ErrorCode code);

    /**
     * Emitted upon successfull connection to the OBEX server.
     */
    void connectedToServer();

    /**
     * Emitted upon disconnection from the server.
     */
    void disconnectedFromServer();

    /**
     * Emitted upon successfull transmittion of an object
     */
    void objectSent();

    /**
     * Emitted when a file is received from the server
     */

    void fileReceived (const QString & fileName);

    /**
     * Emitted when a byte buffer is received from the server
     */
    void dataReceived (const QByteArray & data);

    /**
     * Emitted upon successfull set of the remote current working directory.
     */
    void remotePathSet (const QString & pathName);

private:
    friend class QBtObjectExchangeClientPrivate;
    QBtObjectExchangeClientPrivate* _implPtr;
};

#endif /* QBTOBJECTEXCHANGECLIENT_H_ */
