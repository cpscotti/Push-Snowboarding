/*
 * QBtSerialPortClient_win32.h
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

#ifndef QBTSERIALPORTCLIENT_WIN32_H_
#define QBTSERIALPORTCLIENT_WIN32_H_

#include "./QBtSerialPortClient.h"
#include "../../WinSerialPort/Tserial_event.h"
#include <QBtAuxFunctions.h>

class QBtSerialPortClientPrivate
{
public:
	QBtSerialPortClientPrivate(QBtSerialPortClient* publicClass);
	~QBtSerialPortClientPrivate();
	
	bool IsConnected();
	void Disconnect();
	/**
	* Connect()
	* Connect to the server
	* INFO: if remoteService has no handle information then the classId
	* field is used to connect. (Tested class id working QBtConstants::SerialPort)
	*
	* @param remoteDevice the remote server to connect to
	* @param remoteServive the remote service of the server
	*/
	void Connect(const QBtDevice& remoteDevice, const QBtService& remoteService);
	void SendData(const QString& data);
	void SendData(const QByteArray& data);
	
private:
	/**
	 * PLugInCommPort()
	 * Plug in a virtual comm port given by the OS.
	 * This comm port will be used later on to communicate.
	 */
	bool PLugInCommPort();
	void DeinitializeCommPort();
	
	/*
	 * ConnectBtSerialPortProfile_Handle()
	 * This function is called when remoteService has a valid handle.
	 * and the connecting port.
	 */
	bool ConnectBtSerialPortProfile_Handle();
	
	/*
	 * ConnectBtSerialPortProfile_ServClass()
	 * Used when the only info provided about the service is its class
	 * and the connecting port.
	 */
	bool ConnectBtSerialPortProfile_ServClass();
	
	bool InitializeSerialPortHandle();
	void CloseSerialPortHandle();
	
	static void SerialEventManager(uint32 object, uint32 event);
	void SerialCallback(Tserial_event *com_source, uint32 event);
	
private:
	// the server device info
	QString incomingDataBuffer;
	BTUINT32 OSComPort;
	HANDLE serialHandle;
	
	QBtDevice* server;
	QBtService* connectingService;
	BTCONNHDL connectionHandle;
	Tserial_event *com;
	
	// Qt parent public class
	QBtSerialPortClient* p_ptr;
};

#endif /* QBTSERIALPORTCLIENT_WIN32_H_ */
