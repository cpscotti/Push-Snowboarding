/*
 * QBtSerialPortServer_stub.h
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

#ifndef QBTSERIALPORTSERVER_STUB_H_
#define QBTSERIALPORTSERVER_STUB_H_

#include "QBtSerialPortServer.h"
#include <QBtAuxFunctions.h>

class QBtSerialPortServerPrivate
{
public:
    QBtSerialPortServerPrivate(QBtSerialPortServer* punlicClsass);
    ~QBtSerialPortServerPrivate();

    void StartListener();
    void StopListener();
    void SendData(const QString data);

private:
    QBtSerialPortServer* p_ptr;
};

#endif /* QBTSERIALPORTSERVER_STUB_H_ */
