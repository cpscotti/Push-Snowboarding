/*
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

#ifndef QBTSERIALPORTSERVER_H
#define QBTSERIALPORTSERVER_H

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QBtServiceAdvertiser.h>
#include <QObject>
#include <QByteArray>

//forward declaration
class QBtSerialPortServerPrivate;

/**
 * This class creates a Serial Port Server.
 *
 * After instantiation user can call startServer(const QString&)
 * to start the server and wait for a client to connect. From the
 * serviceName argument user can specify the name of the RFCOMM service
 * which is how the service will be identified by other devices. By default
 * the name is MyRFCOMM.
 * 
 * If server is started successfully then serverStarted() signal is emitted.
 *
 * When a remote client is connected to the local running server
 * clientConnected(const QBtAddress &) is emitted and from that point and
 * on the data transmittion can start.
 *
 * To send data to the client use sendData(const QString &). If successfull
 * dataSent() signal is emitted.
 *
 * When data are received successfully from the remote client dataReceived(const QString &)
 * signal is emitted.
 *
 * If client disconnects,  clientDisconnected() will be emitted and user must
 * call startServer(const QString &) to initialize server again to be ready to
 * accept new connection.
 *
 * The connection to a client (if any) is closed when stopServer() is called along
 * with the termination of advertising of the SPP service.
 *
 * In case of an error error(QBtSerialPortServer::ErrorCode) is called.
 */
class DLL_EXPORT QBtSerialPortServer : public QObject
{
    Q_OBJECT

public:
    
    enum ErrorCode
    {
        ErrorAlreadyInUse,
        ErrorNotSupported,
        ErrorUnavailable,
        ErrorUndefinedError
    };

public:
    QBtSerialPortServer(QObject* parent);
    ~QBtSerialPortServer();

    /**
     * Get the status of the connection.
     * @return true if connected.
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
     * Set the flag inside this object which represents the connection status.
     * NOTE: This flag is updated automatically whenever the connection status
     * changes.
     */
    void setConnectionStatus(bool connected);

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
    /*
     * When server is started, a custom bluetooth service is advertised
     * through QBtServiceAdvertiser. This service can be retrieved using
     * getTransmittingServiceInfo(). If needed, the user can set the name
     * of the transmitting service when calling this function.
     */
    void startServer (const QString& serviceName = "MyRFCOMM");

    /**
     * stopServer()
     *
     * Stops the server, terminates the service transmittion and disconnects
     * from any device connected
     */
    void stopServer();

    /**
     * sendData()
     *
     * If connected to a remote client, sends the data passed as argument
     * to the client. If successfull, dataSent() signal is emitted.
     *
     * @param data The data to be send.
     */
    void sendData (const QString & data);

signals:
    /**
    * Emitted when server is successfully started and ready to use.
    */
    void serverStarted();

    /**
     * Emitted when service advertising is terminated. If any client is connected the
     * a clientDisconnected() signal will be emitted as well.
     */
    void serverStopped();

    /**
     * Emitted when a remote client is connected to the local running server.
     */
    void clientConnected (const QBtAddress & clientAddress);

    /**
     * Emitted when a remote client is disconnected either by a normal disconnection
     * or by a problem to the connection (ex. loss of signal, power off bluetooth hardware device)
     */
    void clientDisconnected();

    /**
     * Emitted after calling sendData() function if data are send successfully to the remote client.
     */
    void dataSent();

    /**
     * Emitted when data are received successfully from the remote client.
     */
    void dataReceived(const QString & data);

    /**
     * Emitted in case of error.
     */
    void error(QBtSerialPortServer::ErrorCode code);

private:
    // Service Advertiser
    QBtServiceAdvertiser* _advertiser;
    
    //transmitting service
    QBtService* _service;
            
    //pointer to implementation
    QBtSerialPortServerPrivate *_implPtr;
    
    // status
    bool _isConnected;
    
    
    friend class QBtSerialPortServerPrivate;
    
};

#endif // QBTSERIALPORTSERVER_H
