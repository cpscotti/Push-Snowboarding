/*
 * QBtSerialPortClient.h
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

#ifndef QBTSERIALPORTCLIENT_H_
#define QBTSERIALPORTCLIENT_H_

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QObject>
#include <QByteArray>

//forward declaration
class QBtSerialPortClientPrivate;

/**
 * Class that provides the mechanism to connect to a bluetooth serial port
 * server and communicate.
 *
 * Calling connect(const QBtDevice&, const QBtService&) user specifies
 * on which device to connect(on which serial port server) and on which
 * bluetooth service. The QBtDevice instance of the server can be
 * aquired using QBtDeviceDiscoverer or user can create his own instance
 * and set at least the QBtAddress field. Also the QBtService can be
 * aquired using QBtServiceDiscoverer targeting the beforementioned device.
 *
 * On successfull connection, connectedToServer() singal is emitted and
 * user is able to sendData(const QString&) to the server. If successfull
 * then dataSend() signal is emitted.
 *
 * If data is recieved from the server, dataReceived(const QString) signal is
 * emitted containing the bytes recieved. At this point user must be carefull
 * to read the data recieved using the same encoding as the sender (in this
 * case serial port server). QString contains all the necessary functions for
 * character encoding that is why is QString is used instead of QByteArray.
 *
 * When connection is no longer needed, disconnect() can be called.
 * disconnectedFromServer() is then emitted.
 *
 * In case of error error(QBtSerialPortClient::ErrorCode) signal is emitted.
 */
class DLL_EXPORT QBtSerialPortClient : public QObject
{
    Q_OBJECT

public:    
    enum ErrorCode
    {
        ErrorAlreadyInUse,
        ErrorNotSupported,
        ErrorUnavailable,
        ErrorOpeningConnection,
        ErrorAlreadyConnected,
        ErrorUndefinedError,
        ErrorUnableToInitializePort,
        ErrorNoDeviceSelected,
        ErrorNoServiceSelected,
        ErrorConnectionError,
        ErrorConnectionTimeout,
        ErrorOnDisconnecting
    };

public:
    /**
     * A Serial port client.
     * According to the bluetooth specification, up to 7 instances of this class
     * can be connected simultaneously to other servers.
     */
    QBtSerialPortClient(QObject* parent);

    /**
     * Destructor.
     *
     * If isConnected() then disconnect() is called.
     */
    ~QBtSerialPortClient();

    /**
     * isConnected()
     *
     * Returns whether or not the client is connected to a remote server.
     */
    bool isConnected();

public slots:
    /**
     * connect()
     * RemoteDevice must contain at least the device address.
     * and remoteService at least a class ID (in case of SerialPort port field is also needed).
     * The remoteService info can be acquired by QBtServiceDiscoverer.
     */
    void connect(const QBtDevice& remoteDevice, const QBtService& remoteService);

    /**
     * disconnect()
     * Disconnects from the remote server if previously succusfully connected.
     */
    void disconnect();

    /**
     * sendData()
     * Send a string to the server.
     * If text is transmitted, it is up to the user to decide which text encoding to use.
     * Upon succesfull transmittion, dataSent() signal is emitted.
     */
    void sendData(const QString& data);
    
    /**
     * Send a array of bytes to the server, as is.
     * Upon succesfull transmittion, dataSent() signal is emitted.
     */         
    void sendData (const QByteArray & data);

signals:
    /**
     * Emitted when successfully disconnected from the remote server (initiated from client).
     */
    void disconnectedFromServer();
    
    /**
     * Emitted when the server initiated a disconnection.
     */
    void connectionResetByPeer ();    

    /**
     * Emitted when successfully connected to the remote server.
     */
    void connectedToServer();

    /**
     * Emitted when after sendData(QString) is called, if the data is send
     * successfully.
     */
    void dataSent();

    /**
     * Emitted as feedback when data are received successfully.
     */
    void dataReceived(const QString & data);

    /**
     * Emitted in case of error.
     */
    void error(QBtSerialPortClient::ErrorCode error);

private:
    friend class QBtSerialPortClientPrivate;
    QBtSerialPortClientPrivate* _implPtr;
};

#endif /* QBTSERIALPORTCLIENT_H_ */
