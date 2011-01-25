/*
 * QBtObjectExchangeServer.h
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

#ifndef QBTOBJECTEXCHANGESERVER_H_
#define QBTOBJECTEXCHANGESERVER_H_

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QBtServiceAdvertiser.h>
#include <QObject>

//Forward declaration
class QBtObjectExchangeServerPrivate;

/**
 * This class is used to create an OBEX server.
 * After instantiation user can call startServer(const QString&)
 * to start the server. From the serviceName argument user can
 * specify the name of the OBEX service which is how the service will
 * be identified by other devices. By default the name is MyOBEX.
 *
 * If sever started successfully and is ready to use then serverStarted() signal is emitted.
 * At destruction time or if user calls stopServer(),terminates the service transmittion
 * and disconnects from any device connected and serverStopped() signal is emitted.
 *
 * All the remaining signals are only for feedback in case any action that is done.
 */
class DLL_EXPORT QBtObjectExchangeServer : public QObject
{
    Q_OBJECT

public:
    enum ErrorCode{
        QObexServerUndefined,
        QObexServerFailedToInitialize,
        QObexServerInUse,
        QObexServerClientAbortedTransfer,
        QObexServerAbortedTransmission,
        QObexServerSessionError,
        QObexServerSessionPutRequestProblem,
        QObexServerSessionCopyFileError
    };

public:
    QBtObjectExchangeServer(QObject* parent);
    ~QBtObjectExchangeServer();

    /**
     * isConnected()
     * @return true if the server is connected.
     */
    bool isConnected();

    /**
     * getTransmittingServiceInfo()
     * If startServer() function is already called then using this function
     * the user can acquire the information of the transmitting service that
     * represents this server to the outside world.
     */
    QBtService getTransmittingServiceInfo();

protected:
    void setTransmittingService(const QBtService& service);

    /**
     * startAdvertisingService()
     *
     * Initializes a new advertiser and starts the transmitting of the
     * service passed as argument.
     */
    void startAdvertisingService(const QBtService& service);

    /**
     * stopAdvertisingService()
     *
     * Stops the transmittion of the service (if any)
     */
    void stopAdvertisingService();

public slots:
    /**
     * startServer()
     *
     * Starts OBEX server by initializing the components needed
     * and by advertising the service by which the server will
     * be identified by other devices.
     */
    void startServer(const QString& serviceName = "MyOBEX");

    /**
     * stopServer()
     *
     * Stops the server, terminates the service transmittion and disconnects
     * from any device connected
     */
    void stopServer();

signals:
    /**
     * Emitted when server is successfully started and ready to use.
     */
    void serverStarted();

    /**
     * Emitted when server is disconnected and stopped
     */
    void serverStopped();

    /**
     * Emitted in case of error
     */
    void error(ErrorCode code);

    /**
     * Emitted as a feedback of the running receive status of
     * any data (file or byte buffer)
     */
    void receivingObjectInfo(int overalDataSize, int bytesSent);

    /**
     * Emitted when the transmittion of a file or byte buffer is started from a remote client
     * to the local device.
     */
    void receivingStarted();

    /**
     * Emitted when a receiving of a file or byte buffer is stopped,
     * being successfull or not.
     *
     * @param receivedFileName
     */
    void receivingStopped (const QString & receivedFileName);

    /**
     * If the server (local device) sends a request to start a transmittion to the remote client
     * connected, the client reserves the right to deny the transmittion before it begins.
     * In that case transmittingRejected() is emitted.
     */
    void transmittingRejected();

    /**
     * Emitted as a feedback of the running transmittion status of
     * any data (file or byte buffer)	 *
     */
    void transmittingObjectInfo(int overalDataSize, int byteSent);

    /**
     * Emitted when a transmittion is started from the local device to a remote device.
     */
    void transmittingStarted (const QString & transmittingfileName);

    /**
     * Emitted upon successfull or not end of transmittion from
     * the local device to a remote client.
     */
    void transmittingStopped (const QString & transmittingfileName);

    /**
     * Emitted if during a transmittion the client aborts it.
     */
    void clientAbortedTransmittion();

private:    
    QBtServiceAdvertiser* 			_advertiser;
    QBtService* 					_service;
    QBtObjectExchangeServerPrivate* _implPtr;    
    bool 							_isConnected;
    
    friend class QBtObjectExchangeServerPrivate;
};

#endif /* QBTOBJECTEXCHANGESERVER_H_ */
