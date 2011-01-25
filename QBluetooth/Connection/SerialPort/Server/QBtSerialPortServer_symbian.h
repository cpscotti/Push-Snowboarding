/*
 * QBtSerialPortServer_symbian.h
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

#ifndef QBTSERIALPORTSERVER_SYMBIAN_H_
#define QBTSERIALPORTSERVER_SYMBIAN_H_

#include <e32base.h>

#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btmanclient.h> // v2.0 bt_sock.h
#include <e32property.h>

//KErrPermissionDenied alredyexists
#include "QBtSerialPortServer.h"

class QBtSerialPortServerPrivate : public CActive
{
public:

	/*!
	 * NewL()
	 *
	 * Create new QBtSerialPortServerPrivate object
	 * return a pointer to the created instance of QBtSerialPortServerPrivate
	 */
	static QBtSerialPortServerPrivate* NewL(QBtSerialPortServer* publicClass);

	/*!
	 * NewLC()
	 *
	 */
	static QBtSerialPortServerPrivate* NewLC(QBtSerialPortServer* publicClass);

	/*!
	 * ~CConnector()
	 *
	 * Destroy the object and release all memory objects
	 */
	~QBtSerialPortServerPrivate();

	/*!
	 * StartListener()
	 *
	 * Starts the listener.  listener will open a listening socket and
	 * listen on incoming connections.
	 */
	void StartListenerL();

	/*!
	 * StopListener()
	 *
	 * Stops the listener, closes the listening socket.
	 */
	void StopListenerL();

	/*!
	 * SendData()
	 *
	 * Sends data to master.  user will be prompted to enter the
	 * message to send.
	 *
	 * param aData the message to be send.
	 */
	void SendData(const QString & data);

	/*!
	 * IsConnected()
	 *
	 * check if listener is connected to.
	 *
	 * return true if listener has a connection from master.
	 */
	TBool IsConnected();
	
	void SetListeningQueueSize(TInt size);

protected:

	/*!
	 * RunL()
	 *
	 * Handle the active objects request completion.
	 */
	void RunL();

	/*!
	 * DoCancel()
	 *
	 * Cancels the outstanding request on active object.
	 */
	void DoCancel();

	TInt RunError(TInt aError);

private:

	/*!
	 * QBtSerialPortServerPrivate()
	 *
	 * Perform the first phase of two phase construction
	 *
	 * @param The qt public class
	 */
	QBtSerialPortServerPrivate(QBtSerialPortServer* publicClass);

	/*!
	 * ConstructL()
	 *
	 */
	void ConstructL();

	/*!
	 * ReceiveData()
	 *
	 * Receive data from listening socket.
	 */
	void ReceiveData();

	/*!
	 * HandleListenerDataReceivedL()
	 *
	 * Handles the data received from a master.
	 *
	 * param aData the data received.
	 */
	void HandleListenerDataReceivedL(const QString & aData);

	/*!
	 * HandleListenerConnectedL()
	 *
	 * Handles the event of slave connection (slave was connected to
	 * by master)
	 */
	void HandleListenerConnectedL();

	/*!
	 * HandleListenerDisconnectedL()
	 *
	 * Handles the event of slave disconnection, the master connection
	 * to slave was terminated
	 */
	void HandleListenerDisconnectedL();

	/*!
	 * HandleListenerDataSend()
	 *
	 * Handles the event of sending successfully data to the server.
	 */
	void HandleListenerDataSendL();


private: // data

	// listening socket
	RSocket iListenSock;
	// accepted socket
	RSocket iSock;
	// length of received data
	TSockXfrLength iLen;
	// buffer holding received data
	TBuf8<128> iBuffer;
	// socket server handle
	RSocketServ iSocketServ;
	// listener connection status
	TBool iIsConnected;
	// the state of the listener
	TInt queueSize;
	enum TState
	{
		ENone = 1,
		EConnecting,
		EWaiting,
		ESending
	};

	TState iState;
	
	//if true when client disconnects, restarts listener
	// to accept the next
	TBool restartAtDisconnect;
	

	QBtSerialPortServer* p_ptr;
};
#endif /* QBTSERIALPORTSERVER_SYMBIAN_H_ */
