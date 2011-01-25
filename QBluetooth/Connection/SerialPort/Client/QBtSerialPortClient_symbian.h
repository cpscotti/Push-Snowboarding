/*
 * QBtSerialPortClient_symbian.h
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

#ifndef QBTSERIALPORTCLIENT_SYMBIAN_H_
#define QBTSERIALPORTCLIENT_SYMBIAN_H_

#include "QBtSerialPortClient.h"
#include <QTimer.h>

#include <e32base.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>

class QBtSerialPortClientPrivate : public CActive
{
		
public:

	/*!
	 * NewL()
	 *
	 *  Create new QBtSerialPortClientPrivate object
	 * return a pointer to the created instance of QBtSerialPortClientPrivate
	 */
	static QBtSerialPortClientPrivate* NewL(QBtSerialPortClient* publicClass);

	/*!
	 * NewLC()
	 *
	 */
	static QBtSerialPortClientPrivate* NewLC(QBtSerialPortClient* publicClass);

	/*!
	 * ~QBtSerialPortClientPrivate()
	 *
	 *  Destroy the object and release all memory objects
	 */
	~QBtSerialPortClientPrivate();

	/*!
	 * ConnectL()
	 *
	 *  Creates a connection to given remote address on given port.
	 *
	 * param aName the name of the slave to connect to.
	 * param aAddr the address of the slave to connect to.
	 * param aPort the port on slave to connect to.
	 *
	 * return the status of connection attempt
	 */
	TBool ConnectL(const QBtDevice& remoteDevice, const QBtService& remoteService);

	/*!
	 * Disconnect()
	 *
	 *  Disconnects connector from slave.
	 */
	void Disconnect();

	/*!
	 * SendDataL()
	 *
	 *  Sends given data to slave.
	 *
	 * param aData the message to send to slave.
	 */
	void SendData(const QString& data);
	void SendData(const QByteArray& data);
	
	/*!
	 * IsConnected()
	 * 
	 * return the connection state
	 */
	TBool IsConnected();

protected:

	/*!
	 * RunL()
	 *
	 *  Handle the active objects request completion.
	 */
	void RunL();

	/*!
	 * DoCancel()
	 *
	 *  Cancels the outstanding request on active object.
	 */
	void DoCancel();

	TInt RunError(TInt aError);

private:

	/*!
	 * QBtSerialPortClientPrivate()
	 *
	 *  Perform the first phase of two phase construction
	 *
	 * @param the qt public class
	 */
	QBtSerialPortClientPrivate(QBtSerialPortClient* publicClass);

	/*!
	 * ConstructL()
	 *
	 */
	void ConstructL();

	/*!
	 * HandleConnectedDataReceived()
	 *
	 *  Handles the data received from a slave.  the handling request
	 * will be passed onto handling observer.
	 *
	 * param aName the name of the slave that sent the data.
	 * param aData the data received.
	 */
	void HandleConnectorDataReceivedL(const QString & aData);

	/*!
	 * HandleConnectorConnectedL()
	 *
	 *  Handles the event of server connection
	 */
	void HandleConnectorConnectedL();

	/*!
	 * HandleConnectorDisconnectedL()
	 *
	 *  Handles the event of server disconnection
	 */
	void HandleConnectorDisconnectedL();
	
	/*!
	 * HandleConnectorDataSentL()
	 * 
	 * Data successfully sent to server
	 */
	void HandleConnectorDataSentL();

	/*!
	 * ReceiveData()
	 *
	 *  Wait for and receive data from slave.
	 */
	void ReceiveData();
	
private:
	
	static TInt ConnectTimerCallBack (TAny* aPtr);
	
	void StartConnectionTimer();
	void CancelConnectionTimer();

private: // data

	// remote device
	QBtDevice device;
	// remote service
	QBtService service;
	// connecting socket
	RSocket iSock;
	// buffer holding received data
	TBuf8<128> iBuffer;
	// length of received data
	TSockXfrLength iLen;
	// socket server handle
	RSocketServ iSocketServ;
	
	// the state of the connector
	enum TState
	{
		ENone = 1,
		EIdle,
		EConnecting,
		EDisconnecting,
		EWaiting,
                ESending
	};

	TState iState;

	QBtSerialPortClient* p_ptr;
	
	CPeriodic* iTimer;
		
	TBTSockAddr _addr;
	
	HBufC8* iMessage;

        int connFailureCnt;
        int pipeDepth;
        static const int maxPipeDepth = 5;
};

#endif /* QBTSERIALPORTCLIENT_SYMBIAN_H_ */
