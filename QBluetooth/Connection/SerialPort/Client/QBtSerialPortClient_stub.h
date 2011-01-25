/*
 * QBtSerialPortClient_stub.h
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

#ifndef QBTSERIALPORTCLIENT_STUB_H_
#define QBTSERIALPORTCLIENT_STUB_H_

#include "./QBtSerialPortClient.h"

class QBtSerialPortClientPrivate
{
public:
	QBtSerialPortClientPrivate(QBtSerialPortClient* publicClass);
	~QBtSerialPortClientPrivate();
	
	bool IsConnected();
	void Disconnect();
	void Connect(const QBtDevice& remoteDevice, const QBtService& remoteService);
	void SendData(const QString& data);
	
private:
	QBtSerialPortClient* p_ptr;
};

#endif /* QBTSERIALPORTCLIENT_STUB_H_ */
